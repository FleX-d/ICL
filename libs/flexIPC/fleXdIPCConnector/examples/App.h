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
#include "FleXdEvent.h"
#include <cstdlib>
#include <iostream>

class App : public flexd::icl::ipc::IPCConnector {
    public:
        App(uint32_t myID, flexd::icl::ipc::FleXdEpoll& poller, bool genericPeersAllowed = false, bool debug = false)
        : IPCConnector(myID, poller, genericPeersAllowed),
          m_poller(poller),
          m_debug(debug)
        {}
        virtual ~App() {}

        void send(uint32_t peerID, const std::string& str) {
            std::cout << "App::send(" << peerID << ") -> " << str << std::endl;
            std::vector<uint8_t> data(str.begin(), str.end());
            auto msg = std::make_shared<flexd::icl::ipc::FleXdIPCMsg>(std::move(data));
            msg->getAdditionalHeader()->setValue_1(0);
            msg->getAdditionalHeader()->setValue_4(getMyID());
            msg->getAdditionalHeader()->setValue_5(peerID);
            sendMsg(msg, peerID);
        }

        virtual void onAfterSndMsg(flexd::icl::ipc::pSharedFleXdIPCMsg msg) override {
            if(!m_debug) {
                return;
            }

            if(msg) {
                if(msg->getHeaderParamType() == true) {
                    std::cout << "[Sent to " << msg->getAdditionalHeader()->getValue_5() << "] -> "
                        << std::string(msg->getAdditionalHeader()->getValue_1() ? "ack: " : "payload: ")
                        << std::string(msg->getPayload().begin(), msg->getPayload().end()) << std::endl;
                } else {
                    std::cout << "[Sent handshake step "  << std::to_string(msg->getHeaderParam()) << "]: ";
                    uint8_t c = 0;
                    uint32_t id = 0;
                    for(uint32_t n : msg->getPayload()) {
                        n <<= (3-c++)*8;
                        id |= n;
                        if(c == 4) {
                            std::cout << std::to_string(id) << " ";
                            c = 0;
                            id = 0;
                        }
                    }
                    std::cout << std::endl;
                }
            } else {
                std::cout << "[Sent empty message]" << std::endl;
            }
        }

        virtual void onBeforeRcvMsg(std::shared_ptr<const flexd::icl::ipc::FleXdIPCMsg> msg) override {
            if(!m_debug) {
                return;
            }

            if(msg) {
                if(msg->getHeaderParamType() == true) {
                    std::cout << "[Received from " << msg->getAdditionalHeader()->getValue_4() << "] -> "
                        << std::string(msg->getAdditionalHeader()->getValue_1() ? "ack: " : "payload: ")
                        << std::string(msg->getPayload().begin(), msg->getPayload().end()) << std::endl;
                } else {
                    std::cout << "[Received handshake step "  << std::to_string(msg->getHeaderParam()) << "]: ";
                    uint8_t c = 0;
                    uint32_t id = 0;
                    for(uint32_t n : msg->getPayload()) {
                        n <<= (3-c++)*8;
                        id |= n;
                        if(c == 4) {
                            std::cout << std::to_string(id) << " ";
                            c = 0;
                            id = 0;
                        }
                    }
                    std::cout << std::endl;
                }
            } else {
                std::cout << "[Received empty message]" << std::endl;
            }
        }

        virtual void receiveMsg(flexd::icl::ipc::pSharedFleXdIPCMsg msg) override {
            std::cout << "App::receiveMsg(" << msg->getAdditionalHeader()->getValue_4() <<") -> "
                      << std::string(msg->getAdditionalHeader()->getValue_1() ? "ack: " : "payload: ")
                      << std::string(msg->getPayload().begin(), msg->getPayload().end()) << "\n";
            if(msg->getAdditionalHeader()->getValue_1() == 0) {
                msg->getAdditionalHeader()->setValue_1(1);
                msg->getAdditionalHeader()->setValue_5(msg->getAdditionalHeader()->getValue_4());
                msg->getAdditionalHeader()->setValue_4(getMyID());
                sendMsg(msg, msg->getAdditionalHeader()->getValue_5());
            }
        }

        virtual void onConnectPeer(uint32_t peerID, bool genericPeer) override {
            std::cout << "onConnectPeer() -> " << peerID << ", generic = " << genericPeer <<  "\n";
            std::string msg = genericPeer
                ? "Hello stranger " + std::to_string(peerID)
                : "Hello friend " + std::to_string(peerID);
            send(peerID, msg);
        }

    private:
        flexd::icl::ipc::FleXdEpoll& m_poller;
        bool m_debug;
};

#endif /* APP_H */

