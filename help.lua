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

    mylib.printInCpp("name: " .. user.name_ .. "  age: " .. user.age_)

    user.name_ = "Hello"
    user.age_ = 20
    
    mylib.printInCpp("name: " .. user.name_ .. "  age: " .. user.age_)

    --user.printUser(user)
    --user:printUser()
end

