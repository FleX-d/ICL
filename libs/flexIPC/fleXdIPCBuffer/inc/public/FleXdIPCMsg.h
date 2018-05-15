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
 * File:   FleXdIPCMsg.h
 * Author: Adrian Peniak
 * Author: Matus Bodorik
 *
 * Created on January 31, 2018, 8:36 PM
 */

#ifndef FLEXDIPCMSG_H
#define FLEXDIPCMSG_H

#include <cstdint>
#include <vector>
#include <memory>


namespace flexd {
    namespace icl {
        namespace epoll {

            /*
             *  start msg........start header......................................................... end header.............
             * |    4    |  16   |    2    |  16     |    8    |  16   |  64  | 64 |    32     | 32  |     2      |    n    |
             * |   0x0   | CRC16 |   0x1   | msgSize | msgType | msgID | from | to | timeStamp | ttl |    0x2     | payload |
             * 
             * msgSize = start byte + header + payload + end byte;
             */
            class FleXdIPCMsg {
            public:
                //TODOD Constructor empty
                explicit FleXdIPCMsg(bool complete, uint16_t CRC16, uint16_t size, uint8_t type, uint16_t ID,  uint64_t from, uint64_t to, uint32_t timeStamp, uint32_t ttl, std::vector<uint8_t>&& payload);
                explicit FleXdIPCMsg(bool complete, uint16_t CRC16, uint16_t size, uint8_t type, uint16_t ID,  uint64_t from, uint64_t to, uint32_t timeStamp, uint32_t ttl, const std::vector<uint8_t>& payload);
                ~FleXdIPCMsg() = default;
                /**
                 * Function return size of message. 
                 * @return size of Message - uint16_t
                 */
                uint16_t getMsgSize() const;
                /**
                 * Function return type of message. 
                 * @return type of Message - uint8_t
                 */
                uint8_t getMsgType() const;
                /**
                 * Function return message ID. 
                 * @return ID of Message - uint16_t
                 */
                uint16_t getMsgID() const;
                /**
                 * Function return ID of sender.  
                 * @return ID of sender - uint64_t
                 */
                uint64_t getFrom() const;
                /**
                 * Function return ID of receiver.  
                 * @return ID of receiver - uint64_t
                 */
                uint64_t getTo() const;
                /**
                 * Function return time stamp.  
                 * @return time stamp - uint32_t
                 */
                uint32_t getTimeStamp() const;
                /**
                 * Function return time to live of message. 
                 * @return time to live of message - uint32_t
                 */
                uint32_t getTtl() const;
                /**
                 * Function return CRC16 of all atributes.  
                 * @return CRC16 of all atributes - uint16_t
                 */
                uint16_t getCRC16() const;
                /**
                 * Function return payload of data. 
                 * @return vector uint16_t CRC16
                 */
                const std::vector<uint8_t>& getPayload() const;
                /**
                 * Static function calculate CRC16 from input data.
                 * @param data - data to calculate CRC16 
                 * @param size - size of data 
                 * @param otherCrc - default 0, other CRC16 which will be included at new CRC16
                 * @return uint16_t CRC16 from input data 
                 */
                static uint16_t calkCRC16(const void * data, size_t size, uint16_t otherCrc = 0);
                /**
                 * Function return bool value of completed message. 
                 * @return true if message is complete, false otherwise.
                 */
                bool isComplete() const;
                /**
                 * Function set bool value of completed message. 
                 * @param complete -bool value of completed message 
                 */
                void setComplete(bool complete);
                /**
                 * Function creates a vector of bytes from attributes of a FleXdIPCMsg, which is ready to send.
                 * @return std::vector<uint8_t> of bytes
                 */
                std::vector<uint8_t> releaseMsg();
                /**
                 * Function calculate CRC16 from all attributes of a FleXdIPCMsg. 
                 * @return uint16_t CRC16
                 */
                uint16_t calculateCRC();
                /**
                 * Function calculate size from all attributes of a FleXdIPCMsg. 
                 * @return uint16_t CRC16
                 */
                uint16_t calculateSize();
                
                FleXdIPCMsg(const FleXdIPCMsg&) = default;
                FleXdIPCMsg& operator=(const FleXdIPCMsg&) = default;
 
            private:
                bool m_complete;
                const uint16_t m_CRC16;
                const uint16_t m_msgSize;
                const uint8_t m_msgType;
                const uint16_t m_msgID;
                const uint64_t m_from;
                const uint64_t m_to;
                const uint32_t m_timeStamp;
                const uint32_t m_ttl;
                const std::vector<uint8_t> m_payload;
            };
            typedef std::shared_ptr<FleXdIPCMsg> pSharedFleXdIPCMsg;
            
        } // namespace epoll
    } // namespace icl
} // namespace flexd

#endif /* FLEXDIPCMSG_H */
