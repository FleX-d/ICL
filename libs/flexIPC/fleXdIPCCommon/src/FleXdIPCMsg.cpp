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
 * Author: Martin Strenger
 *
 * Created on January 31, 2018, 8:36 PM
 */

#include "FleXdIPCMsg.h"
#include "FleXdIPCMsgTypes.h"
#include "BitStream.h"
#include "CRC.h"

namespace flexd {
    namespace icl {
        namespace ipc {

            FleXdIPCAdtHdr::FleXdIPCAdtHdr()
            : m_headerMask(0),
              m_value0(0),
              m_value1(0),
              m_value2(0),
              m_value3(0),
              m_value4(0),
              m_value5(0),
              m_next(nullptr)
            {
            }

            FleXdIPCAdtHdr::FleXdIPCAdtHdr(uint8_t headerMask, uint8_t value0, uint8_t value1, uint16_t value2, uint32_t value3, uint32_t value4, uint32_t value5, FleXdIPCAdtHdr* next /*= nullptr*/)
            : m_headerMask(headerMask),
              m_value0(headerMask & IPC_MSG_ADDITIONAL_HEADER_0_FLAG ? value0 : 0),
              m_value1(headerMask & IPC_MSG_ADDITIONAL_HEADER_1_FLAG ? value1 : 0),
              m_value2(headerMask & IPC_MSG_ADDITIONAL_HEADER_2_FLAG ? value2 : 0),
              m_value3(headerMask & IPC_MSG_ADDITIONAL_HEADER_3_FLAG ? value3 : 0),
              m_value4(headerMask & IPC_MSG_ADDITIONAL_HEADER_4_FLAG ? value4 : 0),
              m_value5(headerMask & IPC_MSG_ADDITIONAL_HEADER_5_FLAG ? value5 : 0),
              m_next(next)
            {
            }

            FleXdIPCAdtHdr::~FleXdIPCAdtHdr()
            {
                m_headerMask = 0;
                if (m_next != nullptr)
                {
                    delete m_next;
                    m_next = nullptr;
                }
            }

            FleXdIPCAdtHdr::FleXdIPCAdtHdr(FleXdIPCAdtHdr&& other)
            : m_headerMask(other.m_headerMask),
              m_value0(other.m_value0),
              m_value1(other.m_value1),
              m_value2(other.m_value2),
              m_value3(other.m_value3),
              m_value4(other.m_value4),
              m_value5(other.m_value5),
              m_next(other.m_next)
            {
                other.m_headerMask = other.m_value0 = other.m_value1 = 0;
                other.m_value2 = 0;
                other.m_value3 = other.m_value4 = other.m_value5 = 0;
                other.m_next = nullptr;
            }

            FleXdIPCAdtHdr& FleXdIPCAdtHdr::operator=(FleXdIPCAdtHdr&& other)
            {
                if (this != &other)
                {
                    if (m_next != nullptr) delete m_next;

                    m_headerMask = other.m_headerMask;
                    m_value0 = other.m_value0;
                    m_value1 = other.m_value1;
                    m_value2 = other.m_value2;
                    m_value3 = other.m_value3;
                    m_value4 = other.m_value4;
                    m_value5 = other.m_value5;
                    m_next = other.m_next;

                    other.m_headerMask = other.m_value0 = other.m_value1 = 0;
                    other.m_value2 = 0;
                    other.m_value3 = other.m_value4 = other.m_value5 = 0;
                    other.m_next = nullptr;
                }
                return *this;
            }

            uint8_t FleXdIPCAdtHdr::getMask() const
            {
                return m_headerMask;
            }
            void FleXdIPCAdtHdr::reset()
            {
                m_headerMask = m_value0 = m_value1 = m_value2 = m_value3 = m_value4 = m_value5 = 0;
				delete m_next;
				m_next = nullptr;
            }

            uint8_t FleXdIPCAdtHdr::getValue_0() const
            {
                return m_value0;
            }
            void FleXdIPCAdtHdr::setValue_0(uint8_t value)
            {
                m_value0 = value;
                m_headerMask |= IPC_MSG_ADDITIONAL_HEADER_0_FLAG;
            }
            void FleXdIPCAdtHdr::resetValue_0()
            {
                m_value0 = 0;
                if (m_headerMask & IPC_MSG_ADDITIONAL_HEADER_0_FLAG) m_headerMask ^= IPC_MSG_ADDITIONAL_HEADER_0_FLAG;
            }

