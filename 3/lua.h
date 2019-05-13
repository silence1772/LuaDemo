/*
** 'export' and 'import' table between Lua state
*/
LUA_API void *(lua_export_table) (lua_State *L, const char *name);
LUA_API void (lua_import_table) (lua_State *L, void *p);
