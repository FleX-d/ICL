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
        App(uint32_t myID, flexd::icl::ipc::FleXdEpoll& poller)
        : IPCConnector(myID, poller),
          m_poller(poller),
          m_msgLimit(2000)
        {}
        virtual ~App() {}
        
        void send(const std::string& str) {
            std::vector<uint8_t> data(str.begin(), str.end());
            auto msg = std::make_shared<flexd::icl::ipc::FleXdIPCMsg>(std::move(data));
            sendMsg(msg);
        }
        
        virtual void receiveMsg(flexd::icl::ipc::pSharedFleXdIPCMsg msg) override {
            std::cout << "receiveMsg() -> " << msg->getMsgSize() << "\n";
        }
        
        virtual void onConnectPeer(uint32_t peerID) override {
            std::cout << "onConnectPeer() -> " << peerID << "\n";
        }

    private:
        flexd::icl::ipc::FleXdEpoll& m_poller;
        size_t m_msgLimit;
};

#endif /* APP_H */

