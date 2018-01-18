  
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


main(int argc, char** argv)
{
   //Create BinStream object with path to file
   base::BinStream b("/home/dev/work/ICL/test.txt"); 
   //Convert to base64
   std::string str = b.getBase64();
   //Create JsonObj
   flexd::icl::JsonObj o = {};
   //Put base64 string to JsonObj
   o = b.getJson("test.txt");
   o.dump();
   
   //Create ifstream 
   std::ifstream s("/home/dev/work/ICL/test.txt", std::ios::binary | std::ios::ate);
   //Check size of stream 
   s.seekg(0, s.end);
   size_t size = s.tellg();
   s.seekg(0, s.beg);
   //Put ifstream to stringstream
   std::stringstream fileData;
   fileData << s.rdbuf();
   
   //Create new BinStream by Stringstream and size
   base::BinStream bb(fileData.str().c_str(), size);
   //Create new JsonObj
   flexd::icl::JsonObj oo;
   //Put base64 string to JsonObj
   oo = b.getJson("test1.txt");
   oo.dump();
   
   //Create new BinStream by ifstream 
   base::BinStream bbb(s);
   //Convert to base64
   str = bbb.getBase64();
   //Create new JsonObj
   flexd::icl::JsonObj ooo;
   //Put base64 string to JsonObj
   ooo = b.getJson("test2.txt");
   ooo.dump();
   
   //Set new path to file to BinStream
   b.set("/home/dev/work/ICL/test.txt");
   //Get base64
   str = b.getBase64();    
   //Decode base64 and write it to new file
   b.write("/home/dev/work/ICL/test1.txt");

   //Set BinStrem by stringstream and size
   b.set(fileData.str().c_str(), size);
   str = b.getBase64();     
   std::cout << str << std::endl;
   
   return 0;
} 
