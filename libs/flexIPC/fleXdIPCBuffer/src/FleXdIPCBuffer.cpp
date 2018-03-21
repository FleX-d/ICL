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
#include "FleXdIPCFactory.h"
#include <iostream>
#include <unistd.h>
#include <vector>

namespace flexd {
    namespace ilc {
        namespace epoll {
            

            FleXdIPCBuffer::FleXdIPCBuffer(size_t maxBufferSize)
            : m_maxBufferSize(maxBufferSize),
              m_queue(),
              m_onMsg(nullptr), 
              m_factory(new FleXdIPCFactory([this](pSharedFleXdIPCMsg msg){releaseMsg(msg);}))
            {
                
            }

            FleXdIPCBuffer::FleXdIPCBuffer(std::function<void(pSharedFleXdIPCMsg msg)> onMsg, size_t maxBufferSize)
            : m_maxBufferSize(maxBufferSize),
              m_queue(),
              m_onMsg(onMsg),
              m_factory(new FleXdIPCFactory([this](pSharedFleXdIPCMsg msg){releaseMsg(msg);}))
            {
            }
            
            FleXdIPCBuffer::~FleXdIPCBuffer()
	    {
	    }
                        
            void FleXdIPCBuffer::rcvMsg(pSharedArray8192& data, size_t size) {
                if(m_factory) {
                    m_factory->parseData(data,size);
		} else {
                    // TODO error
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
                if(msg->isComplete())
                {
                    if(m_onMsg)
                    {
                        m_onMsg(std::move(msg));
                    } else {
                        if(m_bufferSize < m_maxBufferSize)
                        {
                            m_queue.push(std::move(msg));
                            m_bufferSize += msg->getMsgSize();
                        } else {
                            m_onMsg(nullptr);
                        }       
                    }
                } else {
                     m_onMsg(nullptr);
                }
            }
        } // namespace epoll
    } // namespace ilc
} // namespace flexd