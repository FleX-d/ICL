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
#include "FleXdEvent.h"
#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <atomic>

#define PEER_ID1 1111
#define PEER_ID2 2222
#define PEER_ID3 3333

using namespace flexd::icl::ipc;

namespace {
    FleXdEpoll poller1(10);
    FleXdEpoll poller2(10);
    std::atomic_bool onConnectPeerCalled(false);
    std::atomic_bool genericTest(false);

    class IPCTest: public IPCConnector {
    public:
        IPCTest(uint32_t myID, FleXdEpoll& poller, bool genericPeersAllowed = false, bool garantedDelivery = false)
        :IPCConnector(myID,poller, genericPeersAllowed, garantedDelivery){

        }
        ~IPCTest() = default;
        void receiveMsg(flexd::icl::ipc::pSharedFleXdIPCMsg msg) override {

        }
        void onConnectPeer(uint32_t peerID, bool genericPeer) override {
            onConnectPeerCalled.store(true);
            genericTest.store(genericPeer);
        }
        /*
         * Function for testing sending and receiving messages [Don't used now]
         */
        void sendMSGToPeer(uint32_t peerID){
            std::string stringmsg("Message from ");
            stringmsg += std::to_string(peerID);
            std::vector<uint8_t> data {stringmsg.begin(),stringmsg.end()};
            auto msg = std::make_shared<flexd::icl::ipc::FleXdIPCMsg>(std::move(data));
            msg->getAdditionalHeader()->setValue_1(0);
            msg->getAdditionalHeader()->setValue_4(getMyID());
            msg->getAdditionalHeader()->setValue_5(peerID);
            sendMsg(msg, peerID);
        }
    };
    /**
     * Basic Test of functions
     */

    TEST(IPCConnector, getMyID) {
        FleXdEpoll poller(10);
        std::string socPath = "/tmp/FleXd/shared/ipc/uds/" + std::to_string(PEER_ID1);
        ::unlink(socPath.c_str());
        EXPECT_FALSE(checkIfFileExist(socPath));
        IPCTest testConnector(PEER_ID1, poller);
        EXPECT_EQ(PEER_ID1, testConnector.getMyID());
    }

    TEST(IPCConnector, addPeer) {
        FleXdEpoll poller(10);
        std::string socPath = "/tmp/FleXd/shared/ipc/uds/" + std::to_string(PEER_ID1);
        ::unlink(socPath.c_str());
        EXPECT_FALSE(checkIfFileExist(socPath));
        IPCTest testConnector(PEER_ID1, poller);
        EXPECT_TRUE(testConnector.addPeer(PEER_ID2));
    }

    TEST(IPCConnector, serverInitializationFileSocket) {
        FleXdEpoll poller(10);
        std::string socPath = "/tmp/FleXd/shared/ipc/uds/" + std::to_string(PEER_ID1);
        ::unlink(socPath.c_str());
        EXPECT_FALSE(checkIfFileExist(socPath));
        IPCTest testIPC(PEER_ID1, poller);
        EXPECT_TRUE(testIPC.addPeer(PEER_ID2));
        EXPECT_TRUE(checkIfFileExist(socPath));
    }

