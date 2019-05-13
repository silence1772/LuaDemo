﻿#include "lua.hpp"

int main() {

	//从Lua虚拟机L1中导出testTable
	lua_State* L1 = luaL_newstate();
	luaL_openlibs(L1);
	luaL_dofile(L1, "test.lua");
	//lua_getglobal(L1, "testTable");
	void* pExportedTable = lua_export_table(L1, "testTable");

	lua_close(L1);

	//将Lua虚拟机L1中导出的testTable导入Lua虚拟机L2中
	lua_State* L2 = luaL_newstate();
	luaL_openlibs(L2);
	lua_import_table(L2, pExportedTable);
	lua_setglobal(L2, "testTable");
	luaL_dofile(L2, "test2.lua");
	
	lua_close(L2);
}
