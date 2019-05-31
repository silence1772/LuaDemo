#include "lua.hpp"
#include <iostream>
#include <string>


int main() {
	//��Lua�����L1�е���testTable
	lua_State* L1 = luaL_newstate();
	luaL_openlibs(L1);
	luaL_dofile(L1, "LuaCode/test.lua");
	void* pExportedTable = lua_export_table(L1, "testTable");
	lua_close(L1);

	//��Lua�����L1�е�����testTable����Lua�����L2��
	lua_State* L2 = luaL_newstate();
	luaL_openlibs(L2);
	lua_import_table(L2, pExportedTable);
	lua_setglobal(L2, "testTable");
	luaL_dofile(L2, "LuaCode/test2.lua");
	lua_close(L2);

	system("pause");
}
