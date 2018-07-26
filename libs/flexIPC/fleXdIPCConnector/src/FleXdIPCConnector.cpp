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
#include <unistd.h>

#define CLIENT_RECONNECT_RETRY_COUNT    5u
#define CLIENT_RECONNECT_TIMEOUT        1L

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
                    return initAsServer();
                }
                return m_genericPeersAllowed && m_server;
            }

            bool  IPCConnector::initAsServer(){
                const std::string myPath = FLEXDIPCUDSPATH + std::to_string(m_myID);
                m_server = std::make_shared<FleXdIPCProxyBuilder<FleXdUDSServer> >(myPath, m_poller);
                m_server->setOnSndMsg([this](pSharedFleXdIPCMsg msg, int){ this->onAfterSndMsg(msg); });
                m_server->setOnRcvMsg([this](pSharedFleXdIPCMsg msg, int fd){ this->onRcvMsg(msg, fd); });
                m_server->setOnConnectClient([this](int fd){ this->onConnectClient(fd); });
                m_server->setOnDisconnectClient([this](int fd){ this->onDisconnectClient(fd); });
                return m_server->init();
            }

            bool IPCConnector::addClient(uint32_t clientID, const std::string& socPath) {
                auto it = m_clients.find(clientID);
                if(it == m_clients.end()) {
                    auto client = std::make_shared<FleXdIPCProxyBuilder<FleXdUDSClient> >(socPath, m_poller);
                    client->setOnSndMsg([this](pSharedFleXdIPCMsg msg, int){ this->onAfterSndMsg(msg); });
                    client->setOnRcvMsg([this](pSharedFleXdIPCMsg msg, int fd){ this->onRcvMsg(msg, fd); });
                    client->setOnConnect([this](bool ret){ this->onConnect(ret); });
                    client->setOnDisconnect([this](int fd ){ this->onDisconnect(fd); });
                    if(!client->init()) {
                        return false;
                    }
                    const auto& ret = m_clients.insert(std::make_pair(clientID, Client(true, client->getFd(), client)));
                    return ret.second;
                } else if (!it->second.m_active) {
                    auto client = std::make_shared<FleXdIPCProxyBuilder<FleXdUDSClient> >(socPath, m_poller);
                    client->setOnSndMsg([this](pSharedFleXdIPCMsg msg, int){ this->onAfterSndMsg(msg); });
                    client->setOnRcvMsg([this](pSharedFleXdIPCMsg msg, int fd){ this->onRcvMsg(msg, fd); });
                    client->setOnConnect([this](bool ret){ this->onConnect(ret); });
                    client->setOnDisconnect([this](int fd ){ this->onDisconnect(fd); });
                    if(!client->init()) {
                        return false;
                    }
                    it->second.m_active = true;
                    it->second.m_fd = client->getFd();
                    it->second.m_ptr = client;
                    return true;
                }
                return false;
            }

            bool IPCConnector::addPeer(uint32_t peerID)
            {
                const std::string socPath = FLEXDIPCUDSPATH + std::to_string(peerID);
                if(checkIfFileExist(socPath)) {
                    return addClient(peerID, socPath);
                } else {
                    if(!m_server) {
                        if (!initAsServer()){
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
                //TODO
                return false;
            }

            bool IPCConnector::unMutePeer(uint32_t peerID)
            {
                //TODO
                return false;
            }

            bool IPCConnector::sendMsg(pSharedFleXdIPCMsg msg, uint32_t peerID) {
                if(msg) {
                    auto it = m_clients.find(peerID);
                    if(it != m_clients.end()) {
                        bool handshakeMsg = false;
                        if(!msg->getHeaderParamType()) {
                            switch(msg->getHeaderParam()) {
                                case FleXdIPCMsgTypes::Enum::Handshake:
                                case FleXdIPCMsgTypes::Enum::HandshakeAck:
                                case FleXdIPCMsgTypes::Enum::HandshakeSuccess:
                                case FleXdIPCMsgTypes::Enum::HandshakeSuccessGeneric:
                                    //All handshake messages will be discarded after the first successful handshake
                                    if (it->second.m_handshakeDone) {
                                        return true;
                                    }
                                    handshakeMsg = true;
                                    break;
                                case FleXdIPCMsgTypes::Enum::IPCMsg:
                                case FleXdIPCMsgTypes::Enum::Generic:
                                default:
                                    break;
                            }
                        }
                        if(it->second.m_active && (it->second.m_handshakeDone || handshakeMsg) && it->second.m_ptr && (it->second.m_fd != -1)) {
                            it->second.m_ptr->sndMsg(msg, it->second.m_fd);
                            return true;
                        } else if (!handshakeMsg) {
                            it->second.m_queue.push(std::move(msg));
                        }
                        //Try to reconnect if sending as generic client
                        //TODO solve possible Denial of Service
                        if (!it->second.m_active && it->second.m_generic && it->second.m_ptr && it->second.m_ptr->getIPC() && it->second.m_ptr->getIPC()->isClient()) {
                            addClient(it->first, FLEXDIPCUDSPATH + std::to_string(it->first));
                        }
                    }
                }
                return false;
            }

            void IPCConnector::onRcvMsg(pSharedFleXdIPCMsg msg, int fd) {
                onBeforeRcvMsg(msg);
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
                                const uint32_t peer1 = hello.get<uint32_t>();
                                const uint32_t peer2 = hello.get<uint32_t>();
                                handshakeFin(peer1, peer2, fd);
                                break;
                            }
                            case FleXdIPCMsgTypes::Enum::HandshakeSuccess: { //client side
                                BitStream hello(msg->releasePayload());
                                handshakeFinAck(hello.get<uint32_t>(), false);
                                break;
                            }
                            case FleXdIPCMsgTypes::Enum::HandshakeSuccessGeneric: { //client side
                                BitStream hello(msg->releasePayload());
                                handshakeFinAck(hello.get<uint32_t>(), true);
                                break;
                            }
                            case FleXdIPCMsgTypes::Enum::IPCMsg:
                            case FleXdIPCMsgTypes::Enum::Generic: {
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
                        ref.second.m_handshakeDone = false;
                        break;
                    }
                }
            }

            void IPCConnector::onConnect(bool ret) {
                // TODO
            }

            void IPCConnector::onDisconnect(int fd) {
                if(fd > 0 ) {
                    for(auto& ref : m_clients) {
                        if(ref.second.m_fd == fd) {
                            ref.second.m_active = false;
                            ref.second.m_handshakeDone = false;
                            ref.second.m_fd = -1;
                            //Generic client reconnects when sending message
                            if(ref.second.m_generic) {
                                break;
                            }
                            auto pair = m_reconnections.emplace(std::make_pair(ref.first, Reconnection(m_poller, CLIENT_RECONNECT_TIMEOUT)));
                            if(pair.second) {
                                pair.first->second.m_timer.setOnTimer(
                                    [this, peerID = ref.first]() {
                                        //TODO optimization -> it should be possible to use pair.first iterator directly
                                        auto it = m_reconnections.find(peerID);
                                        if(it != m_reconnections.end()) {
                                            if(++it->second.m_retryCount <= CLIENT_RECONNECT_RETRY_COUNT) {
                                                if(addClient(peerID, FLEXDIPCUDSPATH + std::to_string(peerID))) {
                                                    it->second.m_timer.stop();
                                                    it->second.m_retryCount = 0;
                                                    m_reconnections.erase(peerID);
                                                }
                                            } else {
                                                removePeer(peerID);
                                                if(addPeer(peerID)) {
                                                    it->second.m_timer.stop();
                                                    m_reconnections.erase(peerID);
                                                }
                                            }
                                        }
                                    }
                                );
                                pair.first->second.m_timer.start();
                            }
                            ref.second.m_ptr.reset();
                            break;
                        }
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

            void IPCConnector::handshakeFin(uint32_t peerID1, uint32_t peerID2, int fd) {
                BitStream hello;
                hello.put<uint32_t>(peerID1);
                const uint32_t peerID = (peerID1 == m_myID) ? peerID2 : peerID1;
                auto it = m_clients.find(peerID);
                if(it != m_clients.end()) {
                    it->second.m_active = true;
                    it->second.m_fd = fd;
                    auto msg = std::make_shared<FleXdIPCMsg>(it->second.m_generic ? FleXdIPCMsgTypes::Enum::HandshakeSuccessGeneric : FleXdIPCMsgTypes::Enum::HandshakeSuccess, hello.releaseData());
                    it->second.m_ptr->sndMsg(msg, fd);
                    it->second.m_handshakeDone = true;
                    onConnectPeer(peerID, it->second.m_generic);
                    flushQueue(peerID);
                } else if(m_genericPeersAllowed) {
                    auto ret = m_clients.insert(std::make_pair(peerID, Client(true, fd, m_server, true)));
                    if(ret.second) {
                        auto msg = std::make_shared<FleXdIPCMsg>(FleXdIPCMsgTypes::Enum::HandshakeSuccessGeneric, hello.releaseData());
                        ret.first->second.m_ptr->sndMsg(msg, fd);
                        ret.first->second.m_handshakeDone = true;
                        onConnectPeer(peerID, true);
                        flushQueue(peerID);
                    } else {
                        // TODO send handshake again?
                    }
                } else {
                    // TODO
                }
            }

            void IPCConnector::handshakeFinAck(uint32_t peerID, bool genericPeer) {
                auto it = m_clients.find(peerID);
                if(it != m_clients.end()) {
                    it->second.m_generic = genericPeer;
                    it->second.m_handshakeDone = true;
                    onConnectPeer(peerID, genericPeer);
                    flushQueue(peerID);
                } else {
                    // TODO
                }
            }

            void IPCConnector::flushQueue(uint32_t peerID) {
                auto it = m_clients.find(peerID);
                if(it != m_clients.end()) {
                    while(!it->second.m_queue.empty()) {
                        bool ret = sendMsg(it->second.m_queue.front(), peerID);
                        it->second.m_queue.pop();
                        if (!ret) {
                            break;
                        }
                    }
                }
            }

        } // namespace ipc
    } // namespace icl
} // namespace flexd
