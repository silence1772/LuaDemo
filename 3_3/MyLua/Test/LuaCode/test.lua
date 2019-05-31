function print_r ( t )  
    local print_r_cache={}
    local function sub_print_r(t,indent)
        if (print_r_cache[tostring(t)]) then
            print(indent.."*"..tostring(t))
        else
            print_r_cache[tostring(t)]=true
            if (type(t)=="table") then
                for pos,val in pairs(t) do
                    if (type(val)=="table") then
                        print(indent.."["..pos.."] => {")
                        sub_print_r(val,indent..string.rep(" ",string.len(pos)+8))
                        print(indent..string.rep(" ",string.len(pos)+6).." }")
                    elseif (type(val)=="string") then
                        print(indent.."["..pos..'] => "'..val..'"')
                    else
                        print(indent.."["..pos.."] => "..tostring(val))
                    end
                end
            else
                print(indent..tostring(t))
            end
        end
    end
    if (type(t)=="table") then
        print("{")
        sub_print_r(t,"  ")
        print("}")
    else
        sub_print_r(t,"  ")
    end
    print()
end

outside = {
    varString = "It's a outside string";
    varString2 = "It's a outside string";
    outsideTable = {
        varLong = "hiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii";
    }
}

exportstart
testTable = {
    varInt = 9999;
    varBool = true;
    varString = "It's test string for this table";
    varString2 = "It's test string for this table";
    varSubTable = {
        varInt = 8888;
        varBool = false;
        varString = "It's test string for sub table";
    };
    varIntArray = { 10001; 10002; 10003; 10004; };
    varTableArray = { 
        {
           varInt = 10001; 
        };
        {
           varInt = 10002; 
        };
        {
           varInt = 10003; 
        };
    };
    outside = outside;
    longString = "It's a so long stringgggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg";
}
exportend

print("In Lua State L1:")
print_r(testTable)
