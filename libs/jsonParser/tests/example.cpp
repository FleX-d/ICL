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
 * Created on Januar 8, 2018, 10:02 AM
 */

#include "JsonObj.h"
#include <cassert>
#include <string>

int main(int argc, char** argv)
{
    int tempInt = 0;
    double tempDouble = 0.0;
    bool tempBool = false;
    flexd::icl::JsonObj tempJO;
    std::string tempStr;

    flexd::icl::JsonObj o1("{\"happy\": true, \"pi\": 3.14, \"a\":{\"b\":5}, \"test\": \"/lalal/aafd\"}");
    flexd::icl::JsonObj o2("{\"Jozko\":\"Mrkvicka\"}");
    flexd::icl::JsonObj o3("{\"Name\": \"John\" , \"age\": 30, \"car\": false }");


    std::cout << "Start tests " << std::endl;

    //check exist test
    assert(o1.exist("/test1") == false);
    assert(o1.exist("/test/") == false);
    assert(o1.exist("test/") == false);
    assert(o1.exist("/tes") == false);
    assert(o1.exist("/happy") == true);

    //Get test
    assert(o1.get<int>("/a", tempInt) == false);
    assert(o1.get<int>("/a/b/", tempInt) == false);
    assert(o1.get<int>("a/b/", tempInt) == false);
    assert(o1.get<int>("/a/b", tempInt) == true);
    assert(tempInt == 5);

    assert(o1.get<double>("/pi/", tempDouble) == false);
    assert(o1.get<double>("pi", tempDouble) == false);
    assert(o1.get<double>("/pi", tempDouble) == true);
    assert(tempDouble == 3.14);

    assert(o1.get<std::string>("/test/", tempStr) == false);
    assert(o1.get<std::string>("test", tempStr) == false);
    assert(o1.get<std::string>("/test", tempStr) == true);
    assert(tempStr == "/lalal/aafd");

    assert(o1.get<bool>("/happy/", tempBool) == false);
    assert(o1.get<bool>("happy/", tempBool) == false);
    assert(o1.get<bool>("/happ", tempBool) == false);
    assert(o1.get<bool>("/happy", tempBool) == true);
    assert(tempBool == true);

    assert(o1.get<flexd::icl::JsonObj>("/a/b", tempJO) == false);
    assert(o1.get<flexd::icl::JsonObj>("/a", tempJO) == true);

    //Add test
    assert(o1.add<bool>("/booleean/" , false) == false); // /booleean/ not exist
    assert(o1.add<bool>("booleean/" , false) == false); //not exist
    assert(o1.add<bool>("/happy" , false) == false);   //happy exist
    assert(o1.add<bool>("/booleean" , false) == true); //not exist
    o1.get<bool>("/booleean" , tempBool);
    assert(tempBool == false);


    assert(o1.add<int>("/integer/" , 1555) == false); //not exist
    assert(o1.add<int>("integer/" , 1555) == false); //not exist
    assert(o1.add<int>("/a/b" , 1555) == false);     //exist
    assert(o1.add<int>("/integer" , 1555) == true);  //not exist
    o1.get<int>("/integer" , tempInt);
    assert(tempInt == 1555);

    assert(o1.add<double>("/double/" , 1.5) == false); //not exist
    assert(o1.add<double>("integer/" , 1.5) == false); //not exist
    assert(o1.add<double>("/pi" , 1.5) == false);      //exist
    assert(o1.add<double>("/double" , 1.5) == true);   //not exist
    o1.get<double>("/double" , tempDouble);
    assert(tempDouble == 1.5);

    assert(o1.add<std::string>("/string/" , "ssss") == false); //not exist
    assert(o1.add<std::string>("string/" , "ssss") == false);  //not exist
    assert(o1.add<std::string>("/test" , "ssss") == false);    //exist
    assert(o1.add<std::string>("/string" , "ssss") == true);   //not exist
    o1.get<std::string>("/string" , tempStr);
    assert(tempStr == "ssss");

    assert(o1.add<flexd::icl::JsonObj>("/jsonObj/" , o3) == false); //not exist
    assert(o1.add<flexd::icl::JsonObj>("jsonObj/" , o3) == false);  //not exist
    assert(o1.add<flexd::icl::JsonObj>("/a" , o3) == false);        //exist
    assert(o1.add<flexd::icl::JsonObj>("/jsonObj" , o3) == true);   //not exist
    o1.get<flexd::icl::JsonObj>("/jsonObj" , tempJO);


    //set Test
    assert(o1.set<bool>("/booleean/" , true) == false);
    assert(o1.set<bool>("booleean/" , true) == false);
    assert(o1.set<bool>("/bool" , true) == false);
    assert(o1.set<bool>("/booleean" , true) == true);
    o1.get<bool>("/booleean" , tempBool);
    assert(tempBool == true);

    assert(o1.set<int>("/integer/" , 999) == false);
    assert(o1.set<int>("integer/" , 999) == false);
    assert(o1.set<int>("/int" , 999) == false);
    assert(o1.set<int>("/integer" , 999) == true);
    o1.get<int>("/integer" , tempInt);
    assert(tempInt == 999);

    assert(o1.set<double>("/double/" , 15.99) == false);
    assert(o1.set<double>("double/" , 15.99) == false);
    assert(o1.set<double>("/dou" , 15.99) == false);
    assert(o1.set<double>("/double" , 15.99) == true);
    o1.get<double>("/double" , tempDouble);
    assert(tempDouble == 15.99);

    assert(o1.set<std::string>("/string/" , "llll") == false);
    assert(o1.set<std::string>("string/" , "llll") == false);
    assert(o1.set<std::string>("/str" , "llll") == false);
    assert(o1.set<std::string>("/string" , "llll") == true);
    o1.get<std::string>("/string" , tempStr);
    assert(tempStr == "llll");

    assert(o1.set<flexd::icl::JsonObj>("/jsonObj/" , o2) == false);
    assert(o1.set<flexd::icl::JsonObj>("jsonObj/" , o2) == false);
    assert(o1.set<flexd::icl::JsonObj>("/json" , o2) == false);
    assert(o1.set<flexd::icl::JsonObj>("/jsonObj" , o2) == true);
    o1.get<flexd::icl::JsonObj>("/jsonObj" , tempJO);


    //remove test
    assert(o1.remove("/test1") == false);
    assert(o1.remove("/test/") == false);
    assert(o1.remove("test/") == false);
    assert(o1.remove("/test") == true);
    assert(o1.get<std::string>("/test" , tempStr) == false);

    //Merge test
    assert(o2.merge(o3) == true);

    //Dump
    std::cout<< "1. Object" << std::endl;
    o1.dump();
    std::cout<< "2. Object" << std::endl;
    o2.dump();
    std::cout<< "3. Object" << std::endl;
    o3.dump();

    return 0;
}
