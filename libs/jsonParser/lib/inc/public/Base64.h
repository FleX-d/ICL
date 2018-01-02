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
 * File:   Base64.h
 * Author: Matus Bodorik
 * 
 * Created on December 11, 2017, 9:32 AM
 */

#ifndef BASE64_H_
#define BASE64_H_

#define BINSTREAMMAXFILESIZE 100*1024

#include "JsonObj.h"
#include <string>

namespace base {

    class BinStream {
        
    public:
        explicit BinStream(const std::string& filePath);
        explicit BinStream(const void* data, const size_t len);
        explicit BinStream(const std::ifstream& stream);
        ~BinStream() = default;
        /**
         * 
         * @param filePath
         * @return 
         */
        bool set(const std::string& filePath);
        /**
         * 
         * @param data
         * @param len
         * @return 
         */
        bool set(const void* data, const size_t len);
        /**
         * 
         * @return 
         */
        const std::string& getBase64() const; 
        /**
         * 
         * @return 
         */
        size_t getSizeOfBase64() const;
        /**
         * 
         * @param filePath
         * @return 
         */
        bool write(const std::string& filePath);
        /**
         * 
         * @param FileName
         * @return 
         */
        flexd::icl::JsonObj getJson(const std::string& FileName) const;
    
    private:
        std::string b64Encode(const void* data, const size_t len);
        std::string b64Decode(const void* data, const size_t len);
        std::string b64Decode(const std::string& str64);
        std::string b64Encode(const std::string& str64);
        long getFileSize(std::string filename);
        
    private:
        size_t m_dataSize;
        std::string m_dataBase64;
    };
}
#endif /* BASE64_H*/