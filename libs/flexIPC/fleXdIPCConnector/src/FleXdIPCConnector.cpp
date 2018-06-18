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
 * File:   IPCConnector.cpp
 * Author: Adrian Peniak
 *
 * Created on November 27, 2017, 2:26 PM
 */

#include "FleXdIPCConnector.h"
#include "FleXdIPCCommon.h"
#include "FleXdUDSServer.h"
#include "FleXdUDSClient.h"
#include "FleXdIPCProxyBuilder.h"
#include "FleXdIPCMsgTypes.h"
#include "BitStream.h"
#include <sys/types.h>
#include <vector>
#include <cstdint>

namespace flexd {
    namespace icl {
        namespace ipc {
            IPCConnector::IPCConnector(uint32_t myID, FleXdEpoll& poller, bool genericPeersAllowed /*= false*/, bool garantedDelivery /*false*/)
            : m_myID(myID),
              m_poller(poller),
              m_garantedDelivery(garantedDelivery),
              m_genericPeersAllowed(genericPeersAllowed),
              m_server(nullptr),
              m_clients()
            {
                makeParentDir(FLEXDIPCUDSPATH + std::to_string(m_myID));
            }

            IPCConnector::~IPCConnector()
            {

            }

            uint32_t IPCConnector::getMyID() const {
                return m_myID;
            }

            bool IPCConnector::initGenericServer() {
                if(m_genericPeersAllowed && !m_server) {
                    const std::string myPath = FLEXDIPCUDSPATH + std::to_string(m_myID);
                    m_server = std::make_shared<FleXdIPCProxyBuilder<FleXdUDSServer> >(myPath, m_poller);
                    m_server->setOnRcvMsg([this](pSharedFleXdIPCMsg msg, int fd){ this->onRcvMsg(msg, fd); });
                    m_server->setOnConnectClient([this](int fd){ this->onConnectClient(fd); });
                    m_server->setOnDisconnectClient([this](int fd){ this->onDisconnectClient(fd); });
                    return m_server->init();
                }
                return m_genericPeersAllowed && m_server;
            }

            bool IPCConnector::addPeer(uint32_t peerID)
            {
                const std::string socPath = FLEXDIPCUDSPATH + std::to_string(peerID);
                if(checkIfFileExist(socPath)) {
                    return addClient(peerID, socPath);
                } else {
                    if(!m_server) {
                        const std::string myPath = FLEXDIPCUDSPATH + std::to_string(m_myID);
                        m_server = std::make_shared<FleXdIPCProxyBuilder<FleXdUDSServer> >(myPath, m_poller);
                        m_server->setOnRcvMsg([this](pSharedFleXdIPCMsg msg, int fd){ this->onRcvMsg(msg, fd); });
                        m_server->setOnConnectClient([this](int fd){ this->onConnectClient(fd); });
                        m_server->setOnDisconnectClient([this](int fd){ this->onDisconnectClient(fd); });
                        if(!m_server->init()) {
                            return false;
                        }
                    }
                    auto ret = m_clients.insert(std::make_pair(peerID, Client(false, -1, m_server)));
                    return ret.second;
                }
                return false;
            }

            bool IPCConnector::removePeer(uint32_t peerID)
            {
                if(m_myID != peerID) {
                    return m_clients.erase(peerID) != 0;
                }
                return false;
            }
            bool IPCConnector::mutePeer(uint32_t peerID)
            {
                return false;
            }

            bool IPCConnector::unMutePeer(uint32_t peerID)
            {
                return false;
            }

            bool IPCConnector::sendMsg(pSharedFleXdIPCMsg msg) {
                if(msg) {
                    return sendMsg(msg, msg->getAdditionalHeader()->getValue_5());
                }
                return false;
            }

            bool IPCConnector::sendMsg(pSharedFleXdIPCMsg msg, uint32_t peerID) {
                if(msg) {
                    auto it = m_clients.find(peerID);
                    if (it != m_clients.end()) {
                        if((it->second.m_active)&&(it->second.m_ptr)&&(it->second.m_fd != -1)) {
                            it->second.m_ptr->sndMsg(msg, it->second.m_fd);
                            return true;
                        } else {
                            it->second.m_queue.push(std::move(msg));
                        }
                    }
                }
                return false;
            }

            bool IPCConnector::addClient(uint32_t clientID, const std::string& socPath) {
                if(m_clients.count(clientID) == 0) {
                    auto client = std::make_shared<FleXdIPCProxyBuilder<FleXdUDSClient> >(socPath, m_poller);
                    client->setOnRcvMsg([this](pSharedFleXdIPCMsg msg, int fd){ this->onRcvMsg(msg, fd); });
                    client->setOnConnect([this](bool ret){ this->onConnect(ret); });
                    client->setOnDisconnect([this](bool ret){ this->onDisconnect(ret); });
                    if(!client->init()) {
                        return false;
                    }
                    const auto& ret = m_clients.insert(std::make_pair(clientID, Client(true, client->getFd(), client)));
                    return ret.second;
                }
                return false;
            }