            uint8_t FleXdIPCAdtHdr::getValue_1() const
            {
                return m_value1;
            }
            void FleXdIPCAdtHdr::setValue_1(uint8_t value)
            {
                m_value1 = value;
                m_headerMask |= IPC_MSG_ADDITIONAL_HEADER_1_FLAG;
            }
            void FleXdIPCAdtHdr::resetValue_1()
            {
                m_value1 = 0;
                if (m_headerMask & IPC_MSG_ADDITIONAL_HEADER_1_FLAG) m_headerMask ^= IPC_MSG_ADDITIONAL_HEADER_1_FLAG;
            }

            uint16_t FleXdIPCAdtHdr::getValue_2() const
            {
                return m_value2;
            }
            void FleXdIPCAdtHdr::setValue_2(uint16_t value)
            {
                m_value2 = value;
                m_headerMask |= IPC_MSG_ADDITIONAL_HEADER_2_FLAG;
            }
            void FleXdIPCAdtHdr::resetValue_2()
            {
                m_value2 = 0;
                if (m_headerMask & IPC_MSG_ADDITIONAL_HEADER_2_FLAG) m_headerMask ^= IPC_MSG_ADDITIONAL_HEADER_2_FLAG;
            }

            uint32_t FleXdIPCAdtHdr::getValue_3() const
            {
                return m_value3;
            }
            void FleXdIPCAdtHdr::setValue_3(uint32_t value)
            {
                m_value3 = value;
                m_headerMask |= IPC_MSG_ADDITIONAL_HEADER_3_FLAG;
            }
            void FleXdIPCAdtHdr::resetValue_3()
            {
                m_value3 = 0;
                if (m_headerMask & IPC_MSG_ADDITIONAL_HEADER_3_FLAG) m_headerMask ^= IPC_MSG_ADDITIONAL_HEADER_3_FLAG;
            }

            uint32_t FleXdIPCAdtHdr::getValue_4() const
            {
                return m_value4;
            }
            void FleXdIPCAdtHdr::setValue_4(uint32_t value)
            {
                m_value4 = value;
                m_headerMask |= IPC_MSG_ADDITIONAL_HEADER_4_FLAG;
            }
            void FleXdIPCAdtHdr::resetValue_4()
            {
                m_value4 = 0;
                if (m_headerMask & IPC_MSG_ADDITIONAL_HEADER_4_FLAG) m_headerMask ^= IPC_MSG_ADDITIONAL_HEADER_4_FLAG;
            }

            uint32_t FleXdIPCAdtHdr::getValue_5() const
            {
                return m_value5;
            }
            void FleXdIPCAdtHdr::setValue_5(uint32_t value)
            {
                m_value5 = value;
                m_headerMask |= IPC_MSG_ADDITIONAL_HEADER_5_FLAG;
            }
            void FleXdIPCAdtHdr::resetValue_5()
            {
                m_value5 = 0;
                if (m_headerMask & IPC_MSG_ADDITIONAL_HEADER_5_FLAG) m_headerMask ^= IPC_MSG_ADDITIONAL_HEADER_5_FLAG;
            }

            uint8_t FleXdIPCAdtHdr::getNextHeaderMask() const
            {
                return m_next != nullptr ? m_next->getMask() : 0;
            }

            uint16_t FleXdIPCAdtHdr::getSize() const
            {
                return (m_headerMask & IPC_MSG_ADDITIONAL_HEADER_0_FLAG ? sizeof(m_value0) : 0) +
                    (m_headerMask & IPC_MSG_ADDITIONAL_HEADER_1_FLAG ? sizeof(m_value1) : 0) +
                    (m_headerMask & IPC_MSG_ADDITIONAL_HEADER_2_FLAG ? sizeof(m_value2) : 0) +
                    (m_headerMask & IPC_MSG_ADDITIONAL_HEADER_3_FLAG ? sizeof(m_value3) : 0) +
                    (m_headerMask & IPC_MSG_ADDITIONAL_HEADER_4_FLAG ? sizeof(m_value4) : 0) +
                    (m_headerMask & IPC_MSG_ADDITIONAL_HEADER_5_FLAG ? sizeof(m_value5) : 0) +
                    (m_next != nullptr ? sizeof(IPC_MSG_ADDITIONAL_HEADER_NEXT_HEADER_MASK_BIT_COUNT) + m_next->getSize() : 0);
            }

            FleXdIPCAdtHdr* FleXdIPCAdtHdr::createNext()
            {
                if (m_next == nullptr)
                {
                    m_next = new FleXdIPCAdtHdr();
                    m_headerMask |= IPC_MSG_ADDITIONAL_HEADER_NEXT_HEADER_MASK_FLAG;
                }
                return m_next;
            }

            void FleXdIPCAdtHdr::deleteNext()
            {
				delete m_next;
				m_next = nullptr;
                if (m_headerMask & IPC_MSG_ADDITIONAL_HEADER_NEXT_HEADER_MASK_FLAG) m_headerMask ^= IPC_MSG_ADDITIONAL_HEADER_NEXT_HEADER_MASK_FLAG;
            }

