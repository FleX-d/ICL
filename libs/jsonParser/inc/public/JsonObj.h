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
 * Created on November 23, 2017, 10:02 AM
 */

#ifndef JSONOBJ_H
#define JSONOBJ_H


#include "ParserTypes.h"
#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <iostream> // will removed


namespace flexd {
    namespace icl {

        class JsonObj {
            class JsonParser;
            
        public:
            JsonObj();
            JsonObj(const std::string& jString);   
            JsonObj(JsonObj&& other);
            JsonObj& operator=(JsonObj&& other);
            ~JsonObj();

            /**
             * Add value type T according specific path, if the path is not exist at JsonObj.
             * Supported types are std::string, int, double, bool, JsonObj.
             * @param path is specific string format composed of keys.
             * Example of valid path: "/a/b/c".
             * @param val type T is reference to type of VALUE 
             * which will be stored to JsonObj.
             * @return true if path not exist and wal was succesfully added to JsonObj
             * and return false otherwise.
             */
            template<class T>
            ReturnType add(const std::string& path, const T& val) {
                std::cout << "Type not found!\n";
                return Error;
            }

            /**
             * Set value type T according specific path, if the path is exist at JsonObj.
             * Supported types are std::string, int, double, bool.
             * @param path is specific string format composed of keys.
             * Example of valid path: "/a/b/c".
             * @param val type T is reference to type of VALUE 
             * which will overwrite value at path in JsonObj.
             * @return true if path exist and wal was succesfully overwrite at path in JsonObj
             * and return false otherwise.
             */
            template<class T>
            ReturnType set(const std::string& path, const T& val) {
                std::cout << "Type not found!\n";
                return Error;
            }

            /**
             * Get value type T according specific path, if the path is exist at JsonObj.
             * Supported types are std::string, int, double, bool.
             * @param path is specific string format composed of keys.
             * Example of valid path: "/a/b/c".
             * @param val type T is reference to required type of VALUE 
             * where result will be stored.
             * @return true if path exist and return VALUE succesfully converted
             * to required type, false otherwise.
             */
            template<class T>
            ReturnType get(const std::string& path, T& val) const {
                std::cout << "Type not found!\n";
                return Error;
            }

            /**
             * operator + which merge contents of two JsonObjects.
             * @param other reference to JsonObj which content we want merge.
             * @return JsonObj merged with content of other JsonObj.
             */
            JsonObj operator+(const JsonObj& other);

            /**
             * operator += which merge contents of two JsonObjects.
             * @param other reference to JsonObj which content we want merge.
             * @return JsonObj merged with content of other JsonObj. 
             */
            JsonObj operator+=(const JsonObj& other);

            /**
             * Check if path is valid and object is not empty on this path.
             * @param path is specific string format composed of keys.
             * Example of valid path: "/a/b/c".
             * @return true if path is valid and object on path is not empty, 
             * false otherwise.
             */
            ReturnType exist(const std::string& path);

            /**
             * Remove value according specific path, if the path is exist at JsonObj.
             * @param path is specific string format composed of keys.
             * Example of valid path: "/a/b/c".
             * @return true if remove was succesfull, false otherwise.
             */
            ReturnType remove(const std::string& path);

            /**
             * Merge contents of two JsonObj
             * @param other reference to JsonObj which content we want merge
             * @return true if merge was succesfull, false otherwise.
             */
            ReturnType merge(const JsonObj &other);

            /**
             * Print content of JsonObj
             * @param d level of Intent 
             * '-1' will print content to one line,
             * `0` will only insert newlines, 
             * `> 0` (the default 2) selects the most compact representation.
             * @param flat (default 0) print content without paths
             * '1' print content with paths
             */
            void dump(int d = 2, int flat = 0) const; // print content of json
            /**
             * Function store instance of other object JsonParser 
             * @param other - object JsonParse to store
             */
            void storeJson(JsonParser* other);
            /**
             * Function return reference to string with json
             * @param other - reference to string with json
             */
            std::string getJson();

        private:
            ReturnType pathIsValid(const std::string& path) const;
            ReturnType pathExist(const std::string& path) const;
            std::vector<std::string> parsePath(const std::string& path) const;
            ReturnType addInt(const std::string& path, const int& val);
            ReturnType addDouble(const std::string& path, const double& val);
            ReturnType addString(const std::string& path, const std::string& val);
            ReturnType addBoolean(const std::string& path, const bool& val);
            ReturnType addJsonObj(const std::string& path, const JsonObj& val);
            ReturnType addUint8(const std::string& path, const uint8_t& val);
            ReturnType addUint16(const std::string& path, const uint16_t& val);
            ReturnType addUint32(const std::string& path, const uint32_t& val);
            ReturnType addUint64(const std::string& path, const uint64_t& val);
            ReturnType getInt(const std::string& path, int& val) const;
            ReturnType getUint8(const std::string& path, uint8_t& val) const;
            ReturnType getUint16(const std::string& path, uint16_t& val) const;
            ReturnType getUint32(const std::string& path, uint32_t& val) const;
            ReturnType getUint64(const std::string& path, uint64_t& val) const;
            ReturnType getDouble(const std::string& path, double& val) const;
            ReturnType getString(const std::string& path, std::string& val)const;
            ReturnType getBoolean(const std::string& path, bool& val) const;
            ReturnType getJsonObj(const std::string& path, JsonObj& val)const;
            ReturnType setInt(const std::string& path, const int& val);
            ReturnType setUint8(const std::string& path, const uint8_t& val);
            ReturnType setUint16(const std::string& path, const uint16_t& val);
            ReturnType setUint32(const std::string& path, const uint32_t& val);
            ReturnType setUint64(const std::string& path, const uint64_t& val);
            ReturnType setDouble(const std::string& path, const double& val);
            ReturnType setString(const std::string& path, const std::string& val);
            ReturnType setBoolean(const std::string& path, const bool& val);
            ReturnType setJsonObj(const std::string& path, const JsonObj& val);

