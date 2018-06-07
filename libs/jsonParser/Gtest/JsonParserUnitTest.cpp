/*
Copyright (c) 2017, Globallogic s.r.o.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
 * Neither the name of the Globallogic s.r.o. nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC S.R.O. BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* 
 * File:   JsonObj.h
 * 
 * Author: Matus Bodorik
 *
 * Created on March 12, 2018, 10:02 AM
 */

#include "JsonObj.h" 
#include <gtest/gtest.h>
namespace {

    flexd::icl::JsonObj o("{\"happy\": true, \"pi\": 3.14, \"a\":{\"b\":5}, \"test\": \"/lalal/aafd\"}");
    flexd::icl::JsonObj o2("{\"Name\": \"John\" , \"age\": 30, \"car\": false }");
    flexd::icl::JsonObj o3("{\"AppName\": \"Name\" , \"AppID\": 3545, \"Download\": false }");
    flexd::icl::JsonObj o4("{\"uint8\": 5 , \"uint32\": 3545, \"uint64\": 7855488 }");
    int tempINT = 0;
    double tempDouble = 0.0;
    std::string tempStr = "";
    bool tempBool = true;
    flexd::icl::JsonObj tempJO = {};

    TEST(JsonParser, Positive_Response_Funcion_Exist)
    {
        EXPECT_TRUE(o.exist("/happy"));
        EXPECT_TRUE(o.exist("/pi"));
        EXPECT_TRUE(o.exist("/a/b"));
    }

    TEST(JsonParser, Negative_Response_Funcion_Exist)
    {
        EXPECT_FALSE(o.exist("/happ"));
        EXPECT_FALSE(o.exist("pi"));
        EXPECT_FALSE(o.exist("/a/b/"));
        EXPECT_FALSE(o.exist("a/b/"));
    }

    TEST(JsonParser, Positive_Response_Funcion_Get_Integer)
    {
        EXPECT_TRUE(o.get<int>("/a/b", tempINT));
        EXPECT_EQ(5, tempINT);
    }
    
    TEST(JsonParser, Positive_Response_Funcion_Get_Uint)
    {
        uint8_t tempUint8 = 0;
        uint32_t tempUint32 = 0;
        uint64_t tempUint64 = 0;
        EXPECT_TRUE(o4.get<uint8_t>("/uint8", tempUint8));
        EXPECT_TRUE(o4.get<uint32_t>("/uint32", tempUint32));
        EXPECT_TRUE(o4.get<uint64_t>("/uint64", tempUint64));
        EXPECT_EQ(5, tempUint8);
        EXPECT_EQ(3545, tempUint32);
        EXPECT_EQ(7855488, tempUint64);
    }
    
    TEST(JsonParser, Negative_Response_Funcion_Get_Integer)
    {
        EXPECT_FALSE(o.get<int>("/a/b/", tempINT));
        EXPECT_FALSE(o.get<int>("a/b/", tempINT));
        EXPECT_FALSE(o.get<int>("a/b", tempINT));
        EXPECT_FALSE(o.get<int>("/a/bb", tempINT));
    }

    TEST(JsonParser, Positive_Response_Funcion_Get_Double)
    {
        EXPECT_TRUE(o.get<double>("/pi", tempDouble));
        EXPECT_EQ(3.14, tempDouble);
    }

    TEST(JsonParser, Negative_Response_Funcion_Get_Double)
    {
        EXPECT_FALSE(o.get<double>("/pi/", tempDouble));
        EXPECT_FALSE(o.get<double>("pi/", tempDouble));
        EXPECT_FALSE(o.get<double>("pi", tempDouble));
        EXPECT_FALSE(o.get<double>("/pii", tempDouble));
    }

    TEST(JsonParser, Positive_Response_Funcion_Get_String)
    {
        EXPECT_TRUE(o.get<std::string>("/test", tempStr));
        std::string temp = "/lalal/aafd";
        EXPECT_STREQ(temp.c_str(), tempStr.c_str());
    }

    TEST(JsonParser, Negative_Response_Funcion_Get_String)
    {
        EXPECT_FALSE(o.get<std::string>("/test/", tempStr));
        EXPECT_FALSE(o.get<std::string>("test/", tempStr));
        EXPECT_FALSE(o.get<std::string>("test", tempStr));
        EXPECT_FALSE(o.get<std::string>("/testtt", tempStr));
    }

    TEST(JsonParser, Positive_Response_Funcion_Get_Boolean)
    {
        EXPECT_TRUE(o.get<bool>("/happy", tempBool));
        EXPECT_TRUE(tempBool);
    }

    TEST(JsonParser, Negative_Response_Funcion_Get_Boolean)
    {
        EXPECT_FALSE(o.get<bool>("/happy/", tempBool));
        EXPECT_FALSE(o.get<bool>("happy/", tempBool));
        EXPECT_FALSE(o.get<bool>("happy", tempBool));
        EXPECT_FALSE(o.get<bool>("/happyy", tempBool));
    }