            const FleXdIPCAdtHdr* FleXdIPCAdtHdr::getNext() const
            {
                return m_next;
            }
            FleXdIPCAdtHdr* FleXdIPCAdtHdr::getNext()
            {
                return m_next;
            }

            ////////////////
            // FleXdIPCMsg
            ////////////////

            FleXdIPCMsg::FleXdIPCMsg(uint8_t msgType, std::vector<uint8_t>&& payload)
            : m_payload(payload),
              m_headerParamType(IPC_MSG_HEADER_PARAM_TYPE_MSG_TYPE),
              m_headerParam(msgType),
              m_additionalHeader(nullptr),
              m_valid(true)
            {
            }

            FleXdIPCMsg::FleXdIPCMsg(bool headerParamType, uint8_t headerParam, std::vector<uint8_t>&& payload)
            : m_payload(payload),
              m_headerParamType(headerParamType ? IPC_MSG_HEADER_PARAM_TYPE_ADDITIONAL_HEADER : IPC_MSG_HEADER_PARAM_TYPE_MSG_TYPE),
              m_headerParam(headerParamType ? IPC_MSG_HEADER_IN_PAYLOAD_FLAG : headerParam),
              m_additionalHeader(nullptr),
              m_valid(true)
            {
            }

            FleXdIPCMsg::FleXdIPCMsg(FleXdIPCAdtHdr*& additionalHeader, std::vector<uint8_t>&& payload)
            : m_payload(payload),
              m_headerParamType(IPC_MSG_HEADER_PARAM_TYPE_ADDITIONAL_HEADER),
              m_headerParam(additionalHeader != nullptr ? additionalHeader->getMask() : 0),
              m_additionalHeader(additionalHeader),
              m_valid(true)
            {
                additionalHeader = nullptr;
            }

            FleXdIPCMsg::FleXdIPCMsg(std::vector<uint8_t>&& payload, bool calcCRC /*= true*/)
            : m_payload(payload),
              m_headerParamType(IPC_MSG_HEADER_PARAM_TYPE_ADDITIONAL_HEADER),
              m_headerParam(0),
              m_additionalHeader(calcCRC ? new FleXdIPCAdtHdr() : nullptr),
              m_valid(true)
            {
                if (calcCRC) m_additionalHeader->setValue_2(calcCRC16(&m_payload[0] , m_payload.size()));
            }

            FleXdIPCMsg::~FleXdIPCMsg()
            {
                delete m_additionalHeader;
				m_additionalHeader = nullptr;
            }

            FleXdIPCMsg::FleXdIPCMsg(FleXdIPCMsg&& other)
            : m_payload(std::move(other.m_payload)),
              m_headerParamType(other.m_headerParamType),
              m_headerParam(other.m_headerParam),
              m_additionalHeader(other.m_additionalHeader),
              m_valid(other.m_valid)
            {
                other.m_headerParamType = 0;
                other.m_headerParam = 0;
                other.m_additionalHeader = nullptr;
                other.m_valid = false;
            }

            FleXdIPCMsg& FleXdIPCMsg::operator=(FleXdIPCMsg&& other)
            {
                if (this != &other)
                {
                    m_payload.clear();
                    delete m_additionalHeader;

                    m_payload = std::move(other.m_payload);
                    m_headerParamType = other.m_headerParamType;
                    m_headerParam = other.m_headerParam;
                    m_additionalHeader = other.m_additionalHeader;
                    m_valid = other.m_valid;

                    other.m_additionalHeader = nullptr;
                    other.m_valid = false;
                }
                return *this;
            }

            bool FleXdIPCMsg::getHeaderParamType() const
            {
                return m_headerParamType;
            }

            uint8_t FleXdIPCMsg::getHeaderParam() const
            {
                if (m_headerParamType && m_additionalHeader != nullptr) return m_additionalHeader->getMask();
                return m_headerParam;
            }

            void FleXdIPCMsg::setHeaderParam(uint8_t headerParam)
            {
                if (!m_headerParamType) m_headerParam = headerParam;
            }

            void FleXdIPCMsg::resetHeaderParam()
            {
                m_headerParam = 0;
                if (m_headerParamType) deleteAdditionalHeader();
            }

            uint16_t FleXdIPCMsg::getMsgSize() const
            {
                return IPC_MSG_HEADER_SIZE + m_payload.size() + (m_additionalHeader == nullptr ? 0 : m_additionalHeader->getSize());
            }

            FleXdIPCAdtHdr* FleXdIPCMsg::createAdditionalHeader()
            {
                if (m_headerParamType && m_additionalHeader == nullptr && m_headerParam != IPC_MSG_HEADER_IN_PAYLOAD_FLAG)
                {
                    m_additionalHeader = new FleXdIPCAdtHdr();
                }
                return m_additionalHeader;
            }

