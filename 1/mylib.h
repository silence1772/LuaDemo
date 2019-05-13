#include <iostream>
#include <lua.hpp>

// for debug
void PrintLuaStack(lua_State *L)  
{  
    int stackTop=lua_gettop(L);//获取栈顶的索引值  
    int nIdx = 0;  
    int nType;  
  
    printf("--栈顶(v)(%d)--\n", stackTop);  
  
    //显示栈中的元素  
    for(nIdx = stackTop;nIdx > 0;--nIdx)  
    {  
        nType = lua_type(L, nIdx);  
        printf("(i:%d) %s(%s)\n",nIdx, lua_typename(L,nType), lua_tostring(L,nIdx));  
    }  
  
    printf("--栈底--\n");  
}  



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

    luaL_getmetatable(L, "UserClass");
    lua_setmetatable(L, -2);

    return 1;
}

int SetUserName(lua_State* L)
{
    User **u = (User**)luaL_checkudata(L, 1, "UserClass");
    luaL_argcheck(L, u != NULL, 1, "invalid user data");
    
    luaL_checktype(L, -1, LUA_TSTRING);
    
    std::string name = lua_tostring(L, -1);
    (*u)->set_name(name);
    return 0;
}

int GetUserName(lua_State* L)
{
    User **u = (User**)luaL_checkudata(L, 1, "UserClass");
    luaL_argcheck(L, u != NULL, 1, "invalid user data");
    lua_settop(L, 0);

    lua_pushstring(L, (*u)->get_name().c_str());
    return 1;
}

int SetUserAge(lua_State* L)
{
    User **u = (User**)luaL_checkudata(L, 1, "UserClass");
    luaL_argcheck(L, u != NULL, 1, "invalid user data");
    
    luaL_checktype(L, -1, LUA_TNUMBER);
    
    int age = lua_tonumber(L, -1);
    (*u)->set_age(age);
    return 0;
}

int GetUserAge(lua_State* L)
{
    User **u = (User**)luaL_checkudata(L, 1, "UserClass");
    luaL_argcheck(L, u != NULL, 1, "invalid user data");
    lua_settop(L, 0);

    lua_pushnumber(L, (*u)->get_age());
    return 1;
}

int PrintUser(lua_State* L)
{
    User **u = (User**)luaL_checkudata(L, 1, "UserClass");
    luaL_argcheck(L, u != NULL, 1, "invalid user data");
    (*u)->Print();
    return 0;
}

int DeleteUser(lua_State *L)
{
    User **u = (User**)luaL_checkudata(L, 1, "UserClass");
    luaL_argcheck(L, u != NULL, 1, "invalid user data");

    if(u)
    {
        delete *u;
    }
    
    return 0;
}

int Index(lua_State *L)
{
    //PrintLuaStack(L);

    User **u = (User**)luaL_checkudata(L, 1, "UserClass");
    luaL_argcheck(L, u != NULL, 1, "invalid user data");
    
    std::string call_val = lua_tostring(L, 2);

    if (call_val == "name_")
    {
        lua_pushstring(L, (*u)->get_name().c_str());
    }
    else if (call_val == "age_")
    {
        lua_pushnumber(L, (*u)->get_age());
    }
    else if (call_val == "printUser")
    {
        lua_pushcfunction(L, PrintUser);
    }
    else
    {
        return 0;
    }

    return 1;
}

int NewIndex(lua_State *L)
{
    User **u = (User**)luaL_checkudata(L, 1, "UserClass");
    luaL_argcheck(L, u != NULL, 1, "invalid user data");
    
    std::string call_val = lua_tostring(L, 2);

    if (call_val == "name_")
    {
        (*u)->set_name(lua_tostring(L, 3));
    }
    else if (call_val == "age_")
    {
        (*u)->set_age(static_cast<int>(lua_tonumber(L, 3)));
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

    {NULL, NULL}
};

const struct luaL_Reg userlib[] = 
{
    // {"setUserName",SetUserName},
    // {"setUserAge", SetUserAge},
    // {"getUserName",GetUserName},
    // {"getUserAge", GetUserAge},
    // {"printUser", PrintUser},
    {"__gc", DeleteUser},
    {"__index", Index},
    {"__newindex", NewIndex},

    {NULL, NULL}
};

extern "C" int luaopen_mylib(lua_State *L)
{
    // create a metatable 'UserClass'
    luaL_newmetatable(L, "UserClass");

    // set 'UserClass' as itself's __index 
    //lua_pushvalue(L, -1);
    //lua_setfield(L, -2, "__index");

    // set 'UserClass' functions
    luaL_setfuncs(L, userlib, 0);

    luaL_newlib(L, mylib);
    return 1;
}