    TEST(JsonParser, Positive_Response_Funcion_Get_JsonObj)
    {
        EXPECT_TRUE(o.get<flexd::icl::JsonObj>("/a", tempJO));
        int tempi = 0;
        EXPECT_TRUE(tempJO.get<int>("/b", tempi));
        EXPECT_EQ(tempi, 5);  
    }

    TEST(JsonParser, Negative_Response_Funcion_Get_JsonObj)
    {
        EXPECT_FALSE(o.get<flexd::icl::JsonObj>("/a/b", tempJO));
        EXPECT_FALSE(o.get<flexd::icl::JsonObj>("/a/bb", tempJO));
        EXPECT_FALSE(o.get<flexd::icl::JsonObj>("/a/b/", tempJO));
        EXPECT_FALSE(o.get<flexd::icl::JsonObj>("a/b", tempJO));
    }

    TEST(JsonParser, Positive_Response_Funcion_Add_Integer)
    {
        EXPECT_TRUE(o.add<int>("/integer", 1));
        o.get<int>("/integer", tempINT);
        EXPECT_EQ(1, tempINT);
    }
    
    TEST(JsonParser, Positive_Response_Funcion_Add_Uint)
    {
        uint8_t temp = 0;
        EXPECT_TRUE(o.add<uint8_t>("/uint8", 1));
        o.get<uint8_t>("/uint8", temp);
        EXPECT_EQ(1, temp);
        
        uint64_t temp1 = 0;
        EXPECT_TRUE(o.add<uint64_t>("/uint64", 155555555));
        o.get<uint64_t>("/uint64", temp1);
        EXPECT_EQ(155555555, temp1);
    }

    TEST(JsonParser, Negative_Response_Funcion_Add_Integer)
    {
        EXPECT_FALSE(o.add<int>("/integer", 1555)); //already exist
        EXPECT_FALSE(o.add<int>("integer", 1555)); //bad path
        o.get<int>("/integer", tempINT);
        EXPECT_EQ(1, tempINT);
    }

    TEST(JsonParser, Positive_Response_Funcion_Add_Double)
    {
        EXPECT_TRUE(o.add<double>("/double", 1.5));
        o.get<double>("/double", tempDouble);
        EXPECT_EQ(1.5, tempDouble);
    }

    TEST(JsonParser, Negative_Response_Funcion_Add_Double)
    {
        EXPECT_FALSE(o.add<double>("/double", 15.5)); //already exist
        EXPECT_FALSE(o.add<double>("double", 15.5)); //bad path
        o.get<double>("/double", tempDouble);
        EXPECT_EQ(1.5, tempDouble);
    }

    TEST(JsonParser, Positive_Response_Funcion_Add_String)
    {
        EXPECT_TRUE(o.add<std::string>("/string", "ssss"));
        o.get<std::string>("/string", tempStr);
        std::string temp = "ssss";
        EXPECT_STREQ(temp.c_str(), tempStr.c_str());
    }

    TEST(JsonParser, Negative_Response_Funcion_Add_String)
    {
        EXPECT_FALSE(o.add<std::string>("/string", "aaaa")); //already exist
        EXPECT_FALSE(o.add<std::string>("string", "aaaa")); //bad path
        o.get<std::string>("/string", tempStr);
        std::string temp = "ssss";
        EXPECT_STREQ(temp.c_str(), tempStr.c_str());
    }

    TEST(JsonParser, Positive_Response_Funcion_Add_Boolean)
    {
        EXPECT_TRUE(o.add<bool>("/booleean", false));
        o.get<bool>("/booleean", tempBool);
        EXPECT_FALSE(tempBool);
    }

    TEST(JsonParser, Negative_Response_Funcion_Add_Boolean)
    {
        EXPECT_FALSE(o.add<bool>("/booleean", true)); //already exist
        EXPECT_FALSE(o.add<bool>("booleean", true)); //bad path
        o.get<bool>("/booleean", tempBool);
        EXPECT_FALSE(tempBool);
    }

    TEST(JsonParser, Positive_Response_Funcion_Add_JsonObj)
    {
        EXPECT_TRUE(o.add<flexd::icl::JsonObj>("/jsonObj", o2));
        EXPECT_TRUE(o.get<flexd::icl::JsonObj>("/jsonObj", tempJO));
        int tempi = 0;
        std::string temps = "";
        bool tempb = true;
        EXPECT_TRUE(tempJO.get<std::string>("/Name", temps));
        EXPECT_STREQ(temps.c_str(), "John");
        EXPECT_TRUE(tempJO.get<int>("/age", tempi));
        EXPECT_EQ(tempi, 30);
        EXPECT_TRUE(tempJO.get<bool>("/car", tempb));
        EXPECT_FALSE(tempb);
    }

    TEST(JsonParser, Negative_Response_Funcion_Add_JsonObj)
    {
        EXPECT_FALSE(o.add<flexd::icl::JsonObj>("/jsonObj", o2)); //already exist
        EXPECT_FALSE(o.add<flexd::icl::JsonObj>("jsonObj", o2)); //bad path
    }

    TEST(JsonParser, Positive_Response_Funcion_Set_Integer)
    {
        EXPECT_TRUE(o.set<int>("/integer", 999));
        o.get<int>("/integer", tempINT);
        EXPECT_EQ(999, tempINT);
    }
    
