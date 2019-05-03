#include "lua.hpp"
#include "user.h"
#include "mylib.h"
#include <iostream>

// c++调用lua函数
void CallFuncInLua(lua_State* L);
// c++读取lua全局变量
void ReadVariableInLua(lua_State* L);

// lua调用c++函数
void LuaCallFuncInCpp(lua_State* L);
// lua读取c++指定变量
void LuaReadVariableInCpp(lua_State* L);
// lua访问c++类对象
void LuaReadClassInCpp(lua_State* L);


int main()
{
    lua_State *L = luaL_newstate();
    luaopen_base(L);
    
    // 注册c++函数到lua环境中
    luaL_requiref(L, "mylib", luaopen_mylib, 1);
    lua_settop(L, 0);

    // 执行'help.lua'
    luaL_dofile(L, "help.lua");

    // c++调用lua
    CallFuncInLua(L);
    ReadVariableInLua(L);

    // lua调用c++
    LuaCallFuncInCpp(L);
    LuaReadVariableInCpp(L);
    LuaReadClassInCpp(L);

    lua_close(L);
}


void CallFuncInLua(lua_State* L)
{
    // call lua function 'addInLua'
    lua_getglobal(L, "addInLua");
    lua_pushnumber(L, 4);
    lua_pushnumber(L, 3);

    // without error handling
    if (lua_pcall(L, 2, 1, 0) != 0)
        exit(1);
    if (!lua_isnumber(L, -1))
        exit(1);
    double add_in_lua_res = lua_tonumber(L, -1);
    lua_settop(L, 0);

    std::cout << "C++调用lua带参函数 addInLua：" << std::endl;
    std::cout << "4 + 3 = " << std::to_string(add_in_lua_res) << std::endl;
    std::cout << std::endl << "-------------------------------------" << std::endl;
}

void ReadVariableInLua(lua_State* L)
{
    // read global variable in lua
    std::cout << "C++访问lua全局变量：" << std::endl;

    lua_getglobal(L, "luaGlobalBoolean");
    bool lua_global_boolean = lua_toboolean(L, -1);
    std::cout << "luaGlobalBoolean = " << (lua_global_boolean ? "true" : "false") << std::endl;
    lua_settop(L, 0);

    lua_getglobal(L, "luaGlobalNumber");
    int lua_global_number = lua_tonumber(L, -1);
    std::cout << "luaGlobalNumber = " << lua_global_number << std::endl;

    lua_getglobal(L, "luaGlobalString");
    std::string lua_global_string = lua_tostring(L, -1);
    std::cout << "luaGlobalString = " << lua_global_string << std::endl;

    lua_getglobal(L, "luaGlobalTable");
    lua_pushstring(L, "key1");
    lua_gettable(L, -2);
    std::string lua_global_table_key1 = lua_tostring(L, -1);
    std::cout << "luaGlobalTable.key1 = " << lua_global_table_key1 << std::endl;

    std::cout << std::endl << "-------------------------------------" << std::endl;
}

void LuaCallFuncInCpp(lua_State* L)
{
    // call function 'addInCpp'
    lua_getglobal(L, "addInCpp");
    lua_pushnumber(L, 7);
    lua_pushnumber(L, 8);

    if (lua_pcall(L, 2, 1, 0) != 0)
        exit(1);
    if (!lua_isnumber(L, -1))
        exit(1);
    double add_in_cpp_res = lua_tonumber(L, -1);
    lua_settop(L, 0);

    std::cout << "lua调用c++带参函数 addInCpp：" << std::endl;
    std::cout << "7 + 8 = " << std::to_string(add_in_cpp_res) << std::endl;
    std::cout << std::endl << "-------------------------------------" << std::endl;
}

void LuaReadVariableInCpp(lua_State* L)
{
    // read c++ variable
    std::cout << "lua访问和修改c++指定变量：" << std::endl;
    lua_getglobal(L, "accessVariableInCpp");
    if (lua_pcall(L, 0, 0, 0) != 0)
    {
        std::cout << "error running function 'accessVariableInCpp'" << std::endl;
        exit(1);
    }
    std::cout << std::endl << "-------------------------------------" << std::endl;
}

void LuaReadClassInCpp(lua_State* L)
{
    // read c++ class object
    std::cout << "lua访问c++的class对象及其成员函数：" << std::endl;
    lua_getglobal(L, "accessClassInCpp");
    lua_pushstring(L, "Tom");
    lua_pushnumber(L, 20);
    if (lua_pcall(L, 2, 0, 0) != 0)
    {
        std::cout << lua_tostring(L, -1) << std::endl;
        std::cout << "error running function 'accessClassInCpp'" << std::endl;
        exit(1);
    }
    std::cout << std::endl << "-------------------------------------" << std::endl;
}

