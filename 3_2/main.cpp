#include "lua.hpp"
#include <windows.h> 
#include <iostream>
#include <string>

// for debug
void PrintLuaStack(lua_State* L)
{
	int stackTop = lua_gettop(L);
	int nIdx = 0;
	int nType;

	printf("--栈顶(v)(%d)--\n", stackTop);

	for (nIdx = stackTop; nIdx > 0; --nIdx)
	{
		nType = lua_type(L, nIdx);
		printf("(i:%d) %s(%s)\n", nIdx, lua_typename(L, nType), lua_tostring(L, nIdx));
	}

	printf("--栈底--\n");
}

int main() {
	//从Lua虚拟机L1中导出testTable
	lua_State* L1 = luaL_newstate();
	luaL_openlibs(L1);
	luaL_dofile(L1, "LuaCode/test.lua");
	void* pExportedTable = lua_export_table(L1, "testTable");

	lua_close(L1);

	
	//将Lua虚拟机L1中导出的testTable导入Lua虚拟机L2中
	lua_State* L2 = luaL_newstate();
	luaL_openlibs(L2);
	lua_import_table(L2, pExportedTable);
	lua_setglobal(L2, "testTable");
	luaL_dofile(L2, "LuaCode/test2.lua");
	
	lua_close(L2);
	
	system("pause");
}