    TEST(IPCConnector, initialization_clientToServer) {
        std::string socPath1 = "/tmp/FleXd/shared/ipc/uds/" + std::to_string(PEER_ID1);
        std::string socPath2 = "/tmp/FleXd/shared/ipc/uds/" + std::to_string(PEER_ID2);
        ::unlink(socPath1.c_str());
        EXPECT_FALSE(checkIfFileExist(socPath1));
        ::unlink(socPath2.c_str());
        EXPECT_FALSE(checkIfFileExist(socPath2));

        std::thread handlerPoller1(&FleXdEpoll::loop, &poller1);
        std::thread handlerPoller2(&FleXdEpoll::loop, &poller2);

        IPCTest* App0(new IPCTest(PEER_ID1, poller1));
        IPCTest App1(PEER_ID2, poller2);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        EXPECT_FALSE(checkIfFileExist(socPath1));
        App0->addPeer(PEER_ID2);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        EXPECT_TRUE(checkIfFileExist(socPath1));
        App1.addPeer(PEER_ID1);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        EXPECT_FALSE(checkIfFileExist(socPath2));
        delete App0;
        EXPECT_FALSE(checkIfFileExist(socPath1));
        std::this_thread::sleep_for(std::chrono::milliseconds(7000));
        EXPECT_TRUE(checkIfFileExist(socPath2));

        App0 = new IPCTest(PEER_ID1, poller1);
        EXPECT_FALSE(checkIfFileExist(socPath1));
        App0->addPeer(PEER_ID2);
        EXPECT_FALSE(checkIfFileExist(socPath1));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        delete App0;

        poller1.endLoop();
        poller2.endLoop();
        handlerPoller1.join();
        handlerPoller2.join();
    }

    TEST(IPCConnector, initialization_clientReconnect) {
        std::string socPath1 = "/tmp/FleXd/shared/ipc/uds/" + std::to_string(PEER_ID1);
        std::string socPath2 = "/tmp/FleXd/shared/ipc/uds/" + std::to_string(PEER_ID2);
        ::unlink(socPath1.c_str());
        EXPECT_FALSE(checkIfFileExist(socPath1));
        ::unlink(socPath2.c_str());
        EXPECT_FALSE(checkIfFileExist(socPath2));

        std::thread handlerPoller1(&FleXdEpoll::loop, &poller1);
        std::thread handlerPoller2(&FleXdEpoll::loop, &poller2);

        IPCTest* App0(new IPCTest(PEER_ID1, poller1));
        IPCTest App1(PEER_ID2, poller2);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        EXPECT_FALSE(checkIfFileExist(socPath1));
        App0->addPeer(PEER_ID2);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        EXPECT_TRUE(checkIfFileExist(socPath1));
        App1.addPeer(PEER_ID1);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        EXPECT_FALSE(checkIfFileExist(socPath2));
        delete App0;
        EXPECT_FALSE(checkIfFileExist(socPath1));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        EXPECT_FALSE(checkIfFileExist(socPath2));

        App0 = new IPCTest(PEER_ID1, poller1);
        EXPECT_FALSE(checkIfFileExist(socPath1));
        onConnectPeerCalled.store(false);
        App0->addPeer(PEER_ID2);
        EXPECT_TRUE(checkIfFileExist(socPath1));
        uint8_t count = 0;
        while(!onConnectPeerCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            ASSERT_LE(++count, 20);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        delete App0;

        poller1.endLoop();
        poller2.endLoop();
        handlerPoller1.join();
        handlerPoller2.join();
    }

    TEST(IPCConnector, genericServer) {
        std::string socPath1 = "/tmp/FleXd/shared/ipc/uds/" + std::to_string(PEER_ID3);
        std::string socPath2 = "/tmp/FleXd/shared/ipc/uds/" + std::to_string(PEER_ID2);
        ::unlink(socPath1.c_str());
        EXPECT_FALSE(checkIfFileExist(socPath1));
        ::unlink(socPath2.c_str());

        std::thread handlerPoller1([](void){ poller1.loop(); });
        std::thread handlerPoller2([](void){ poller2.loop(); });

        EXPECT_FALSE(checkIfFileExist(socPath2));
        IPCTest genIPC(PEER_ID3, poller1, true);
        EXPECT_TRUE(genIPC.initGenericServer());
        IPCTest client(PEER_ID2, poller2);
        onConnectPeerCalled.store(false);
        genericTest.store(false);
        EXPECT_TRUE(client.addPeer(PEER_ID3));
        uint8_t count = 0;
        while(!onConnectPeerCalled.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            ASSERT_LE(++count, 20);
        }
        EXPECT_TRUE(genericTest);

        poller1.endLoop();
        poller2.endLoop();
        handlerPoller1.join();
        handlerPoller2.join();
    }

}