            void FleXdIPCMsg::deleteAdditionalHeader()
            {
                delete m_additionalHeader;
                m_additionalHeader = nullptr;
                if (m_headerParamType && m_headerParam != IPC_MSG_HEADER_IN_PAYLOAD_FLAG) m_headerParam = 0;
            }

            const FleXdIPCAdtHdr* FleXdIPCMsg::getAdditionalHeader() const
            {
                return m_additionalHeader;
            }

            FleXdIPCAdtHdr* FleXdIPCMsg::getAdditionalHeader()
            {
                return m_additionalHeader;
            }

            const std::vector<uint8_t>& FleXdIPCMsg::getPayload() const
            {
                return m_payload;
            }

            std::vector<uint8_t> FleXdIPCMsg::releasePayload()
            {
                m_valid = false;
                return std::move(m_payload);
            }

            bool FleXdIPCMsg::isValid() const
            {
                return m_valid;
            }

            std::vector<uint8_t> FleXdIPCMsg::releaseMsg()
            {
                BitStream bs;
                bs.put((uint8_t) IPC_MSG_START_MSG_FLAG, IPC_MSG_START_MSG_FLAG_BIT_COUNT);
                bs.put((uint8_t) IPC_MSG_START_HEADER_FLAG, IPC_MSG_HEADER_START_FLAG_BIT_COUNT);
                bs.put((uint8_t) (m_headerParamType ? IPC_MSG_HEADER_PARAM_TYPE_ADDITIONAL_HEADER : IPC_MSG_HEADER_PARAM_TYPE_MSG_TYPE), IPC_MSG_HEADER_PARAMETER_TYPE_BIT_COUNT);
                if (m_headerParamType && m_additionalHeader != nullptr) m_headerParam = m_additionalHeader->getMask();
                bs.put(m_headerParam, IPC_MSG_HEADER_PARAMETER_BIT_COUNT);
                bs.put(getMsgSize(), IPC_MSG_MSG_SIZE_BIT_COUNT);
                bs.put((uint8_t) IPC_MSG_END_HEADER_FLAG, IPC_MSG_HEADER_END_FLAG_BIT_COUNT);
                if (m_headerParamType)
                {
                    FleXdIPCAdtHdr* ah = m_additionalHeader;
                    while (ah != nullptr)
                    {
                        uint8_t mask = ah->getMask();
                        if (mask & IPC_MSG_ADDITIONAL_HEADER_0_FLAG) bs.put(ah->getValue_0(), IPC_MSG_ADDITIONAL_HEADER_VALUE_0_BIT_COUNT);
                        if (mask & IPC_MSG_ADDITIONAL_HEADER_1_FLAG) bs.put(ah->getValue_1(), IPC_MSG_ADDITIONAL_HEADER_VALUE_1_BIT_COUNT);
                        if (mask & IPC_MSG_ADDITIONAL_HEADER_2_FLAG) bs.put(ah->getValue_2(), IPC_MSG_ADDITIONAL_HEADER_VALUE_2_BIT_COUNT);
                        if (mask & IPC_MSG_ADDITIONAL_HEADER_3_FLAG) bs.put(ah->getValue_3(), IPC_MSG_ADDITIONAL_HEADER_VALUE_3_BIT_COUNT);
                        if (mask & IPC_MSG_ADDITIONAL_HEADER_4_FLAG) bs.put(ah->getValue_4(), IPC_MSG_ADDITIONAL_HEADER_VALUE_4_BIT_COUNT);
                        if (mask & IPC_MSG_ADDITIONAL_HEADER_5_FLAG) bs.put(ah->getValue_5(), IPC_MSG_ADDITIONAL_HEADER_VALUE_5_BIT_COUNT);
                        if (mask & IPC_MSG_ADDITIONAL_HEADER_NEXT_HEADER_MASK_FLAG)
                        {
                            bs.put(ah->getNextHeaderMask(), IPC_MSG_ADDITIONAL_HEADER_NEXT_HEADER_MASK_BIT_COUNT);
                            ah = ah->getNext();
                        }
                        else break;
                    }
                }
                bs.put(m_payload.begin(), m_payload.end());
                return bs.releaseData();
            }

            uint16_t FleXdIPCMsg::calcCRC16(const void * data, size_t size, uint16_t otherCrc)
            {
                if(otherCrc)
                {
                    return CRC::Calculate(data , size, CRC::CRC_16_ARC(), otherCrc);
                }
                return CRC::Calculate(data , size, CRC::CRC_16_ARC());
            }

        } // namespace ipc
    } // namespace icl
} // namespace flexd