        private:
            JsonParser* m_obj;
        };
    } //namespace icl
} //namespace flexd

namespace flexd {
    namespace icl {

        template<>
        inline ReturnType JsonObj::add<int>(const std::string& path, const int& val) {
            return addInt(path, val);
        }
        
        template<>
        inline ReturnType JsonObj::add<uint8_t>(const std::string& path, const uint8_t& val) {
            return addUint8(path, val);
        }
        
        template<>
        inline ReturnType JsonObj::add<uint16_t>(const std::string& path, const uint16_t& val) {
            return addUint16(path, val);
        }
        
        template<>
        inline ReturnType JsonObj::add<uint32_t>(const std::string& path, const uint32_t& val) {
            return addUint32(path, val);
        }
        
        template<>
        inline ReturnType JsonObj::add<uint64_t>(const std::string& path, const uint64_t& val) {
            return addUint64(path, val);
        }

        template<>
        inline ReturnType JsonObj::add<double>(const std::string& path, const double& val) {
            return addDouble(path, val);
        }

        template<>
        inline ReturnType JsonObj::add<std::string>(const std::string& path, const std::string& val) {
            return addString(path, val);
        }

        template<>
        inline ReturnType JsonObj::add<bool>(const std::string& path, const bool& val) {
            return addBoolean(path, val);
        }

        template<>
        inline ReturnType JsonObj::add<JsonObj>(const std::string& path, const JsonObj& val) {
            return addJsonObj(path, val);
        }
        
        template<>
        inline ReturnType JsonObj::get<int>(const std::string& path, int& val) const {
            return getInt(path, val);
        }
        
        template<>
        inline ReturnType JsonObj::get<uint8_t>(const std::string& path, uint8_t& val) const {
            return getUint8(path, val);
        }
        
        template<>
        inline ReturnType JsonObj::get<uint16_t>(const std::string& path, uint16_t& val) const {
            return getUint16(path, val);
        }
        
        template<>
        inline ReturnType JsonObj::get<uint32_t>(const std::string& path, uint32_t& val) const {
            return getUint32(path, val);
        }
        
        template<>
        inline ReturnType JsonObj::get<uint64_t>(const std::string& path, uint64_t& val) const {
            return getUint64(path, val);
        }

        template<>
        inline ReturnType JsonObj::get<double>(const std::string& path, double& val) const {
            return getDouble(path, val);
        }

        template<>
        inline ReturnType JsonObj::get<std::string>(const std::string& path, std::string& val) const {
            return getString(path, val);
        }

        template<>
        inline ReturnType JsonObj::get<bool>(const std::string& path, bool& val) const {
            return getBoolean(path, val);
        }
        
        template<>
        inline ReturnType JsonObj::get<JsonObj>(const std::string& path, JsonObj& val) const {
            return getJsonObj(path, val);
        }

        template<>
        inline ReturnType JsonObj::set<int>(const std::string& path, const int& val) {
            return setInt(path, val);
        }
        
        template<>
        inline ReturnType JsonObj::set<uint8_t>(const std::string& path, const uint8_t& val) {
            return setUint8(path, val);
        }

        template<>
        inline ReturnType JsonObj::set<uint16_t>(const std::string& path, const uint16_t& val) {
            return setUint16(path, val);
        }
        
        template<>
        inline ReturnType JsonObj::set<uint32_t>(const std::string& path, const uint32_t& val) {
            return setUint32(path, val);
        }
        
        template<>
        inline ReturnType JsonObj::set<uint64_t>(const std::string& path, const uint64_t& val) {
            return setUint64(path, val);
        }
        
        template<>
        inline ReturnType JsonObj::set<double>(const std::string& path, const double& val) {
            return setDouble(path, val);
        }

        template<>
        inline ReturnType JsonObj::set<std::string>(const std::string& path, const std::string& val) {
            return setString(path, val);
        }

        template<>
        inline ReturnType JsonObj::set<bool>(const std::string& path, const bool& val) {
            return setBoolean(path, val);
        }

        template<>
        inline ReturnType JsonObj::set<JsonObj>(const std::string& path, const JsonObj& val) {
            return setJsonObj(path, val);
        }

    } //namespace jp
} //namespace ini
#endif /* JSONOBJ_H */

