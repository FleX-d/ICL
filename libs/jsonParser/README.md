 
# JsonParser

Library for work with Json data and to encode/decode a file to Base64

## Example Usecase

JsonObj - Facade for serialization and de-serialization Json data to object base on Nlohmann library from https//:github.com/nlohmann/json
Base64 - Utility function to decode and encode file to Base64, put Base64 to JsonObj

## Usage JsonObj

Supported types are std::string, int, double, bool, JsonObj.

### Put data to JsonObj

    flexd::icl::JsonObj jsonObj;
    jsonObj.add<bool>("/Boolean", false);
    jsonObj.add<int>("/Integer", 1);
    jsonObj.add<std::string>("/String" , "ss");
    
### Get data from JsonObj 
    
    flexd::icl::JsonObj jsonObj("{\"Boolean\": true, \"Integer\": 1, \"String\": \"ss\"}");
    bool tempbool = false;
    int tempint = 0;
    std::string tempstring = "";
    jsonObj.get<bool>("/Boolean", tempbool);
    jsonObj.get<int>("/Integer", tempint);
    jsonObj.get<std::string>("/String" , tempstring);
    
### Set data to JsonObj 

    flexd::icl::JsonObj jsonObj("{\"Boolean\": true, \"Integer\": 1, \"String\": \"ss\"}");
    jsonObj.set<bool>("/Boolean", false);
    jsonObj.set<int>("/Integer", 0);
    jsonObj.set<std::string>("/String" , "bb");
    
### Remove data from JsonObj 

    flexd::icl::JsonObj jsonObj("{\"Boolean\": true, \"Integer\": 1, \"String\": \"ss\"}");
    jsonObj.remove("/Boolean");
    
### Dump data from JsonObj
    
    flexd::icl::JsonObj jsonObj("{\"Boolean\": true, \"Integer\": 1, \"String\": \"ss\"}");
    jsonObj.dump();
    
## Usage Base64    

    base::BinStream b("../test.txt");     //Create BinStream object with path to file
    std::string str = b.getBase64();      //Get encoded Base64 string
    flexd::icl::JsonObj jsonObj = {};     //Create empty JsonObj
    jsonObj = b.getJson("test.txt");      //Get encoded Base64 string in JsonObj
    jsonObj.dump();
    
    
