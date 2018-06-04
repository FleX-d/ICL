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
#include "FleXdUDSServer.h"
#include "FleXdUDSClient.h"
#include "FleXdIPCProxyBuilder.h"
#include "FleXdIPCMsgTypes.h"
#include "BitStream.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <cstdint>

namespace {
    bool checkIfFileExist(const std::string& filePath) {
        struct stat info;
        return (stat(filePath.c_str(), &info) == 0);
    }
    
    bool makeDir(const std::string& dirPath) {
        if (dirPath.empty()) return false;
        return (mkdir(dirPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO | S_IXOTH) == 0);
    }
    
    std::string cutLastPathElement(const std::string& dirPath) {
        return dirPath.substr(0, dirPath.find_last_of("\\/"));
    }
    
    bool makeParentDir(const std::string& filePath) {
        std::string dir = cutLastPathElement(filePath);
        if(checkIfFileExist(dir)) return true;
        if(!checkIfFileExist(cutLastPathElement(dir))) makeParentDir(dir);
        return makeDir(dir);
    }
}


namespace flexd {
    namespace icl {
        namespace ipc {          
            IPCConnector::IPCConnector(uint32_t myID, FleXdEpoll& poller, bool garantedDelivery /*false*/)
            : m_myID(myID),
              m_poller(poller),
              m_garantedDelivery(garantedDelivery),
              m_server(nullptr),
              m_clients()
            {
                makeParentDir(FLEXDIPCUDSPATH + std::to_string(m_myID));
            }
            
            IPCConnector::~IPCConnector()
            {
                
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
                    auto it = m_clients.find(peerID);
                    if (it != m_clients.end()) {
                        m_clients.erase(it);
                        return true;
                    }
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
                auto it = m_clients.find(msg->getAdditionalHeader()->getValue_5());
                if (it != m_clients.end()) {
                    if((it->second.m_active)&&(it->second.m_ptr)&&(it->second.m_fd != -1)) {
                        it->second.m_ptr->sndMsg(msg, it->second.m_fd);
                        return true;
                    }
                }
                return false;
            }
            
            bool IPCConnector::addClient(uint32_t clientID, const std::string& socPath) {
                auto it = m_clients.find(clientID);
                if (it == m_clients.end()) {
                    auto client = std::make_shared<FleXdIPCProxyBuilder<FleXdUDSClient> >(socPath, m_poller);
                    client->setOnRcvMsg([this](pSharedFleXdIPCMsg msg, int fd){ this->onRcvMsg(msg, fd); });
                    client->setOnConnectClient([this](int fd){ this->onConnectClient(fd); });
                    client->setOnDisconnectClient([this](int fd){ this->onDisconnectClient(fd); });
                    if(!client->init()) {
                        return false;
                    }
                    auto ret = m_clients.insert(std::make_pair(clientID, Client(true, client->getFd(), client)));
                    return ret.second;
                    }
                return false;
            }
            
            void IPCConnector::onRcvMsg(pSharedFleXdIPCMsg msg, int fd) {
                if(msg->getHeaderParamType() == true) {
                    receiveMsg(msg);
                } else {
                    switch(msg->getHeaderParam()) {
                        case FleXdIPCMsgTypes::Enum::Handshake: {
                            BitStream hello(msg->releasePayload());
                            handshakeAck(hello.get<uint32_t>(), fd);
                            break;
                        }
                        case FleXdIPCMsgTypes::Enum::HandshakeAck: {
                            BitStream hello(msg->releasePayload());
                            handshakeFin(hello.get<uint32_t>(), fd);
                            break;  
                        }
                        case FleXdIPCMsgTypes::Enum::HandshakeSuccess: {
                            BitStream hello(msg->releasePayload());
                            onConnectPeer(hello.get<uint32_t>());
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
            
            void IPCConnector::handshake(int fd) {
                BitStream hello;
                hello.put<uint32_t>(m_myID);
                auto msg = std::make_shared<FleXdIPCMsg>(false, FleXdIPCMsgTypes::Enum::Handshake, hello.releaseData());
                m_server->sndMsg(msg,fd);
            }
            
            void IPCConnector::handshakeAck(uint32_t peerID, int fd) {
                auto it = m_clients.find(peerID);
                if (it != m_clients.end()) {
                    BitStream hello;
                    hello.put<uint32_t>(peerID);
                    auto msg = std::make_shared<FleXdIPCMsg>(false, FleXdIPCMsgTypes::Enum::HandshakeAck, hello.releaseData());
                    it->second.m_ptr->sndMsg(msg, fd);
                } else {
                    // TODO
                }
            }
            
            void IPCConnector::handshakeFin(uint32_t peerID, int fd) {
                BitStream hello;
                hello.put<uint32_t>(peerID);
                if(m_myID == peerID) {
                    auto msg = std::make_shared<FleXdIPCMsg>(false, FleXdIPCMsgTypes::Enum::HandshakeSuccess, hello.releaseData());
                    m_server->sndMsg(msg, fd);
                } else {
                    auto it = m_clients.find(peerID);
                    if (it != m_clients.end()) {
                        it->second.m_active = true;
                        it->second.m_fd = fd;
                        auto msg = std::make_shared<FleXdIPCMsg>(false, FleXdIPCMsgTypes::Enum::HandshakeSuccess, hello.releaseData());
                        it->second.m_ptr->sndMsg(msg, fd);
                    } else {
                        // TODO
                    }
                }
            }
            
        } // namespace ipc
    } // namespace icl
} // namespace flexd