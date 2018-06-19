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
 * File:   Base64.cpp
 * 
 * Author: Matus Bodorik
 * 
 * Created on December 11, 2017, 10:02 AM
 */


#include "Base64.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <sys/stat.h>
#include <sstream>



namespace base
{
    static const int MaxSizeFile = 100000;

    static const char* B64chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    static const int B64index [256] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 63, 62, 62, 63, 52, 53, 54, 55,
        56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6,
        7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0,
        0, 0, 0, 63, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};

    BinStream::BinStream()
    : m_dataSize(0),
      m_dataBase64("")
    {    
    }
    
    BinStream::BinStream(const std::string& filePath)
    : m_dataSize(0),
      m_dataBase64("")
    {

        if (getFileSize(filePath) != -1 && getFileSize(filePath) <= MaxSizeFile)
        {
            std::ifstream s(filePath.c_str(), std::ios::binary | std::ios::ate);

            if (s)
            {
                s.seekg(0, s.end);
                m_dataSize = s.tellg();
                s.seekg(0, s.beg);
                std::stringstream fileData;
                fileData << s.rdbuf();
                m_dataBase64 = b64Encode((void*) (fileData.str().c_str()), m_dataSize);
            }
            s.close();
        }
    }

    BinStream::BinStream(const void* data, const size_t len)
    : m_dataSize(0),
      m_dataBase64("")
    {
        if (len <= MaxSizeFile)
        {
            m_dataSize = len;
            m_dataBase64 = b64Encode(data, len);
        }
    }

    BinStream::BinStream(const std::ifstream&& stream)
    : m_dataSize(0),
      m_dataBase64("")
    {
        if (stream)
        { 
            std::stringstream fileData;
            fileData << stream.rdbuf();
            fileData.seekg(0, fileData.end);
            m_dataSize = fileData.tellg();
            fileData.seekg(0, fileData.beg);
            if (m_dataSize <= MaxSizeFile)
            {
                m_dataBase64 = b64Encode((void*) (fileData.str().c_str()), m_dataSize);
            }
        }
    }
    
    BinStream::BinStream(const flexd::icl::JsonObj& jsonObj)
    : m_dataSize(0),
      m_dataBase64("")
    {
        jsonObj.get<std::string>("/File/Data", m_dataBase64);
        int temp;
        jsonObj.get<int>("/File/FileSize", temp);
        m_dataSize = temp;
    }
    

    bool BinStream::set(const std::string& filePath)
    {
        if (getFileSize(filePath) != -1 && getFileSize(filePath) <= MaxSizeFile)
        {
            std::ifstream s(filePath.c_str(), std::ios::binary | std::ios::ate);
            if (s)
            {
                s.seekg(0, s.end);
                m_dataSize = s.tellg();
                s.seekg(0, s.beg);
                std::stringstream fileData;
                fileData << s.rdbuf();
                m_dataBase64.clear();
                m_dataBase64 = b64Encode((void*) (fileData.str().c_str()), m_dataSize);
            }
            s.close();
            return true;
        }
        return false;
    }

    bool BinStream::set(const void* data, const size_t len)
    {
        if (len <= MaxSizeFile)
        {
            m_dataBase64.clear();
            m_dataSize = len;
            m_dataBase64 = b64Encode(data, len);
            return true;
        }
        return false;
    }


    const std::string& BinStream::getBase64() const 
    {
        return m_dataBase64;
    }

    size_t BinStream::getSizeOfBase64() const 
    {
        return m_dataSize;
    }

    bool BinStream::write(const std::string& filePath) 
    {
        std::ofstream file(filePath.c_str(), std::ios::out | std::ios::binary);
        if(file){
            file << b64Decode(m_dataBase64);
            //file.write((char*) &content, sizeof (content));
            file.close();
            return true;
        }
        return false;
    }

    flexd::icl::JsonObj BinStream::getJson(const std::string& FileName) const
    {
        flexd::icl::JsonObj j = {};
        
        bool temp = j.add<std::string>("/File/Name", FileName);
        bool temp1 = j.add<int>("/File/FileSize",getSizeOfBase64());
        bool temp2 = j.add<std::string>("/File/Data", getBase64());
        if(temp && temp1 && temp2)
        {
           return std::move(j);
        }
        return flexd::icl::JsonObj();
            
    }

    bool BinStream::setBase64(const std::string& stringBase64)
    {
        m_dataBase64 = stringBase64;
        m_dataSize = m_dataBase64.size();
        if(m_dataBase64 == stringBase64)
        {
            return true;
        }
        return false;
    }

//    size_t BinStream::get(void* data) const // naplni void* decodovanymi datmi z BASE64 a vrati velkost dat ako size_t
//    {
//            
//        //std::string str = b64Decode(m_dataBase64);
//        data = new char[m_dataSize];
//        std::cop
//        return m_dataSize;
//    }


    long BinStream::getFileSize(std::string filename)
    {
        struct stat stat_buf;
        int rc = stat(filename.c_str(), &stat_buf);
        return rc == 0 ? stat_buf.st_size : -1;
    }

    std::string BinStream::b64Encode(const void* data, const size_t len)
    {
        unsigned char* p = (unsigned char*) data;
        size_t d = len % 3;
        std::string str64(4 * (int(d > 0) + len / 3), '=');

        for (size_t i = 0, j = 0; i < len - d; i += 3)
        {
            int n = int(p[i]) << 16 | int(p[i + 1]) << 8 | p[i + 2];
            str64[j++] = B64chars[n >> 18];
            str64[j++] = B64chars[n >> 12 & 0x3F];
            str64[j++] = B64chars[n >> 6 & 0x3F];
            str64[j++] = B64chars[n & 0x3F];
        }
        if (d--) /// padding
        {
            int n = d ? int(p[len - 2]) << 8 | p[len - 1] : p[len - 1];
            str64[str64.size() - 2] = d ? B64chars[(n & 0xF) << 2] : '=';
            str64[str64.size() - 3] = d ? B64chars[n >> 4 & 0x03F] : B64chars[(n & 3) << 4];
            str64[str64.size() - 4] = d ? B64chars[n >> 10] : B64chars[n >> 2];
        }
        return str64;
    }

    std::string BinStream::b64Decode(const void* data, const size_t len)
    {
        unsigned char* p = (unsigned char*) data;
        int pad = len > 0 && (len % 4 || p[len - 1] == '=');
        const size_t L = ((len + 3) / 4 - pad) * 4;
        std::string str(L / 4 * 3 + pad, '\0');

        for (size_t i = 0, j = 0; i < L; i += 4)
        {
            int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
            str[j++] = n >> 16;
            str[j++] = n >> 8 & 0xFF;
            str[j++] = n & 0xFF;
        }
        if (pad)
        {
            int n = B64index[p[L]] << 18 | B64index[p[L + 1]] << 12;
            str[str.size() - 1] = n >> 16;

            if (len > L + 2 && p[L + 2] != '=')
            {
                n |= B64index[p[L + 2]] << 6;
                str.push_back(n >> 8 & 0xFF);
            }
        }
        return str;
    }

    std::string BinStream::b64Decode(const std::string& str64)
    {
        return b64Decode(str64.c_str(), str64.size());
    }

    std::string BinStream::b64Encode(const std::string& str64)
    {
        return b64Encode(str64.c_str(), str64.size());
    }

}
