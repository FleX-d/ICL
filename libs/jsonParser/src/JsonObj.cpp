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
 * File:   JsonObj.cpp
 * Author: Matus Bodorik
 * 
 * Created on November 23, 2017, 10:02 AM
 */

#include "JsonObj.h"
#include "json.hpp"
#include <memory>
#include <iostream>



namespace flexd
{
    namespace icl
    {

        class JsonObj::JsonParser
        {
        public:

            JsonParser() : m_ctx(nlohmann::json())
            {
            }

            JsonParser(const nlohmann::json& ctx) : m_ctx(ctx)
            {
            }
            
            ~JsonParser() = default;
            nlohmann::json m_ctx;
            };

        JsonObj::JsonObj(JsonObj&& other) : m_obj(nullptr)
        {
            m_obj = std::move(other.m_obj);
            other.m_obj = nullptr;
        }
        
        JsonObj& JsonObj::operator=(JsonObj&& other)
        {
            if(this != &other)
            {
                delete m_obj;
                
                m_obj = std::move(other.m_obj);   
                other.m_obj = nullptr;
            }
            return *this;
        }
            
        JsonObj::JsonObj()
        : m_obj(new JsonParser())
        {
        }

        JsonObj::JsonObj(const std::string& jString)
        : m_obj(new JsonParser(nlohmann::json::parse(jString)))
        {
        }

        JsonObj::~JsonObj()
        {
            delete m_obj; //TODO not removed
        }

        JsonObj JsonObj::operator+(const JsonObj& other)
        {
            if (merge(other))
            {
                return JsonObj(m_obj->m_ctx.dump());
            }
            return JsonObj(m_obj->m_ctx.dump());
        }

        JsonObj JsonObj::operator+=(const JsonObj& other)
        {
            if (merge(other))
            {
                return JsonObj(m_obj->m_ctx.dump());
            }
            return JsonObj(m_obj->m_ctx.dump());
        }

        ReturnType JsonObj::addInt(const std::string& path, const int& val)
        {
            if (pathIsValid(path) && !pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val;
                return Success;
            }
            return Error;
        }
        
        ReturnType JsonObj::addUint8(const std::string& path, const uint8_t& val)
        {
            if (pathIsValid(path) && !pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val;
                return Success;
            }
            return Error;
        }
        
        ReturnType JsonObj::addUint16(const std::string& path, const uint16_t& val)
        {
            if (pathIsValid(path) && !pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val;
                return Success;
            }
            return Error;
        }
        
        ReturnType JsonObj::addUint32(const std::string& path, const uint32_t& val)
        {
            if (pathIsValid(path) && !pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val;
                return Success;
            }
            return Error;
        }
        
        ReturnType JsonObj::addUint64(const std::string& path, const uint64_t& val)
        {
            if (pathIsValid(path) && !pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val;
                return Success;
            }
            return Error;
        }


        ReturnType JsonObj::addDouble(const std::string& path, const double& val)
        {
            if (pathIsValid(path) && !pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val;
                return Success;
            }
            return Error;
        }

        ReturnType JsonObj::addString(const std::string& path, const std::string& val)
        {
            if (pathIsValid(path) && !pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val;
                return Success;
            }
            return Error;

        }

        ReturnType JsonObj::addBoolean(const std::string& path, const bool& val)
        {
            if (pathIsValid(path) && !pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val;
                return Success;
            }
            return Error;
        }

        ReturnType JsonObj::addJsonObj(const std::string& path, const JsonObj& val)
        {
            if (pathIsValid(path) && !pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val.m_obj->m_ctx;

                return Success;
            }
            return Error;
        }
        
        ReturnType JsonObj::getInt(const std::string& path, int& val) const
        {

            if (pathIsValid(path) && pathExist(path))
            {
                nlohmann::json::json_pointer ptr(path);
                if (m_obj->m_ctx.at(ptr).is_number_integer())
                {
                    val = m_obj->m_ctx.at(ptr);
                    return Success;
                }
            }
            return Error;
        }
        
