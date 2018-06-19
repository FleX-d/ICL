
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
#include "Base64.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


int main(int argc, char** argv)
{

   base::BinStream b("/home/dev/work/ICL/test.txt");    //Create BinStream object with path to file
   std::string str = b.getBase64();                     //Convert to base64
   flexd::icl::JsonObj o;                               //Create JsonObj
   o = b.getJson("test.txt");                           //Put base64 string to JsonObj
   o.dump();                                            //Output of JsonObj

   std::ifstream s("/home/dev/work/ICL/json.txt", std::ios::binary | std::ios::ate);  //Create new ifstream
   s.seekg(0, s.end);                                   //Check size of stream
   size_t size = s.tellg();
   s.seekg(0, s.beg);
   std::stringstream fileData;                          //Put ifstream to new stringstream
   fileData << s.rdbuf();
   s.seekg(0, s.beg);

   base::BinStream bb(fileData.str().c_str(), size);    //Create new BinStream by Stringstream and size
   flexd::icl::JsonObj oo;                              //Create new JsonObj
   oo = bb.getJson("test.txt");                         //Put base64 string to JsonObj
   oo.dump();                                           //Output of JsonObj

   base::BinStream bbb (std::move(s));                  //Create new BinStream by moving ifstream
   str = bbb.getBase64();                               //Get base64 from BinStream
   flexd::icl::JsonObj ooo = {};                        //Create new JsonObj
   ooo = bbb.getJson("test2.txt");                      //Get JsonObj with encoded Base64 string
   ooo.dump();

   base::BinStream json(o);                             //Create new BinStream by JsonObj
   std::string temp = json.getBase64();                 //Get base64 from BinStream
   std::cout << temp << std::endl;                      //Output

   b.set("/home/dev/work/ICL/json.txt");                //Set new path to file to BinStream
   str = b.getBase64();                                 //Get base64
   base::BinStream js;                                  //Create new empty BinStream
   js.setBase64(str);                                   //Set Base64 string to empty BinStream
   js.write("/home/dev/work/ICL/json1.txt");            //Decode base64 and write it to new file

   b.set(fileData.str().c_str(), size);                 //Set BinStrem by stringstream and size
   str = b.getBase64();                                 //Get base64
   std::cout << str << std::endl;                       //Output

   return 0;
}