    TEST(JsonParser, Positive_Response_Funcion_Set_Uint)
    {
        uint8_t temp = 0;
        EXPECT_TRUE(o.set<uint8_t>("/uint8", 9));
        o.get<uint8_t>("/uint8", temp);
        EXPECT_EQ(9, temp);
        
        uint64_t temp1 = 0;
        EXPECT_TRUE(o.set<uint64_t>("/uint64", 99999999));
        o.get<uint64_t>("/uint64", temp1);
        EXPECT_EQ(99999999, temp1);
    }

    TEST(JsonParser, Negative_Response_Funcion_Set_Integer)
    {
        EXPECT_FALSE(o.set<int>("integer/", 999)); //bad path
        EXPECT_FALSE(o.set<int>("/int", 999)); //not exist
    }

    TEST(JsonParser, Positive_Response_Funcion_Set_Double)
    {
        EXPECT_TRUE(o.set<double>("/double" , 2.2));
        o.get<double>("/double" , tempDouble);
        EXPECT_EQ(2.2, tempDouble);
    }
    
    TEST(JsonParser, Negative_Response_Funcion_Set_Double)
    {
        EXPECT_FALSE(o.set<double>("double/", 3.3)); //bad path
        EXPECT_FALSE(o.set<double>("/float", 3.3)); //not exist
    }
    
    TEST(JsonParser, Positive_Response_Funcion_Set_String)
    {
        EXPECT_TRUE(o.set<std::string>("/string" , "llll"));
        o.get<std::string>("/string" , tempStr);
        std::string temp = "llll";
        EXPECT_STREQ(temp.c_str(), tempStr.c_str());
    }
    
    TEST(JsonParser, Negative_Response_Funcion_Set_String)
    {
        EXPECT_FALSE(o.set<std::string>("string/", "kkkk")); //bad path
        EXPECT_FALSE(o.set<std::string>("/str", "kkkk")); //not exist
    }
    
    TEST(JsonParser, Positive_Response_Funcion_Set_Boolean)
    {
        EXPECT_TRUE(o.set<bool>("/booleean" , true));
        o.get<bool>("/booleean" , tempBool);
        EXPECT_TRUE(tempBool);
    }
     
    TEST(JsonParser, Negative_Response_Funcion_Set_Boolean)
    {
        EXPECT_FALSE(o.set<bool>("booleean/", false)); //bad path
        EXPECT_FALSE(o.set<bool>("/bool", false)); //not exist
    }
    
    TEST(JsonParser, Positive_Response_Funcion_Set_JsonObj)
    {
        EXPECT_TRUE(o.set<flexd::icl::JsonObj>("/jsonObj" , o3));
        EXPECT_TRUE(o.get<flexd::icl::JsonObj>("/jsonObj" , tempJO));
        int tempi = 0;
        std::string temps = "";
        bool tempb = true;
        EXPECT_TRUE(tempJO.get<std::string>("/AppName", temps));
        EXPECT_STREQ(temps.c_str(), "Name");
        EXPECT_TRUE(tempJO.get<int>("/AppID", tempi));
        EXPECT_EQ(tempi, 3545);
        EXPECT_TRUE(tempJO.get<bool>("/Download", tempb));
        EXPECT_FALSE(tempb); 
    }
    
    TEST(JsonParser, Negative_Response_Funcion_Set_JsonObj)
    {
        EXPECT_FALSE(o.set<flexd::icl::JsonObj>("jsonObj/" , o3));//bad path
        EXPECT_FALSE(o.set<flexd::icl::JsonObj>("/json" , o3));//not exist
    }
    
    TEST(JsonParser, Negative_Response_Funcion_Remove_Element)
    {
        EXPECT_FALSE(o.remove("/test/")); //bad path
        EXPECT_FALSE(o.remove("test/")); //bad path
        EXPECT_FALSE(o.remove("/testt")); // not exist
        EXPECT_TRUE(o.get<std::string>("/test" , tempStr));
    }
    
    TEST(JsonParser, Positive_Response_Funcion_Remove_Element)
    {
        EXPECT_TRUE(o.remove("/test"));
        EXPECT_FALSE(o.get<std::string>("/test" , tempStr));
    }
    
    TEST(JsonParser, Positive_Response_Funcion_Merge_Objects)
    {
        EXPECT_TRUE(o2.merge(o3));
    }

    TEST(JsonParser, Negative_Response_Funcion_Merge_Objects)
    {
        flexd::icl::JsonObj temp = {};
        EXPECT_FALSE(o2.merge(temp));
    }
    
    TEST(JsonParser, Positive_Response_Funcion_Dump_Objects)
    {
        o.dump();
        o2.dump();
        o3.dump();
    }
   
    TEST(JsonParser, Positive_Response_Funcion_GetJson_)
    {
        flexd::icl::JsonObj temp2("{\"happy\": true, \"pi\": 3.14, \"test\": \"/lalal/aafd\"}");
        std::string str = temp2.getJson();
        EXPECT_EQ(str, "{\"happy\":true,\"pi\":3.14,\"test\":\"/lalal/aafd\"}");
    }
}