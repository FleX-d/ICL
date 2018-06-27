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
 * File:   FleXdIPCConnectorTest.cpp
 *
 * Author:Jakub Pekar
 *
 * Created on June 27, 2018, 10:02 AM
 */

#include "FleXdIPCConnector.h"
#include "FleXdEpoll.h"
#include "FleXdIPCCommon.h"
#include <gtest/gtest.h>
#include <future>
#include <chrono>
#include <thread>

#define IPC1_PEER_ID 10000
#define IPC2_PEER_ID 20000
#define IPC3_PEER_ID 30000
using namespace flexd::icl::ipc;

namespace {
    FleXdEpoll poller(10);
    uint32_t myID = 132;
    bool genericPeersAllowed = true;
    bool garantedDelivery = false;

    class IPCTest: public IPCConnector {
    public:
        IPCTest(uint32_t myID, FleXdEpoll& poller, bool genericPeersAllowed = false, bool garantedDelivery = false)
        :IPCConnector(myID,poller,genericPeersAllowed,garantedDelivery){
            
        }
        ~IPCTest() = default;
        void receiveMsg(flexd::icl::ipc::pSharedFleXdIPCMsg msg) override {
            
        }
        void onConnectPeer(uint32_t peerID, bool genericPeer) override {
            
        }

    };

    IPCTest testConnector1(myID, poller);
    IPCTest testConnector2(myID+2, poller);
    IPCTest testConnector5(myID+5, poller,genericPeersAllowed,garantedDelivery);
    /**
     * Basic Test of functions
     */

    bool swapClientServer(IPCTest* test){
        delete test;
        return true; 
    }
    
    bool createSingleServer(IPCTest* test,uint32_t peerid){
        test = new IPCTest(peerid,poller);
        test->addPeer(IPC2_PEER_ID);
        poller.loop();
        return false;
    }
    
    bool createSingleIPCClient(IPCTest* test,uint32_t peerid){
        test = new IPCTest(peerid,poller);
        test->addPeer(IPC1_PEER_ID);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return false;
    }

    TEST(IPCConnector, getMyID){
        EXPECT_EQ(myID, testConnector1.getMyID());
    }
    TEST(IPCConnector, addPeer){
        EXPECT_TRUE(testConnector1.addPeer(myID+2));
    }

    TEST(IPCConnector, serverInitialization){
        std::string socPath = "/tmp/FleXd/shared/ipc/uds/" + std::to_string(myID+10);
        EXPECT_FALSE(checkIfFileExist(socPath));
        IPCTest testIPC(myID+10, poller);
        testIPC.addPeer(myID+4);
        EXPECT_TRUE(checkIfFileExist(socPath));
    }

    TEST(IPCConnector, initialization_clientToServer){
        uint32_t serverID = 11111;
        uint32_t clientID = 12000;
        IPCTest* IPCConn1Server = new IPCTest(serverID,poller);
        IPCTest* IPCConn2Client = nullptr;
        IPCConn1Server->addPeer(clientID);
        auto handlerServer = std::async(createSingleServer,IPCConn1Server,serverID);
        auto handlerClient = std::async(createSingleIPCClient,IPCConn2Client,clientID);
        EXPECT_TRUE(checkIfFileExist("/tmp/FleXd/shared/ipc/uds/" + std::to_string(serverID)));
        
        /*
        std::string socPath = "/tmp/FleXd/shared/ipc/uds/" + std::to_string(myID+3);
        EXPECT_FALSE(checkIfFileExist(socPath));
        IPCTest* testConnector3 = new IPCTest(myID+3, poller);
        EXPECT_TRUE(testConnector3->addPeer(myID+4));
        EXPECT_TRUE(checkIfFileExist("/tmp/FleXd/shared/ipc/uds/" + std::to_string(myID+3)));
        IPCTest testConnector4(myID+4, poller);
        EXPECT_FALSE(checkIfFileExist("/tmp/FleXd/shared/ipc/uds/" + std::to_string(myID+4)));
        EXPECT_TRUE(testConnector4.addPeer(myID+3));
        EXPECT_FALSE(checkIfFileExist("/tmp/FleXd/shared/ipc/uds/" + std::to_string(myID+4)));
        std::async(swapClientServer,testConnector3);
        EXPECT_FALSE(checkIfFileExist("/tmp/FleXd/shared/ipc/uds/" + std::to_string(myID+3)));*/
    }

}

