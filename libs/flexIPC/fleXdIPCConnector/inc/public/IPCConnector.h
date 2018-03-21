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
 * File:   IPCConnector.h
 * Author: Adrian Peniak
 *
 * Created on November 27, 2017, 2:26 PM
 */

#ifndef IPCCONNECTOR_H
#define IPCCONNECTOR_H

#include <cstdlib>
#include <cstdint>
#include <list>
#include <memory>

///*tmp decalreation*/
//typedef __int128 int128_t;
//typedef unsigned __int128 uint128_t;
//typedef uint128_t uuid_t;
///*tmp decalreation - end*/
//
namespace flexd {
    namespace icl {
        namespace ipc {
//            
//            class IPCConnector {
//            public:
//                struct PeerUuid {
//                    uuid_t uuid;
//                    uint32_t magicNumber;
//                };
//            public:
//                /**
//                 * 
//                 * @param yourUuid
//                 * @param magicNumber
//                 */
//                IPCConnector(uuid_t yourUuid, uint32_t magicNumber = 0);
//                /**
//                 * 
//                 */
//                virtual ~IPCConnector();
//                /**
//                 * 
//                 * @param yourUuid
//                 * @param peerUuids
//                 * @return 
//                 */
//                bool start(/*TODO*/);
//                bool stop(/*TODO*/);
//                /**
//                 * 
//                 * @param peerUuid
//                 * @return 
//                 */
//                bool addPeer(uuid_t peerUuid, uint32_t magicNumber = 0);
//                /**
//                 * 
//                 * @param peerUuid
//                 * @return 
//                 */
//                bool addPeer(PeerUuid peerUuid);
//                /**
//                 * 
//                 * @param peerUuids
//                 * @return 
//                 */
//                bool addPeer(std::list<PeerUuid> peerUuids);
//                /**
//                 * 
//                 * @param peerUuid
//                 * @return 
//                 */
//                bool removePeer(uuid_t peerUuid);
//                /**
//                 * 
//                 * @param peerUuids
//                 * @return 
//                 */
//                bool removePeer(std::list<PeerUuid> peerUuids);
//                /**
//                 * 
//                 * @param peerUuid peer uuid number.
//                 * @param muteTime time for mute peer. Zero is equal forever.
//                 * @return true if operation pass successful (peer was removed), false otherwise.
//                 */
//                bool mutePeer(uuid_t peerUuid, uint32_t muteTime = 0);
//                /**
//                 * 
//                 * @param peerUuid
//                 * @return 
//                 */
//                bool unMutePeer(uuid_t peerUuid);
//                
//                IPCConnector(const IPCConnector&) = delete;
//                IPCConnector& operator=(const IPCConnector&) = delete;
//                
//            protected:
//                /**
//                 * 
//                 * @param msg
//                 * @return 
//                 */
//                bool sendMsg(IPCMessage_t msg, bool blocking = false);
//                /**
//                 * 
//                 * @return 
//                 */
//                virtual IPCMessage_t receiveMsg() = 0;               
//                
//                
//            private:
//                uuid_t m_yourUuid;
//                std::list<PeerUuid> m_peerUuids;
//                uint32_t m_magicNumber;
//            };
        } // namespace ipc
    } // namespace icl
} // namespace flexd
#endif /* IPCCONNECTOR_H */