            void IPCConnector::onRcvMsg(pSharedFleXdIPCMsg msg, int fd) {
                if(msg) {
                    if(msg->getHeaderParamType() == true) {
                        receiveMsg(msg);
                    } else {
                        switch(msg->getHeaderParam()) {
                            case FleXdIPCMsgTypes::Enum::Handshake: { //client side
                                BitStream hello(msg->releasePayload());
                                handshakeAck(hello.get<uint32_t>(), fd);
                                break;
                            }
                            case FleXdIPCMsgTypes::Enum::HandshakeAck: { //server side
                                BitStream hello(msg->releasePayload());
                                bool genericPeer = false;
                                const uint32_t peer1 = hello.get<uint32_t>();
                                const uint32_t peer2 = hello.get<uint32_t>();
                                if (handshakeFin(peer1, peer2, fd, genericPeer)) {
                                    onConnectPeer(peer1, genericPeer);
                                }
                                break;
                            }
                            case FleXdIPCMsgTypes::Enum::HandshakeSuccess: { //client side
                                BitStream hello(msg->releasePayload());
                                const uint32_t peer = hello.get<uint32_t>();
                                onConnectPeer(peer, false);
                                flushQueue(peer);
                                break;
                            }
                            case FleXdIPCMsgTypes::Enum::HandshakeSuccessGeneric: { //client side
                                BitStream hello(msg->releasePayload());
                                const uint32_t peer = hello.get<uint32_t>();
                                onConnectPeer(peer, true);
                                flushQueue(peer);
                                break;
                            }
                            case FleXdIPCMsgTypes::Enum::IPCMsg: {
                                receiveMsg(msg);
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
            }

            void IPCConnector::onConnectClient(int fd) {
                if(m_server) {
                    handshake(fd);
                }
            }

            void IPCConnector::onDisconnectClient(int fd) {
                for(auto& ref : m_clients) {
                    if(ref.second.m_fd == fd) {
                        ref.second.m_active = false;
                    }
                }
            }

            void IPCConnector::onConnect(bool ret) {
                // TODO
            }

            void IPCConnector::onDisconnect(bool ret) {
                if(!ret) {
                    if(!m_server) {
                        // TODO switch to server mode
                    } else {
                        // TODO remove from list
                    }
                }
            }

            void IPCConnector::handshake(int fd) {
                BitStream hello;
                hello.put<uint32_t>(m_myID);
                auto msg = std::make_shared<FleXdIPCMsg>(FleXdIPCMsgTypes::Enum::Handshake, hello.releaseData());
                m_server->sndMsg(msg, fd);
            }

            void IPCConnector::handshakeAck(uint32_t peerID, int fd) {
                auto it = m_clients.find(peerID);
                if(it != m_clients.end()) {
                    BitStream hello;
                    hello.put<uint32_t>(peerID);
                    hello.put<uint32_t>(getMyID());
                    auto msg = std::make_shared<FleXdIPCMsg>(FleXdIPCMsgTypes::Enum::HandshakeAck, hello.releaseData());
                    it->second.m_ptr->sndMsg(msg, fd);
                } else {
                    // TODO
                }
            }

            bool IPCConnector::handshakeFin(uint32_t peerID1, uint32_t peerID2, int fd, bool& genericPeer) {
                BitStream hello;
                genericPeer = false;
                hello.put<uint32_t>(peerID1);
                const uint32_t peerID = (peerID1 == m_myID) ? peerID2 : peerID1;
                auto it = m_clients.find(peerID);
                if(it != m_clients.end()) {
                    it->second.m_active = true;
                    it->second.m_fd = fd;
                    auto msg = std::make_shared<FleXdIPCMsg>(FleXdIPCMsgTypes::Enum::HandshakeSuccess, hello.releaseData());
                    it->second.m_ptr->sndMsg(msg, fd);
                    flushQueue(peerID);
                    return true;
                } else if(m_genericPeersAllowed) {
                    auto ret = m_clients.insert(std::make_pair(peerID, Client(true, fd, m_server)));
                    if(ret.second) {
                        auto msg = std::make_shared<FleXdIPCMsg>(FleXdIPCMsgTypes::Enum::HandshakeSuccessGeneric, hello.releaseData());
                        ret.first->second.m_ptr->sndMsg(msg, fd);
                        flushQueue(peerID);
                        genericPeer = true;
                        return true;
                    } else {
                        // TODO send handshake again?
                    }
                } else {
                    // TODO
                }
                return false;
            }

            void IPCConnector::flushQueue(uint32_t peerID) {
                auto it = m_clients.find(peerID);
                if(it != m_clients.end()) {
                    while(!it->second.m_queue.empty()) {
                        const bool ret = sendMsg(it->second.m_queue.front());
                        it->second.m_queue.pop();
                        if(!ret) break;
                    }
                }
            }

        } // namespace ipc
    } // namespace icl
} // namespace flexd
