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
 * Author: Martin Strenger
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
        namespace ipc {

            class FleXdIPCAdtHdr {
            public:
                FleXdIPCAdtHdr();
                explicit FleXdIPCAdtHdr(uint8_t headerMask, uint8_t value0, uint8_t value1, uint16_t value2, uint32_t value3, uint32_t value4, uint32_t value5);
                ~FleXdIPCAdtHdr();
                FleXdIPCAdtHdr(const FleXdIPCAdtHdr&) = delete;
                FleXdIPCAdtHdr& operator=(const FleXdIPCAdtHdr&) = delete;
                FleXdIPCAdtHdr(FleXdIPCAdtHdr&&);
                FleXdIPCAdtHdr& operator=(FleXdIPCAdtHdr&&);
                uint8_t getMask() const;
                void reset();
                uint8_t getValue_0() const;
                void setValue_0(uint8_t value);
                void resetValue_0();
                uint8_t getValue_1() const;
                void setValue_1(uint8_t value);
                void resetValue_1();
                uint16_t getValue_2() const;
                void setValue_2(uint16_t value);
                void resetValue_2();
                uint32_t getValue_3() const;
                void setValue_3(uint32_t value);
                void resetValue_3();
                uint32_t getValue_4() const;
                void setValue_4(uint32_t value);
                void resetValue_4();
                uint32_t getValue_5() const;
                void setValue_5(uint32_t value);
                void resetValue_5();
                uint8_t getNextHeaderMask() const;
                uint16_t getSize() const;
                FleXdIPCAdtHdr* createNext();
                void deleteNext();
                const FleXdIPCAdtHdr* getNext() const;
                FleXdIPCAdtHdr* getNext();

            private:
                uint8_t m_headerMask;
                uint8_t m_value0;
                uint8_t m_value1;
                uint16_t m_value2;
                uint32_t m_value3;
                uint32_t m_value4;
                uint32_t m_value5;
                FleXdIPCAdtHdr* m_next;
            };

            /*
             * GENERAL MESSAGE FORMAT
             *              ...msg start....header start...........................................................header end................................
             * BITCOUNT     |      3      |      2      |         1         |           8            |    16     |      2     |      0 - 136      |    n    |
             * VALUE        |     0x0     |     0x1     | header param type |     header parameter   |  msgSize  |     0x2    | additional header | payload |
             *
             * POSSIBLE MESSAGE TYPES
             *              |     0x0     |     0x1     |         0         |      message type      |  msgSize  |     0x2    |        ---        | payload |
             *              |     0x0     |     0x1     |         1         | additional header mask |  msgSize  |     0x2    | additional header | payload |
             *              |     0x0     |     0x1     |         1         |         0xFF           |  msgSize  |     0x2    |        ---        | payload |
             *
             * msgSize = msg start + header + additional header + payload;
             */
            class FleXdIPCMsg {
            public:
                explicit FleXdIPCMsg(uint8_t msgType, std::vector<uint8_t>&& payload);
                explicit FleXdIPCMsg(bool headerParamType, uint8_t headerParam, std::vector<uint8_t>&& payload);
                explicit FleXdIPCMsg(FleXdIPCAdtHdr*& additionalHeader, std::vector<uint8_t>&& payload);
                explicit FleXdIPCMsg(std::vector<uint8_t>&& payload, bool calculateCRC = true);
                virtual ~FleXdIPCMsg();
                FleXdIPCMsg(const FleXdIPCMsg&) = delete;
                FleXdIPCMsg& operator=(const FleXdIPCMsg&) = delete;
                FleXdIPCMsg(FleXdIPCMsg&&);
                FleXdIPCMsg& operator=(FleXdIPCMsg&&);
                /*
                 * Function returns header parameter type
                 * @return header parameter type - 0 = message type, 1 = additional header mask
                 */
                bool getHeaderParamType() const;
                /*
                 * Function returns header parameter
                 * @return header parameter - message type or additional header mask
                 */
                uint8_t getHeaderParam() const;
                /*
                 * Function sets header parameter, works only when header parameter type = 0
                 */
                void setHeaderParam(uint8_t headerParam);
                /*
                 * Function resets header parameter and deletes the additional header if exists
                 * @param headerParam - header parameter
                 */
                void resetHeaderParam();
                /*
                 * Function returns size of the message
                 * @return size of the message
                 */
                uint16_t getMsgSize() const;
                /*
                 * Function creates and returns pointer to additional header, works only when header parameter type = 1
                 * @return pointer to additional header object or nullptr
                 */
                FleXdIPCAdtHdr* createAdditionalHeader();
                /*
                 * Function returns pointer to additional header
                 * @return pointer to additional header object or nullptr
                 */
                const FleXdIPCAdtHdr* getAdditionalHeader() const;
                FleXdIPCAdtHdr* getAdditionalHeader();
                /*
                 * Function deletes additional header if exists
                 */
                void deleteAdditionalHeader();
                /*
                 * Function returns payload data
                 * @return data uint8_t vector
                 */
                const std::vector<uint8_t>& getPayload() const;
                /*
                 * Function moves payload data
                 * @return data uint8_t vector
                 */
                std::vector<uint8_t>&& releasePayload();
                /*
                 * Function returns validity of the message
                 * @return message validity
                 */
                bool isValid() const;
                /*
                 * Function creates a vector of bytes from attributes of a FleXdIPCMsg, which is ready to send.
                 * @return std::vector<uint8_t> of bytes
                 */
                std::vector<uint8_t> releaseMsg();
                /*
                 * Static function calculate CRC16 from input data.
                 * @param data - data to calculate CRC16
                 * @param size - size of data
                 * @param otherCrc - default 0, other CRC16 which will be included at new CRC16
                 * @return uint16_t CRC16 from input data
                 */
                static uint16_t calcCRC16(const void* data, size_t size, uint16_t otherCrc = 0);

            private:
                std::vector<uint8_t> m_payload;
                bool m_headerParamType;
                uint8_t m_headerParam;
                FleXdIPCAdtHdr* m_additionalHeader;
                bool m_valid;
            };
            typedef std::shared_ptr<FleXdIPCMsg> pSharedFleXdIPCMsg;

        } // namespace epoll
    } // namespace icl
} // namespace flexd

#endif /* FLEXDIPCMSG_H */
