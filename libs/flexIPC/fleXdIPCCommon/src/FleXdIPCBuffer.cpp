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

#include "FleXdIPCBuffer.h"
#include "FleXdIPCBufferTypes.h"
#include "FleXdIPCMsgTypes.h"
#include "CRC.h"
#include <unistd.h>
#include <vector>

// TODO define as static var for IPCMSG
namespace flexd {
    namespace icl {
        namespace ipc {

            FleXdIPCBuffer::FleXdIPCBuffer(int fd, size_t maxBufferSize)
            : m_fd(fd),
              m_maxBufferSize(maxBufferSize),
              m_bufferSize(0),
              m_cache(),
              m_queue(),
              m_onMsg(nullptr)
            {
            }

            FleXdIPCBuffer::FleXdIPCBuffer(int fd, std::function<void(pSharedFleXdIPCMsg, int)> onMsg, size_t maxBufferSize)
            : m_fd(fd),
              m_maxBufferSize(maxBufferSize),
              m_bufferSize(0),
              m_cache(),
              m_queue(),
              m_onMsg(onMsg)
            {
            }

            FleXdIPCBuffer::~FleXdIPCBuffer()
            {
            }


            FleXdIPCBuffer::FleXdIPCBuffer(FleXdIPCBuffer&& other)
            : m_fd(other.m_fd),
              m_maxBufferSize(other.m_maxBufferSize),
              m_bufferSize(other.m_bufferSize),
              m_cache(std::move(other.m_cache)),
              m_queue(std::move(other.m_queue)),
              m_onMsg(std::move(other.m_onMsg))
            {
            }

            FleXdIPCBuffer& FleXdIPCBuffer::operator=(FleXdIPCBuffer&& other)
            {
                m_fd = other.m_fd;
                m_maxBufferSize = other.m_maxBufferSize;
                m_bufferSize = other.m_bufferSize;
                m_cache = std::move(other.m_cache);
                m_queue = std::move(other.m_queue);
                m_onMsg = std::move(other.m_onMsg);
                return *this;
            }

            int FleXdIPCBuffer::getFd() const {
                return m_fd;
            }

            void FleXdIPCBuffer::setFd(int fd) {
                m_fd = fd;
            }

            void FleXdIPCBuffer::rcvMsg(pSharedArray8192& data, size_t size)
            {
                m_cache.putToEnd(data->begin(), data->begin() + (size));
                rcvMsg();
            }

