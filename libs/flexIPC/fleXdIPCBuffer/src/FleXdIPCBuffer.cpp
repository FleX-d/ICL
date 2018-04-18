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
 * File:   FleXdIPCBuffer.cpp
 * Author: Adrian Peniak
 * Author: Matus Bodorik
 * 
 * Created on January 31, 2018, 8:32 PM
 */

#include "FleXdLogger.h"
#include "FleXdIPCBuffer.h"
#include "CRC.h"
#include <unistd.h>
#include <vector>

// TODO define as static var for IPCMSG
#define HEADERSIZE 32

namespace flexd {
    namespace ilc {
        namespace epoll {
            
            FleXdIPCBuffer::FleXdIPCBuffer(size_t maxBufferSize)
            : m_maxBufferSize(maxBufferSize),
              m_cache(),
              m_queue(),
              m_onMsg(nullptr)
            {
                  FLEX_LOG_TRACE("FleXdIPCBuffer -> Start");
            }

            FleXdIPCBuffer::FleXdIPCBuffer(std::function<void(pSharedFleXdIPCMsg msg)> onMsg, size_t maxBufferSize)
            : m_maxBufferSize(maxBufferSize),
              m_cache(),
              m_queue(),
              m_onMsg(onMsg)
            {
                  FLEX_LOG_TRACE("FleXdIPCBuffer -> Start");
            }
            
            FleXdIPCBuffer::~FleXdIPCBuffer()
	    {
                FLEX_LOG_TRACE("FleXdIPCBuffer -> Destroyed");
	    }
            

            FleXdIPCBuffer::FleXdIPCBuffer(FleXdIPCBuffer&& other)       
            : m_maxBufferSize(other.m_maxBufferSize),
              m_cache(std::move(other.m_cache)),
              m_queue(std::move(other.m_queue)),
              m_onMsg(std::move(other.m_onMsg))           
            {
                FLEX_LOG_TRACE("FleXdIPCBuffer moved");
            }
            
            FleXdIPCBuffer& FleXdIPCBuffer::operator=(FleXdIPCBuffer&& other)
            {
                FLEX_LOG_TRACE("FleXdIPCBuffer moved");
                m_maxBufferSize = other.m_maxBufferSize;
                m_cache = std::move(other.m_cache);
                m_queue = std::move(other.m_queue);
                m_onMsg = std::move(other.m_onMsg);
                return *this;
            }
            
            void FleXdIPCBuffer::rcvMsg(pSharedArray8192& data, size_t size) 
            {
                FLEX_LOG_TRACE("FleXdIPCBuffer::rcvMsg() -> Receive array with size: ", size);
                m_cache.putToEnd(data->begin(), data->begin() + (size));
                rcvMsg();
            }
            
            void FleXdIPCBuffer::rcvMsg() 
            {
                FLEX_LOG_TRACE("FleXdIPCBuffer::rcvMsg() -> Start parse data");
                if(m_cache.getData().size() >= 5)
                {
                    m_cache.reset();
                    uint8_t startMsgFlag = m_cache.get<uint8_t>(4);
                    if (startMsgFlag == 0)
                    {
                        uint16_t crc16 = m_cache.get<uint16_t>(16);
                        uint8_t startHeaderFlag = m_cache.get<uint8_t>(2);
                        if (startHeaderFlag == 1)
                        {
                            uint16_t msgSize = m_cache.get<uint16_t>(16);
                            if(msgSize >= HEADERSIZE)
                            {
                                if(m_cache.getData().size() >= HEADERSIZE)
                                {
                                    uint8_t type = m_cache.get<uint8_t>(8);
                                    uint16_t msgID = m_cache.get<uint16_t>(16);
                                    uint64_t from = m_cache.get<uint64_t>(64);
                                    uint64_t to = m_cache.get<uint64_t>(64);
                                    uint32_t timeStamp = m_cache.get<uint32_t>(32);
                                    uint32_t ttl = m_cache.get<uint32_t>(32);
                                    uint8_t endHeaderFlag = m_cache.get<uint8_t>(2);
                                    if(endHeaderFlag == 2)
                                    {
                                        if(m_cache.getData().size() >= msgSize)
                                        {
                                            size_t payloadSize = msgSize - HEADERSIZE;      
                                            std::vector<uint8_t> tmp(m_cache.getRest());
                                            std::vector<uint8_t> payload(tmp.begin(), tmp.begin() + payloadSize);
                                            BiteStream newData(std::vector<uint8_t>(tmp.begin() + payloadSize, tmp.end()));
                                            std::swap(m_cache, newData);
                                            
                                            uint16_t calculateCRC = CRC::Calculate(&msgSize , sizeof(msgSize), CRC::CRC_16_ARC());
                                            calculateCRC = CRC::Calculate(&type , sizeof(type), CRC::CRC_16_ARC(), calculateCRC);
                                            calculateCRC = CRC::Calculate(&msgID , sizeof(msgID), CRC::CRC_16_ARC(), calculateCRC);
                                            calculateCRC = CRC::Calculate(&from , sizeof(from), CRC::CRC_16_ARC(), calculateCRC);
                                            calculateCRC = CRC::Calculate(&to , sizeof(to), CRC::CRC_16_ARC(), calculateCRC);
                                            calculateCRC = CRC::Calculate(&timeStamp , sizeof(timeStamp), CRC::CRC_16_ARC(), calculateCRC);
                                            calculateCRC = CRC::Calculate(&ttl , sizeof(ttl), CRC::CRC_16_ARC(), calculateCRC);
                                            calculateCRC = CRC::Calculate(&payload[0] , payload.size(), CRC::CRC_16_ARC(), calculateCRC);
                                            if(crc16 == calculateCRC){
                                                releaseMsg(std::make_shared<FleXdIPCMsg>(true, crc16, msgSize, type, msgID, from, to, timeStamp, ttl, std::move(payload)));
                                                FLEX_LOG_TRACE("FleXdIPCBuffer::rcvMsg() ->  Release complete message");
                                            } else {
                                                //releaseMsg(std::make_shared<FleXdIPCMsg>(false, crc16, msgSize, type, msgID, from, to, timeStamp, ttl, std::move(payload)))
                                                FLEX_LOG_WARN("FleXdIPCBuffer::rcvMsg() ->  Receive complete message, but CRC16 is not valid!");
                                            }
                                        } else {
                                            FLEX_LOG_TRACE("FleXdIPCBuffer::rcvMsg() ->  Wait for next read");
                                        }    
                                    } else {
                                        //std::vector<uint8_t> payload;
                                        //releaseMsg(std::make_shared<FleXdIPCMsg>(false, crc16, msgSize, type, msgID, from, to, timeStamp, ttl, std::move(payload)))
                                        FLEX_LOG_WARN("FleXdIPCBuffer::rcvMsg() ->  Receive invalid message, endHeaderFlag != 2!");
                                    }
                                } else {
                                    FLEX_LOG_TRACE("FleXdIPCBuffer::rcvMsg() ->  Wait for next read");
                                }
                            } else {
                                findNonCoruptedMessage(msgSize);
                            }
                        } else {
                            findNonCoruptedMessage(0);
                        }  
                    } else {
                        findNonCoruptedMessage(0);
                    }
                }
            }
	    
