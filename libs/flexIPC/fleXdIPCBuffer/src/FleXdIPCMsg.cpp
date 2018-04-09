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
 * File:   FleXdIPCMsg.cpp
 * Author: Adrian Peniak
 * Author: Matus Bodorik
 * 
 * Created on January 31, 2018, 8:36 PM
 */

#define IPCMSGHEADERSIZE 256

#include "FleXdIPCMsg.h"
#include "FleXdIPCBufferTypes.h"
#include "BitStream.h"
#include "CRC.h"

namespace flexd {
    namespace ilc {
        namespace epoll {

            FleXdIPCMsg::FleXdIPCMsg(bool complete, uint16_t crc16, uint16_t size, uint8_t type, uint16_t ID, uint64_t from, uint64_t to, uint32_t timeStamp, uint32_t ttl, std::vector<uint8_t>&& payload)
            : m_complete(complete),
            m_CRC16(crc16),
            m_msgSize(size),
            m_msgType(type),
            m_msgID(ID),
            m_from(from),
            m_to(to),
            m_timeStamp(timeStamp),
            m_ttl(ttl),
            m_payload(std::move(payload))
            {
            }

            FleXdIPCMsg::FleXdIPCMsg(bool complete, uint16_t crc16, uint16_t size, uint8_t type, uint16_t ID, uint64_t from, uint64_t to, uint32_t timeStamp, uint32_t ttl, const std::vector<uint8_t>& payload)
            : m_complete(complete),
            m_CRC16(crc16),
            m_msgSize(size),
            m_msgType(type),
            m_msgID(ID),
            m_from(from),
            m_to(to),
            m_timeStamp(timeStamp),
            m_ttl(ttl),
            m_payload(payload)
            {
            }

            uint16_t FleXdIPCMsg::getMsgSize() const
            {
                return m_msgSize;
            }

            uint8_t FleXdIPCMsg::getMsgType() const
            {
                return m_msgType;
            }

            uint16_t FleXdIPCMsg::getMsgID() const
            {
                return m_msgID;
            }

            uint64_t FleXdIPCMsg::getFrom() const
            {
                return m_from;
            }

            uint64_t FleXdIPCMsg::getTo() const
            {
                return m_to;
            }

            uint32_t FleXdIPCMsg::getTimeStamp() const
            {
                return m_timeStamp;
            }

            uint32_t FleXdIPCMsg::getTtl() const
            {
                return m_ttl;
            }

            uint16_t FleXdIPCMsg::getCRC16() const
            {
                return m_CRC16;
            }

            const std::vector<uint8_t>& FleXdIPCMsg::getPayload() const
            {
                return m_payload;
            }
            
            uint16_t FleXdIPCMsg::calkCRC16(const void * data, size_t size, uint16_t otherCrc)
            {
                if(otherCrc)
                {
                    return CRC::Calculate(data , size, CRC::CRC_16_ARC(), otherCrc);
                }
                return CRC::Calculate(data , size, CRC::CRC_16_ARC());
            }

            bool FleXdIPCMsg::isComplete() const
            {
                return m_complete;
            }

            void FleXdIPCMsg::setComplete(const bool complete)
            {
                m_complete = complete;
            }

            std::vector<uint8_t> FleXdIPCMsg::releaseMsg()
            {
                BiteStream bs;
                uint16_t crc  = calculateCRC();
                bs.put((uint8_t) 0, 4);
                if(m_CRC16 == 0){bs.put(crc, 16);}
                else {bs.put(m_CRC16, 16);}
                bs.put((uint8_t) 1, 2);
                bs.put(m_msgSize, 16);
                bs.put(m_msgType, 8);
                bs.put(m_msgID, 16);
                bs.put((uint64_t) m_from, 64);
                bs.put((uint64_t) m_to, 64);
                bs.put(m_timeStamp, 32);
                bs.put(m_ttl, 32);
                bs.put((uint8_t) 2, 2);
                bs.put(m_payload.begin(), m_payload.end());
                std::vector<uint8_t> msg = bs.releaseData();
                return std::move(msg);
            }
            
            uint16_t FleXdIPCMsg::calculateCRC()
            {
                uint16_t crc = 0;
                crc = CRC::Calculate(&m_msgSize , sizeof(m_msgSize), CRC::CRC_16_ARC());
                crc = CRC::Calculate(&m_msgType , sizeof(m_msgType), CRC::CRC_16_ARC(), crc);
                crc = CRC::Calculate(&m_msgID , sizeof(m_msgID), CRC::CRC_16_ARC(), crc);
                crc = CRC::Calculate(&m_from , sizeof(m_from), CRC::CRC_16_ARC(), crc);
                crc = CRC::Calculate(&m_to , sizeof(m_to), CRC::CRC_16_ARC(), crc);
                crc = CRC::Calculate(&m_timeStamp , sizeof(m_timeStamp), CRC::CRC_16_ARC(), crc);
                crc = CRC::Calculate(&m_ttl , sizeof(m_ttl), CRC::CRC_16_ARC(), crc);
                crc = CRC::Calculate(&m_payload[0] , m_payload.size(), CRC::CRC_16_ARC(), crc);
                return crc;
            }
        } // namespace epoll
    } // namespace ilc
} // namespace flexd