            void FleXdIPCBuffer::rcvMsg()
            {
                if (m_cache.getData().size() >= IPC_MSG_MIN_SIZE_OF_CACHE_TO_PARSE)
                {
                    m_cache.reset();
                    if (m_cache.get<uint8_t>(IPC_MSG_START_MSG_FLAG_BIT_COUNT) == IPC_MSG_START_MSG_FLAG)
                    {
                        if (m_cache.get<uint8_t>(IPC_MSG_HEADER_START_FLAG_BIT_COUNT) == IPC_MSG_START_HEADER_FLAG)
                        {
                            const bool headerParamType = m_cache.get<uint8_t>(IPC_MSG_HEADER_PARAMETER_TYPE_BIT_COUNT);
                            const uint8_t headerParam = m_cache.get<uint8_t>(IPC_MSG_HEADER_PARAMETER_BIT_COUNT);
                            const uint16_t msgSize = m_cache.get<uint16_t>(IPC_MSG_MSG_SIZE_BIT_COUNT);
                            if (m_cache.get<uint8_t>(IPC_MSG_HEADER_END_FLAG_BIT_COUNT) == IPC_MSG_END_HEADER_FLAG)
                            {
                                if (m_cache.getData().size() >= msgSize)
                                {
                                    FleXdIPCAdtHdr* msgAdditionalHeader = nullptr;
                                    uint16_t addHdrSize = 0;
                                    if (headerParamType && headerParam != 0 && headerParam != IPC_MSG_HEADER_IN_PAYLOAD_FLAG)
                                    {
                                        uint8_t headerMask = headerParam;
                                        msgAdditionalHeader = new FleXdIPCAdtHdr();
                                        FleXdIPCAdtHdr* additionalHeader = msgAdditionalHeader;
                                        while (headerMask != 0 && additionalHeader != nullptr)
                                        {
                                            if (headerMask & IPC_MSG_ADDITIONAL_HEADER_0_FLAG)
                                                additionalHeader->setValue_0(m_cache.get<uint8_t>(IPC_MSG_ADDITIONAL_HEADER_VALUE_0_BIT_COUNT));
                                            if (headerMask & IPC_MSG_ADDITIONAL_HEADER_1_FLAG)
                                                additionalHeader->setValue_1(m_cache.get<uint8_t>(IPC_MSG_ADDITIONAL_HEADER_VALUE_1_BIT_COUNT));
                                            if (headerMask & IPC_MSG_ADDITIONAL_HEADER_2_FLAG)
                                                additionalHeader->setValue_2(m_cache.get<uint16_t>(IPC_MSG_ADDITIONAL_HEADER_VALUE_2_BIT_COUNT));
                                            if (headerMask & IPC_MSG_ADDITIONAL_HEADER_3_FLAG)
                                                additionalHeader->setValue_3(m_cache.get<uint32_t>(IPC_MSG_ADDITIONAL_HEADER_VALUE_3_BIT_COUNT));
                                            if (headerMask & IPC_MSG_ADDITIONAL_HEADER_4_FLAG)
                                                additionalHeader->setValue_4(m_cache.get<uint32_t>(IPC_MSG_ADDITIONAL_HEADER_VALUE_4_BIT_COUNT));
                                            if (headerMask & IPC_MSG_ADDITIONAL_HEADER_5_FLAG)
                                                additionalHeader->setValue_5(m_cache.get<uint32_t>(IPC_MSG_ADDITIONAL_HEADER_VALUE_5_BIT_COUNT));
                                            if (headerMask & IPC_MSG_ADDITIONAL_HEADER_NEXT_HEADER_MASK_FLAG)
                                            {
                                                headerMask = m_cache.get<uint8_t>(IPC_MSG_ADDITIONAL_HEADER_NEXT_HEADER_MASK_BIT_COUNT);
                                                additionalHeader = additionalHeader->createNext();
                                            }
                                            else
                                            {
                                                headerMask = 0;
                                                additionalHeader = nullptr;
                                            }
                                        }
                                        addHdrSize = msgAdditionalHeader->getSize();
                                    }
                                    const size_t payloadSize = msgSize - addHdrSize - IPC_MSG_HEADER_SIZE;
                                    std::vector<uint8_t> tmp(m_cache.getRest());
                                    std::vector<uint8_t> payload(tmp.begin(), tmp.begin() + payloadSize);
                                    BitStream newData(std::vector<uint8_t>(tmp.begin() + payloadSize, tmp.end()));
                                    std::swap(m_cache, newData);
                                    if (headerParamType && msgAdditionalHeader != nullptr)
                                    {
                                        if (headerParam & IPC_MSG_ADDITIONAL_HEADER_2_FLAG)
                                        {
                                            uint16_t calculateCRC = 0;
                                            if(payloadSize != 0)
                                            {
                                                calculateCRC = CRC::Calculate(&payload[0] , payload.size(), CRC::CRC_16_ARC());
                                            }
                                            if(msgAdditionalHeader->getValue_2() == calculateCRC)
                                            {
                                                releaseMsg(std::move(std::make_shared<FleXdIPCMsg>(msgAdditionalHeader, std::move(payload))));
                                            } else {
                                                //TODO - Acknowledge message is corrupted
                                                delete msgAdditionalHeader;
                                                rcvMsg();
                                            }
                                        } else {
                                            releaseMsg(std::move(std::make_shared<FleXdIPCMsg>(msgAdditionalHeader, std::move(payload))));
                                        }
                                    } else {
                                        releaseMsg(std::move(std::make_shared<FleXdIPCMsg>(headerParamType, headerParam, std::move(payload))));
                                    }
                                } else {
                                }
                            } else {
                                //TODO - Acknowledge message is corrupted
                                findNonCoruptedMessage(sizeof(uint8_t));
                            }
                        } else {
                            //TODO - Acknowledge message is corrupted
                            findNonCoruptedMessage(0);
                        }
                    } else {
                        //TODO - Acknowledge message is corrupted
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
                pSharedFleXdIPCMsg ret = std::move(m_queue.front());
                m_queue.pop();
                return std::move(ret);
            }

            void FleXdIPCBuffer::releaseMsg(pSharedFleXdIPCMsg&& msg)
            {
                if (msg) {
                    if (m_onMsg) {
                        m_onMsg(std::move(msg), m_fd);
                    } else {
                        if(m_bufferSize < m_maxBufferSize)
                        {
                            m_bufferSize += msg->getMsgSize();
                            m_queue.push(std::move(msg));
                        }
                    }
                }
                rcvMsg();
            }

            void FleXdIPCBuffer::findNonCoruptedMessage(uint16_t coruptedMsgSize)
            {
                unsigned counter = coruptedMsgSize * 8u;
                if (coruptedMsgSize < IPC_MSG_HEADER_SIZE)
                {
                    while (counter <= m_cache.getData().size() * 8u)
                    {
                        if (m_cache.getWithOffset<uint8_t>(counter, IPC_MSG_START_MSG_FLAG_BIT_COUNT) == IPC_MSG_START_MSG_FLAG)
                        {
                            if (m_cache.get<uint8_t>(IPC_MSG_HEADER_START_FLAG_BIT_COUNT) == IPC_MSG_START_HEADER_FLAG)
                            {
                                m_cache.get<uint8_t>(IPC_MSG_HEADER_PARAMETER_TYPE_BIT_COUNT);
                                m_cache.get<uint8_t>(IPC_MSG_HEADER_PARAMETER_BIT_COUNT);
                                if(m_cache.get<uint16_t>(IPC_MSG_MSG_SIZE_BIT_COUNT) <= m_cache.getData().size())
                                {
                                    if(m_cache.get<uint8_t>(IPC_MSG_HEADER_END_FLAG_BIT_COUNT) == IPC_MSG_END_HEADER_FLAG)
                                    {
                                        m_cache.getWithOffset<uint8_t>(counter - 8u); // last byte from previous message
                                        BitStream newData(m_cache.getRest());
                                        std::swap(m_cache, newData);
                                        rcvMsg();
                                        break;
                                    }
                                }
                            }
                        }
                        counter += 8u;
                    }
                }
            }

        } // namespace ipc
    } // namespace icl
} // namespace flexd