            pSharedFleXdIPCMsg FleXdIPCBuffer::front() const {
                return m_queue.front();
            }

            pSharedFleXdIPCMsg FleXdIPCBuffer::back() const {
                return m_queue.back();
            }

            pSharedFleXdIPCMsg FleXdIPCBuffer::pop() {
                auto ret = m_queue.front();
                m_queue.pop();
                return std::move(ret);
            }
            
            void FleXdIPCBuffer::releaseMsg(pSharedFleXdIPCMsg msg)
            {
                if(msg)
                {
                    FLEX_LOG_DEBUG("FleXdIPCFactory::releaseMsg() -> MSG: complete: ", msg->isComplete(), 
                                   " crc16: ", msg->getCRC16(), 
                                   " msgSize: ", msg->getMsgSize(), 
                                   " type: ", msg->getMsgType(), 
                                   " msgID: ", msg->getMsgID(), 
                                   " from: ", msg->getFrom(),
                                   " to: ", msg->getTo(), 
                                   " timeStamp: ", msg->getTimeStamp(), 
                                   " ttl: ", msg->getTtl(), 
                                   "payload: ", msg->getPayload().data());
                    if(m_onMsg)
                    {
                        m_onMsg(std::move(msg));
                    } else {
                        if(m_bufferSize < m_maxBufferSize)
                        {
                            m_queue.push(std::move(msg));
                            m_bufferSize += msg->getMsgSize();
                        } else {
                            FLEX_LOG_WARN("FleXdIPCBuffer::releaseMsg() -> onMsg = NULL and buffer is full");
                        }       
                    }
                } else {
                    FLEX_LOG_ERROR("FleXdIPCBuffer::releaseMsg() -> msg = NULL!");
                }
            }
            
            void FleXdIPCBuffer::findNonCoruptedMessage(uint16_t coruptedMsgSize)
            {
                FLEX_LOG_TRACE("FleXdIPCBuffer::findNonCoruptedMessage() ->  Try to find next non-corrupted Msg");
                unsigned counter = coruptedMsgSize/8;
                if(coruptedMsgSize < HEADERSIZE)
                {           
                    while(counter <= (m_cache.getData().size()))
                    {
                        uint8_t startMsgFlag = m_cache.getWithOffset<uint8_t>(counter*8, 4);
                        m_cache.get<uint16_t>(16);
                        uint8_t startHeaderFlag = m_cache.get<uint8_t>(2);
                        if((startMsgFlag == 0) && (startHeaderFlag == 1))
                        {
                            uint16_t msgSize = m_cache.get<uint16_t>(16);
                            if(msgSize <= m_cache.getData().size())
                            {
                                uint8_t endHeaderFlag = m_cache.getWithOffset<uint8_t>(216,2);
                                if(endHeaderFlag == 2)
                                {
                                    FLEX_LOG_TRACE("FleXdIPCBuffer::findNonCoruptedMessage() ->  Find Message Success");
                                    m_cache.getWithOffset<uint8_t>(counter*8 - 8); // last byte from previous message
                                    BiteStream oldData(m_cache.getRest());
                                    std::swap(m_cache, oldData);
                                    rcvMsg();
                                    break;   
                                }
                            }
                        } 
                        counter++;   
                    }
                }   
            } 
            
        } // namespace epoll
    } // namespace ilc
} // namespace flexd