        ReturnType JsonObj::getUint8(const std::string& path, uint8_t& val) const
        {
            if (pathIsValid(path) && pathExist(path))
            {
                nlohmann::json::json_pointer ptr(path);
                if (m_obj->m_ctx.at(ptr).is_number_unsigned())
                {
                    val = m_obj->m_ctx.at(ptr);
                    return Success;
                }
            }
            return Error;
        }
        
        ReturnType JsonObj::getUint16(const std::string& path, uint16_t& val) const
        {
            if (pathIsValid(path) && pathExist(path))
            {
                nlohmann::json::json_pointer ptr(path);
                if (m_obj->m_ctx.at(ptr).is_number_unsigned())
                {
                    val = m_obj->m_ctx.at(ptr);
                    return Success;
                }
            }
            return Error;
        }
        
        ReturnType JsonObj::getUint32(const std::string& path, uint32_t& val) const
        {
            if (pathIsValid(path) && pathExist(path))
            {
                nlohmann::json::json_pointer ptr(path);
                if (m_obj->m_ctx.at(ptr).is_number_unsigned())
                {
                    val = m_obj->m_ctx.at(ptr);
                    return Success;
                }
            }
            return Error;
        }
        
        ReturnType JsonObj::getUint64(const std::string& path, uint64_t& val) const
        {
            if (pathIsValid(path) && pathExist(path))
            {
                nlohmann::json::json_pointer ptr(path);
                if (m_obj->m_ctx.at(ptr).is_number_unsigned())
                {
                    val = m_obj->m_ctx.at(ptr);
                    return Success;
                }
            }
            return Error;
        }

        ReturnType JsonObj::getDouble(const std::string& path, double& val) const
        {
            if (pathIsValid(path) && pathExist(path))
            {
                nlohmann::json::json_pointer ptr(path);
                if (m_obj->m_ctx.at(ptr).is_number_float())
                {
                    val = m_obj->m_ctx.at(ptr);
                    return Success;
                }
            }
            return Error;
        }

        ReturnType JsonObj::getString(const std::string& path, std::string& val) const
        {
            if (pathIsValid(path) && pathExist(path))
            {
                nlohmann::json::json_pointer ptr(path);
                if (m_obj->m_ctx.at(ptr).is_string())
                {
                    val = m_obj->m_ctx.at(ptr);
                    return Success;
                }
            }
            return Error;
        }

        ReturnType JsonObj::getBoolean(const std::string& path, bool& val) const
        {
            if (pathIsValid(path) && pathExist(path))
            {
                nlohmann::json::json_pointer ptr(path);
                if (m_obj->m_ctx.at(ptr).is_boolean())
                {
                    val = m_obj->m_ctx.at(ptr);
                    return Success;
                }
            }
            return Error;
        }

        ReturnType JsonObj::getJsonObj(const std::string& path, JsonObj& val)const
        {
            if (pathIsValid(path) && pathExist(path))
            {
                nlohmann::json::json_pointer ptr(path);
                if (m_obj->m_ctx.at(ptr).is_object())
                {
                    val.m_obj->m_ctx = m_obj->m_ctx.at(ptr);
                    return Success;
                }
            }
            return Error;
        }

        ReturnType JsonObj::setInt(const std::string& path, const int& val)
        {
            if (pathIsValid(path) && pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val;
                return Success;
            }
            return Error;
        }
        
        ReturnType JsonObj::setUint8(const std::string& path, const uint8_t& val)
        {
            if (pathIsValid(path) && pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val;
                return Success;
            }
            return Error;
        }
        
        ReturnType JsonObj::setUint16(const std::string& path, const uint16_t& val)
        {
            if (pathIsValid(path) && pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val;
                return Success;
            }
            return Error;
        }
        
        ReturnType JsonObj::setUint32(const std::string& path, const uint32_t& val)
        {
            if (pathIsValid(path) && pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val;
                return Success;
            }
            return Error;
        }

        ReturnType JsonObj::setUint64(const std::string& path, const uint64_t& val)
        {
            if (pathIsValid(path) && pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val;
                return Success;
            }
            return Error;
        }

