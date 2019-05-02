luaGlobalBoolean = true
luaGlobalNumber = 10
luaGlobalString = "string"
luaGlobalTable = {key1 = "aa", key2 = "bb", key3 = "cc"}

function addInLua(a, b)
    local result = a + b
    return result
end

function addInCpp(a, b)
    local result = mylib.addInCPP(a, b)
    return result
end

function accessVariableInCpp()
    local i = mylib.getCppGlobalInteger()
    mylib.setCppGlobalInteger(i, 99)
    local a = mylib.getCppGlobalIntArray()
    mylib.setCppGlobalIntArray(a)
    local s = mylib.getCppGlobalString()
    mylib.setCppGlobalString(s)
end

function accessClassInCpp(name, age)
    local user = mylib.newUser()
    mylib.setUserName(user, name)
    mylib.setUserAge(user, age)
    mylib.printUser(user)
    mylib.deleteUser(user)
end