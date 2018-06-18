/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   App.h
 * Author: peniak
 *
 * Created on June 3, 2018, 9:09 PM
 */

#ifndef APP_H
#define APP_H

#include "FleXdIPCConnector.h"
#include "FleXdEpoll.h"
#include <cstdlib>
#include <iostream>

class App : public flexd::icl::ipc::IPCConnector {
    public:
        App(uint32_t myID, flexd::icl::ipc::FleXdEpoll& poller, bool genericPeersAllowed = false)
        : IPCConnector(myID, poller, genericPeersAllowed),
          m_poller(poller),
          m_msgLimit(2000)
        {}
        virtual ~App() {}

        void send(uint32_t peerID, const std::string& str) {
            std::vector<uint8_t> data(str.begin(), str.end());
            auto msg = std::make_shared<flexd::icl::ipc::FleXdIPCMsg>(std::move(data));
            msg->getAdditionalHeader()->setValue_1(0);
            msg->getAdditionalHeader()->setValue_4(getMyID());
            msg->getAdditionalHeader()->setValue_5(peerID);
            sendMsg(msg);
        }

        virtual void receiveMsg(flexd::icl::ipc::pSharedFleXdIPCMsg msg) override {
            std::cout << "App::receiveMsg() -> "
                      << std::string(msg->getAdditionalHeader()->getValue_1() ? "ack: " : "payload: ")
                      << std::string(msg->getPayload().begin(), msg->getPayload().end()) << "\n";
            if(msg->getAdditionalHeader()->getValue_1() == 0) {
                msg->getAdditionalHeader()->setValue_1(1);
                msg->getAdditionalHeader()->setValue_5(msg->getAdditionalHeader()->getValue_4());
                msg->getAdditionalHeader()->setValue_4(getMyID());
                sendMsg(msg);
            }

        }

        virtual void onConnectPeer(uint32_t peerID, bool genericPeer) override {
            std::cout << "onConnectPeer() -> " << peerID << ", generic = "<< genericPeer << "\n";
            std::string msg = "onConnectPeer test msg from " + std::to_string(getMyID()) + " to " + std::to_string(peerID);
            send(peerID, msg);
        }

    private:
        flexd::icl::ipc::FleXdEpoll& m_poller;
        size_t m_msgLimit;
};

#endif /* APP_H */

