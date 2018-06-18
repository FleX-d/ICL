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
 * File:   FleXdIPCConnector.h
 * Author: Adrian Peniak
 *
 * Created on November 27, 2017, 2:26 PM
 */

#ifndef FLEXDIPCCONNECTOR_H
#define FLEXDIPCCONNECTOR_H

#include <FleXdEpoll.h>
#include <FleXdIPCMsg.h>
#include <FleXdIPCProxy.h>
#include <cstdlib>
#include <cstdint>
#include <map>
#include <memory>
#include <queue>


namespace flexd {
    namespace icl {
        namespace ipc {
            class IPCConnector {
            public:
                explicit IPCConnector(uint32_t myID, FleXdEpoll& poller, bool genericPeersAllowed = false, bool garantedDelivery = false);
                IPCConnector(const IPCConnector&) = delete;
                IPCConnector& operator=(const IPCConnector&) = delete;
                virtual ~IPCConnector();

                uint32_t getMyID() const;
                bool initGenericServer();
                bool sendMsg(pSharedFleXdIPCMsg msg);
                bool sendMsg(pSharedFleXdIPCMsg msg, uint32_t peerID);
                bool addPeer(uint32_t peerID);
                bool removePeer(uint32_t peerID);
                bool mutePeer(uint32_t peerID);
                bool unMutePeer(uint32_t peerID);
                bool PeerStatus(uint32_t peerID) const;

            protected:
                virtual void receiveMsg(pSharedFleXdIPCMsg msg) = 0;
                virtual void onConnectPeer(uint32_t peerID, bool genericPeer) = 0;

            private:
                bool addClient(uint32_t clientID, const std::string& socPath);
                void onRcvMsg(pSharedFleXdIPCMsg msg, int fd);
                void onConnectClient(int fd);
                void onDisconnectClient(int fd);
                void onConnect(bool ret);
                void onDisconnect(bool ret);
                void handshake(int fd);
                void handshakeAck(uint32_t peerID, int fd);
                bool handshakeFin(uint32_t peerID1, uint32_t peerID2, int fd, bool& genericPeer);
                void flushQueue(uint32_t peerID);

            private:
                struct Client {
                    Client(bool active = false, int fd = -1, pSharedFleXdIPCProxy ptr = nullptr)
                    : m_active(active), m_fd(fd), m_ptr(std::move(ptr)), m_queue() {}
                    bool m_active;
                    int m_fd;
                    pSharedFleXdIPCProxy m_ptr;
                    std::queue<pSharedFleXdIPCMsg> m_queue;
                };
                const uint32_t m_myID;
                FleXdEpoll& m_poller;
                const bool m_garantedDelivery;
                const bool m_genericPeersAllowed;
                pSharedFleXdIPCProxy m_server;
                std::map<uint32_t, Client> m_clients;
            };

        } // namespace ipc
    } // namespace icl
} // namespace flexd
#endif /* FLEXDIPCCONNECTOR_H */
