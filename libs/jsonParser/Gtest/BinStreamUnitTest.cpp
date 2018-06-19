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
 * Created on March 15, 2018, 10:02 AM
 */

#include <gtest/gtest.h>
#include "JsonObj.h"
#include "Base64.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#define TEST_FILE "/tmp/test.txt"
#define TEST_FILE2 "/tmp/test2.txt"

namespace {

    flexd::icl::JsonObj o = {};
    std::string base = "";

    void createTestFile()
    {
        std::ofstream file;
        file.open (TEST_FILE);
        file << "Some text...\nSome more text...\n";
        file.close();
    }

    TEST(Binstream, Default_Constructor)
    {
        base::BinStream b;
        std::string str = b.getBase64();
        EXPECT_TRUE(str.empty());
        EXPECT_STREQ("", str.c_str());
    }

    TEST(Binstream, Constructor_With_Path)
    {
        createTestFile();
        base::BinStream b(TEST_FILE);
        std::string str = b.getBase64();
        base = b.getBase64();
        EXPECT_FALSE(str.empty());
        EXPECT_STREQ(base.c_str(), str.c_str());
        o = b.getJson("test.txt");
        ::remove(TEST_FILE);
    }

    TEST(Binstream, Constructor_With_StringStream_And_Size)
    {
        createTestFile();
        std::ifstream s(TEST_FILE, std::ios::binary | std::ios::ate);
        s.seekg(0, s.end);
        size_t size = s.tellg();
        s.seekg(0, s.beg);
        std::stringstream fileData;
        fileData << s.rdbuf();
        s.seekg(0, s.beg);
        base::BinStream b(fileData.str().c_str(), size);
        std::string str = b.getBase64();
        EXPECT_FALSE(str.empty());
        EXPECT_STREQ(base.c_str(), str.c_str());
        ::remove(TEST_FILE);
    }

    TEST(Binstream, Constructor_By_Moving_Ifstream)
    {
        createTestFile();
        std::ifstream s(TEST_FILE, std::ios::binary | std::ios::ate);
        s.seekg(0, s.beg);
        base::BinStream b(std::move(s));
        std::string str = b.getBase64();
        EXPECT_FALSE(str.empty());
        EXPECT_STREQ(base.c_str(), str.c_str());
        ::remove(TEST_FILE);
    }

    TEST(Binstream, Constructor_by_JsonObj)
    {
        base::BinStream json(o);
        std::string str = json.getBase64();
        EXPECT_FALSE(str.empty());
        EXPECT_STREQ(base.c_str(), str.c_str());
    }

    TEST(Binstream, Set_Binstream_by_Path)
    {
        createTestFile();
        base::BinStream b;
        b.set(TEST_FILE);
        std::string str = b.getBase64();
        EXPECT_FALSE(str.empty());
        EXPECT_STREQ(base.c_str(), str.c_str());
        ::remove(TEST_FILE);
    }

    TEST(Binstream, Set_Binstream_by_StringStream_And_Size)
    {
        createTestFile();
        base::BinStream b;
        std::ifstream s(TEST_FILE, std::ios::binary | std::ios::ate);
        s.seekg(0, s.end);
        size_t size = s.tellg();
        s.seekg(0, s.beg);
        std::stringstream fileData;
        fileData << s.rdbuf();
        s.seekg(0, s.beg);
        b.set(fileData.str().c_str(), size);
        std::string str = b.getBase64();
        EXPECT_FALSE(str.empty());
        EXPECT_STREQ(base.c_str(), str.c_str());
        ::remove(TEST_FILE);
    }

    TEST(Binstream, Set_Binstream_By_Function_SetBase64)
    {
        base::BinStream b;
        b.setBase64(base);
        std::string str = b.getBase64();
        EXPECT_FALSE(str.empty());
        EXPECT_STREQ(base.c_str(), str.c_str());
    }

    TEST(Binstream, Get_Size_By_Function_GetSizeBase64)
    {
        base::BinStream b;
        b.setBase64(base);
        std::string str = b.getBase64();
        size_t size = b.getSizeOfBase64();
        EXPECT_FALSE(str.empty());
        EXPECT_STREQ(base.c_str(), str.c_str());
        EXPECT_EQ(size, str.size());
    }

    TEST(Binstream, Function_Write_Base64)
    {
        base::BinStream b;
        b.setBase64(base);
        b.write(TEST_FILE2);
        std::ifstream s(TEST_FILE2, std::ios::binary | std::ios::ate);
        EXPECT_TRUE(s);
        s.seekg(0, s.beg);
        base::BinStream bb(std::move(s));
        std::string str = bb.getBase64();
        EXPECT_STREQ(base.c_str(), str.c_str());
        ::remove(TEST_FILE2);
    }

    TEST(Binstream, Function_Get_JsonObj)
    {
        base::BinStream b;
        b.setBase64(base);
        flexd::icl::JsonObj o = b.getJson("test.txt");
        std::string temps = "";
        int tempi = 0;
        std::string tempss = "";
        o.get<std::string>("/File/Name", temps);
        o.get<int>("/File/FileSize", tempi);
        o.get<std::string>("/File/Data", tempss);
        EXPECT_STREQ(base.c_str(), tempss.c_str());
        EXPECT_STREQ("test.txt", temps.c_str());
        EXPECT_EQ(tempi, tempss.size());
    }
}
