#include <iostream>
#include <lua.hpp>



// for lua call c++ function
int AddInCPP(lua_State *L)
{
    double a = lua_tonumber(L, 1);
    double b = lua_tonumber(L, 2);
    lua_pushnumber(L, a + b);
    return 1;
}



// for lua read c++ variable
int cpp_global_integer = 66;
int cpp_global_int_array[5] = {1, 2, 3, 4, 5};
std::string cpp_global_string = "cpp global string";

int GetCppGlobalInteger(lua_State *L)
{
    int **i = (int**)lua_newuserdata(L, sizeof(int*));
    *i = &cpp_global_integer;
    return 1;
}

int SetCppGlobalInteger(lua_State *L)
{
    int **i = (int**)lua_touserdata(L, 1);
    luaL_argcheck(L, i != NULL, 1, "invalid user data");
    std::cout << "cpp_global_integer = " << **i;
    
    luaL_checktype(L, -1, LUA_TNUMBER);
    
    int value = lua_tonumber(L, -1);
    **i = value;
    std::cout << " ==> " << **i << std::endl;
    return 0;
}

int GetCppGlobalIntArray(lua_State *L)
{
    int (**i)[5] = (int (**)[5])lua_newuserdata(L, sizeof(int (*)[5]));
    *i = &cpp_global_int_array;
    return 1;
}

int SetCppGlobalIntArray(lua_State *L)
{
    int (**i)[5] = (int (**)[5])lua_touserdata(L, 1);
    luaL_argcheck(L, i != NULL, 1, "invalid user data");

    std::cout << "cpp_global_int_array = { ";
    int len = sizeof(**i) / sizeof((**i)[0]);
    for (int j = 0; j < len; ++j)
        std::cout << (**i)[j] << " ";

    std::cout << "}" << " ==> { ";
    for (int j = 0; j < len; ++j)
    {
        (**i)[j] = j;
        std::cout << (**i)[j] << " ";
    }
    std::cout << "}" << std::endl;
    return 0;
}

int GetCppGlobalString(lua_State *L)
{
    std::string **i = (std::string **)lua_newuserdata(L, sizeof(std::string*));
    *i = &cpp_global_string;
    return 1;
}

int SetCppGlobalString(lua_State *L)
{
    std::string **i = (std::string **)lua_touserdata(L, 1);
    luaL_argcheck(L, i != NULL, 1, "invalid user data");

    std::cout << "cpp_global_string = \"" << **i << "\" ==> ";
    **i = "cpp_global_string";
    std::cout << "\"" << **i << "\"" << std::endl;
    return 0;
}



// for lua access c++ class
int NewUser(lua_State* L)
{
    User **u = (User**)lua_newuserdata(L, sizeof(User*));
    *u = new User();
    return 1;
}

int SetUserName(lua_State* L)
{
    User **u = (User**)lua_touserdata(L, 1);
    luaL_argcheck(L, u != NULL, 1, "invalid user data");
    
    luaL_checktype(L, -1, LUA_TSTRING);
    
    std::string name = lua_tostring(L, -1);
    (*u)->set_name(name);
    return 0;
}

int GetUserName(lua_State* L)
{
    User **u = (User**)lua_touserdata(L, 1);
    luaL_argcheck(L, u != NULL, 1, "invalid user data");
    lua_settop(L, 0);
    lua_pushstring(L, (*u)->get_name().c_str());
    return 1;
}

int SetUserAge(lua_State* L)
{
    User **u = (User**)lua_touserdata(L, 1);
    luaL_argcheck(L, u != NULL, 1, "invalid user data");
    
    luaL_checktype(L, -1, LUA_TNUMBER);
    
    int age = lua_tonumber(L, -1);
    (*u)->set_age(age);
    return 0;
}

int GetUserAge(lua_State* L)
{
    User **u = (User**)lua_touserdata(L, 1);
    luaL_argcheck(L, u != NULL, 1, "invalid user data");
    lua_settop(L, 0);
    lua_pushnumber(L, (*u)->get_age());
    return 1;
}

int PrintUser(lua_State* L)
{
    User **u = (User**)lua_touserdata(L, 1);
    luaL_argcheck(L, u != NULL, 1, "invalid user data");
    (*u)->Print();
    return 0;
}

int DeleteUser(lua_State *L)
{
    User **u = (User**)lua_touserdata(L, 1);
    if(u)
    {
        delete *u;
    }
    
    return 0;
}


const struct luaL_Reg mylib[] = 
{
    {"addInCPP", AddInCPP},

    {"getCppGlobalInteger", GetCppGlobalInteger},
    {"setCppGlobalInteger", SetCppGlobalInteger},
    {"getCppGlobalIntArray", GetCppGlobalIntArray},
    {"setCppGlobalIntArray", SetCppGlobalIntArray},
    {"getCppGlobalString", GetCppGlobalString},
    {"setCppGlobalString", SetCppGlobalString},

    {"newUser", NewUser},
    {"setUserName",SetUserName},
    {"setUserAge", SetUserAge},
    {"getUserName",GetUserName},
    {"getUserAge", GetUserAge},
    {"printUser", PrintUser},
    {"deleteUser", DeleteUser},

    {NULL, NULL}
};

extern "C" int luaopen_mylib(lua_State *L)
{
    luaL_newlib(L, mylib);
    return 1;
}