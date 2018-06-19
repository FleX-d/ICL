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
 * File:   FleXdIPCProxy.cpp
 * Author: Adrian Peniak
 *
 * Created on May 31, 2018, 7:49 PM
 */

#include "FleXdIPCProxy.h"

namespace flexd {
    namespace icl {
        namespace ipc {

            FleXdIPCProxy::FleXdIPCProxy(pSharedFleXdIPC ipc)
            : m_ipc(ipc){
            }

            FleXdIPCProxy::~FleXdIPCProxy() {
            }

            int FleXdIPCProxy::getFd() const {
                return m_ipc->getFd();
            }

            pSharedFleXdIPC FleXdIPCProxy::getIPC() {
                return m_ipc;
            }

            bool FleXdIPCProxy::init() {
                const bool ret = m_ipc->init();
                if (m_onInit) {
                    m_onInit(ret);
                } else {
                    m_ipc->onInit(ret);
                }
                return ret;
            }

            bool FleXdIPCProxy::connect() {
                const bool ret = m_ipc->connect();
                if (m_onConnect) {
                    m_onConnect(ret);
                } else {
                    m_ipc->onConnect(ret);
                }
                return ret;
            }

            bool FleXdIPCProxy::disconnect() {
                const bool ret = m_ipc->disconnect();
                if (m_onDisconnect) {
                    m_onDisconnect(ret);
                } else {
                    m_ipc->onDisconnect(ret);
                }
                return ret;
            }

            void FleXdIPCProxy::sndMsg(pSharedFleXdIPCMsg msg, int fd) {
                m_ipc->sndMsg(msg, fd);
                if(m_onSndMsg) {
                    m_onSndMsg(msg, fd);
                } else {
                    m_ipc->onSndMsg(msg, fd);
                }
            }

            void FleXdIPCProxy::connectClient(int fd) {
                m_ipc->connectClient(fd);
                if(m_onConnectClient) {
                    m_onConnectClient(fd);
                } else {
                    m_ipc->onConnectClient(fd);
                }
            }

            void FleXdIPCProxy::disconnectClient(int fd) {
                m_ipc->disconnectClient(fd);
                if(m_onDisconnectClient) {
                    m_onDisconnectClient(fd);
                } else {
                    m_ipc->onDisconnectClient(fd);
                }
            }

            void FleXdIPCProxy::rcvMsg(pSharedFleXdIPCMsg msg, int fd) {
                m_ipc->rcvMsg(msg, fd);
                if(m_onRcvMsg) {
                    m_onRcvMsg(msg, fd);
                } else {
                    m_ipc->onRcvMsg(msg, fd);
                }
            }

            void FleXdIPCProxy::rcvEvent(FleXdEpoll::Event e) {
                m_ipc->rcvEvent(e);
                if(m_onRcvEvent) {
                    m_onRcvEvent(e);
                } else {
                    m_ipc->onRcvEvent(e);
                }
            }

            void FleXdIPCProxy::setOnInit(std::function<void(bool)> fcn) {
                m_onInit = fcn;
            }

            void FleXdIPCProxy::setOnConnect(std::function<void(bool)> fcn) {
                m_onConnect = fcn;
            }

            void FleXdIPCProxy::setOnDisconnect(std::function<void(bool)> fcn) {
                m_onDisconnect = fcn;
            }

            void FleXdIPCProxy::setOnConnectClient(std::function<void(int)> fcn) {
                m_onConnectClient = fcn;
            }

            void FleXdIPCProxy::setOnDisconnectClient(std::function<void(int)> fcn) {
                m_onDisconnectClient = fcn;
            }

            void FleXdIPCProxy::setOnSndMsg(std::function<void(pSharedFleXdIPCMsg, int)> fcn) {
                m_onSndMsg = fcn;
            }

            void FleXdIPCProxy::setOnRcvMsg(std::function<void(pSharedFleXdIPCMsg, int)> fcn) {
                m_onRcvMsg = fcn;
            }

            void FleXdIPCProxy::setOnRcvEvent(std::function<void(FleXdEpoll::Event)> fcn) {
                m_onRcvEvent = fcn;
            }

        } // namespace ipc
    } // namespace icl
} // namespace flexd

