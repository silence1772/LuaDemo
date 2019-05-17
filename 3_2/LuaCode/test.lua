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
}
testTable.varString = "hello"
exportend