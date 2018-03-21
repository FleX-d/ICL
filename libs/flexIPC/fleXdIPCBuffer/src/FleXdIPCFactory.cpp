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
 * File:   FleXdIPCFactory.cpp
 * Author: Matus Bodorik
 * 
 * Created on February 21, 2018, 8:32 AM
 */

#include "FleXdIPCFactory.h"
#include "CRC.h"
#include <iostream>

namespace flexd {
    namespace ilc {
        namespace epoll {

            FleXdIPCFactory::FleXdIPCFactory(std::function<void(pSharedFleXdIPCMsg msg)> releaseMsg)
            : m_releaseMsg(releaseMsg)
            {      
            }

            FleXdIPCFactory::~FleXdIPCFactory()
            {
            }

            void FleXdIPCFactory::parseData(const pSharedArray8192& data, const size_t size)
            {
                
                uint16_t msgSize = 0;
                uint8_t startMsgFlag = 0;
                uint16_t crc16 = 0;
                uint8_t startHeaderFlag = 0;
                uint8_t type = 0;
                uint16_t msgID = 0;
                uint64_t from = 0;
                uint64_t to = 0;
                uint32_t timeStamp = 0;
                uint32_t ttl = 0;
                uint8_t endHeaderFlag =0;
                m_data.putToEnd(data->begin(), data->begin() + (size));
                if(m_data.getData().size() >= 5)
                {
                    m_data.reset();
                    startMsgFlag = m_data.get<uint8_t>(4);
                    if (startMsgFlag == 0)
                    {
                        
                        crc16 = m_data.get<uint16_t>(16);
                        startHeaderFlag = m_data.get<uint8_t>(2);
                        if (startHeaderFlag == 1)
                        {
                            msgSize = m_data.get<uint16_t>(16);
                            if((m_data.getData().size() >= msgSize) && (msgSize >= 32))
                            {
                                type = m_data.get<uint8_t>(8);
                                msgID = m_data.get<uint16_t>(16);
                                from = m_data.get<uint64_t>(64);
                                to = m_data.get<uint64_t>(64);
                                timeStamp = m_data.get<uint32_t>(32);
                                ttl = m_data.get<uint32_t>(32);
                                endHeaderFlag = m_data.get<uint8_t>(2);
                                if(endHeaderFlag == 2)
                                {
                                    size_t headerSize = 32;
                                    size_t payloadSize = 0;
                                    payloadSize = msgSize - headerSize;      
                                    std::vector<uint8_t> payload;
                                    for(size_t i = 0; i < payloadSize; i++)
                                    {
                                        payload.push_back(m_data.get<uint8_t>(8));
                                    }
                                    uint16_t calculateCRC = 0;
                                    calculateCRC = CRC::Calculate(&msgSize , sizeof(msgSize), CRC::CRC_16_ARC());
                                    calculateCRC = CRC::Calculate(&type , sizeof(type), CRC::CRC_16_ARC(), calculateCRC);
                                    calculateCRC = CRC::Calculate(&msgID , sizeof(msgID), CRC::CRC_16_ARC(), calculateCRC);
                                    calculateCRC = CRC::Calculate(&from , sizeof(from), CRC::CRC_16_ARC(), calculateCRC);
                                    calculateCRC = CRC::Calculate(&to , sizeof(to), CRC::CRC_16_ARC(), calculateCRC);
                                    calculateCRC = CRC::Calculate(&timeStamp , sizeof(timeStamp), CRC::CRC_16_ARC(), calculateCRC);
                                    calculateCRC = CRC::Calculate(&ttl , sizeof(ttl), CRC::CRC_16_ARC(), calculateCRC);
                                    calculateCRC = CRC::Calculate(&payload[0] , payload.size(), CRC::CRC_16_ARC(), calculateCRC);
                                    if(crc16 == calculateCRC){
                                        releaseMsg(true,crc16, msgSize, type, msgID, from, to, timeStamp, ttl, std::move(payload));
                                    } else 
                                    {
                                        releaseMsg(false,crc16, msgSize, type, msgID, from, to, timeStamp, ttl, std::move(payload));
                                    } 
                                }else 
                                {
                                    std::vector<uint8_t> payload;
                                    releaseMsg(false,crc16, msgSize, type, msgID, from, to, timeStamp, ttl, std::move(payload));
                                }  
                            }else 
                            {
                                std::vector<uint8_t> payload;
                                releaseMsg(false,crc16, msgSize, type, msgID, from, to, timeStamp, ttl, std::move(payload));
                            }     
                        }
                    }
                }
            }
            
            
            void FleXdIPCFactory::releaseMsg(bool complete ,uint16_t crc16,uint16_t msgSize, uint8_t type,uint16_t msgID,uint64_t from,uint64_t to,uint32_t timeStamp,uint32_t ttl,const std::vector<uint8_t>& payload)
            {
                m_releaseMsg(std::move(std::make_shared<FleXdIPCMsg>(complete, crc16, msgSize, type, msgID, from, to, timeStamp, ttl, payload)));
                BiteStream newData(std::move(m_data.getRest()));
                std::swap(m_data, newData);
                if(m_data.getData().size() != 0)
                {                                             
                    std::vector<uint8_t> data = m_data.getData();
                    std::array<uint8_t, 8192> array;
                    std::memcpy(&array[0], data.data(), data.size());
                    std::shared_ptr<std::array<uint8_t, 8192> > array_ptr = std::make_shared<std::array<uint8_t, 8192> >(array);
                    BiteStream newData;
                    std::swap(m_data, newData);
                    parseData(array_ptr, data.size());
                }                                            
            }
    
    
        }
    }
}