        ReturnType JsonObj::setDouble(const std::string& path, const double& val)
        {
            if (pathIsValid(path) && pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val;
                return Success;
            }
            return Error;
        }

        ReturnType JsonObj::setString(const std::string& path, const std::string& val)
        {
            if (pathIsValid(path) && pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val;
                return Success;
            }
            return Error;
        }

        ReturnType JsonObj::setBoolean(const std::string& path, const bool& val)
        {
            if (pathIsValid(path) && pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val;
                return Success;
            }
            return Error;
        }

        ReturnType JsonObj::setJsonObj(const std::string& path, const JsonObj& val)
        {
            if (pathIsValid(path) && pathExist(path))
            {
                m_obj->m_ctx[nlohmann::json::json_pointer(path)] = val.m_obj->m_ctx;

                return Success;
            }
            return Error;
        }

        ReturnType JsonObj::remove(const std::string& path)
        {

            if (pathIsValid(path) && pathExist(path))
            {
                std::string s = "";
                std::vector<std::string> temp = parsePath(path);
                int size = temp.size();
                for (auto& it : temp)
                {
                    if (size == 1)
                    {
                        m_obj->m_ctx[nlohmann::json::json_pointer(s)].erase(it);
                        return Success;
                    }
                    s.append("/" + it);
                    --size;
                }
            }
            return Error;
        }

        ReturnType JsonObj::exist(const std::string& path)
        {
            if (pathIsValid(path) && pathExist(path))
            {
                return Success;
            }
            return Error;
        }

        void JsonObj::dump(int d, int flat) const
        {
            if ((d == -1 || d > 0) && flat == 0)
            {
                std::cout << m_obj->m_ctx.dump(d) << std::endl;
            } else if ((d == -1 || d > 0) && flat == 1)
            {
                std::cout << m_obj->m_ctx.flatten().dump(d) << std::endl;
            }
        }

        void JsonObj::storeJson(JsonParser* other)
        {
            m_obj->m_ctx = other->m_ctx;
        }

        std::string JsonObj::getJson()
        {
            std::string str = m_obj->m_ctx.dump();
            return std::move(str);
        }

        ReturnType JsonObj::merge(const JsonObj& other)
        {
            if(!other.m_obj->m_ctx.empty())
            {
                for (nlohmann::json::const_iterator it = other.m_obj->m_ctx.begin(); it != other.m_obj->m_ctx.end(); ++it)
                {
                    m_obj->m_ctx[it.key()] = it.value();
                }
                return Success;
            }
            return Error;
        }

        ReturnType JsonObj::pathIsValid(const std::string& path) const
        {
            if (path.size() >= 1 && (path.at(0) == '/') && (path.at(1) != '/') && (path.at(path.length() - 1) != '/'))
            {
                return Success;
            }
            return Error;
        }

        std::vector<std::string> JsonObj::parsePath(const std::string& path) const
        {
            std::vector<std::string> token;

            if (pathIsValid(path))
            {
                std::string s = path;
                std::string delimiter = "/";
                size_t pos = 0;


                s.erase(0, pos + delimiter.length());
                while ((pos = s.find(delimiter)) != std::string::npos)
                {
                    token.push_back(s.substr(0, pos));
                    s.erase(0, pos + delimiter.length());
                }
                token.push_back(s);
                return token;

            } else
            {
                token.clear();
                return (token);
            }
        }

        ReturnType JsonObj::pathExist(const std::string& path) const
        {

            if (pathIsValid(path))
            {
                std::vector<std::string> temp;
                temp = parsePath(path);

                if (!temp.empty())
                {
                    std::string s = "";
                    int size = temp.size();

                    for (auto& it : temp)
                    {
                        s.append("/" + it);
                        nlohmann::json::json_pointer ptr(s);
                        try
                        {
                            bool temp = m_obj->m_ctx.at(ptr).empty();
                            if (!temp)
                            {
                                size--;
                                continue;
                            }
                        } catch (nlohmann::detail::out_of_range& e)
                        {
                            //std::cout << e.what() << '\n';
                            return Error;
                        }
                    }
                    return Success;
                }
            }
            return Error;
        }
    }
}

