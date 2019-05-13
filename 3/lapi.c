/*
** 将allgc链表逆序
*/
static void reverse_allgc(lua_State *L) {
  global_State* g = G(L);
  GCObject* pre = NULL;
  GCObject* curr = g->allgc;
  GCObject* temp = NULL;
  while (curr != NULL) {
    temp = curr->next;
    curr->next = pre;
    pre = curr;
    curr = temp;
  }
  g->allgc = pre;
}


static void get_all_next(lua_State *L, Table *t, GCObject ***a, int *size, int *k) {
  /* 内存不足时两倍增长 */
  if (*k >= *size) {
    luaM_reallocvector(L, *a, *size, *size * 2, GCObject*);
    *size = *size * 2;
  }
  (*a)[(*k)++] = t->next; /* 保存该table在allgc链表中的后一个节点 */
  /* 遍历该table引用的其他对象 */
  unsigned int i;
  for (i = 0; i < t->sizearray; i++) {  /* 遍历数组部分 */
    if (ttype(&t->array[i]) == LUA_TTABLE)
      get_all_next(L, hvalue(&t->array[i]), a, size, k);
  }
  Node* n;
  Node* limit = gnode(t, cast(size_t, sizenode(t)));
  for (n = gnode(t, 0); n < limit; n++) {  /* 遍历哈希表部分 */
    if (ttype(gval(n)) == LUA_TTABLE)
      get_all_next(L, hvalue(gval(n)), a, size, k);
  }
}


static void check_pre(GCObject **a, int size, GCObject *del, GCObject *pre) {
  int i;
  for (i = 0; i < size; ++i) {
    if (a[i] == del) {
      a[i] = pre;
      return;
    }
  }
}


static void detach_in_allgc(lua_State *L, Table *t, GCObject **a, int size, int *k) {
  /* 从allgc链表中剥离 */
  if (a[*k] == NULL) { /* 前一个节点为空说明是allgc的第一个节点 */
    global_State* g = G(L);
    g->allgc = t->next;
  }
  else a[*k]->next = t->next;
  /* 确保前一个节点是有效的 */
  check_pre(a, size, t, a[*k]);
  ++(*k);
  t->next = NULL;
  /* 遍历该table引用的其他对象 */
  unsigned int i;
  for (i = 0; i < t->sizearray; i++) {  /* 遍历数组部分 */
    if (ttype(&t->array[i]) == LUA_TTABLE)
      detach_in_allgc(L, hvalue(&t->array[i]), a, size, k);
  }
  Node* n;
  Node* limit = gnode(t, cast(size_t, sizenode(t)));
  for (n = gnode(t, 0); n < limit; n++) {  /* 遍历哈希表部分 */
    if (ttype(gval(n)) == LUA_TTABLE)
      detach_in_allgc(L, hvalue(gval(n)), a, size, k);
  }
}


static void detach_table(lua_State *L, Table *t) {
  GCObject** previous = NULL;
  int size = 0;
  int k = 0;

  /* 逆转allgc链表 */
  reverse_allgc(L);
  /* 获取GCObject在allgc链表中的前一个节点 */
  size = t->sizearray + allocsizenode(t);
  previous = luaM_newvector(L, size, GCObject*);
  get_all_next(L, t, &previous, &size, &k);
  /* 恢复allgc链表 */
  reverse_allgc(L);

  /* 将table中的GCObject从allgc中剥离 */
  k = 0;
  detach_in_allgc(L, t, previous, size, &k);
  luaM_freearray(L, previous, cast(size_t, size));
}

/*
** Detach the table at given index from current Lua state
*/
LUA_API void *lua_export_table (lua_State *L, const char *name) {
  StkId o;
  Table *t = NULL;
  lua_lock(L);
  
  /* 获取table指针 */
  lua_getglobal(L, name);
  o = index2addr(L, -1);
  api_check(L, ttistable(o), "table expected");
  t = hvalue(o);
  lua_remove(L, -1);
  /* 切断全局变量对该table的引用 */
  lua_pushnil(L);
  lua_setglobal(L, name);
  /* 将该table从GC模块中剥离 */
  detach_table(L, t);

  lua_unlock(L);
  return t;
}


static void mergetable(lua_State *L, Table *t) {
  global_State* g = G(L);
  /* 加入allgc中 */
  t->next = g->allgc;
  g->allgc = t;
  /* 遍历table引用的其他对象 */
  unsigned int i;
  for (i = 0; i < t->sizearray; i++) {  /* 遍历数组部分 */
    if (ttype(&t->array[i]) == LUA_TTABLE)
      mergetable(L, hvalue(&t->array[i]));
  }
  Node* n;
  Node* limit = gnode(t, cast(size_t, sizenode(t)));
  for (n = gnode(t, 0); n < limit; n++) {  /* 遍历哈希表部分 */
    if (ttype(gval(n)) == LUA_TTABLE)
      mergetable(L, hvalue(gval(n)));
  }
}


/*
** Reconstruct a table from p (exported by lua_export_table) 
** and pushes it onto the stack
*/
LUA_API void lua_import_table (lua_State *L, void *p) {
  lua_lock(L);
  sethvalue(L, L->top, p); /* 将table压入栈顶 */
  api_incr_top(L);
  mergetable(L, cast(Table*, p)); /* 将table及其引用并入GC模块中 */
  lua_unlock(L);
}