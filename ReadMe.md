
# Internal Communication Layer

The library for communication between processes running on the FleX-d distribution.

## Features
#TODO (Security, socket path, ...)

## Using

#TODO 

## Parts of ICL

### fleXdEpoll

Epoll is application that controls the assignment of the threads to individual applications running on the FleX-d distribution.

#### Link
https://github.com/FleX-d/ICL/tree/dev/libs/flexIPC/fleXdEpoll

### fleXdIPCCommon

The elements that are needed for IPCConnector e.g. message definition, buffer for message buffering from IPC or parents classes for inheriting.

#### Link
https://github.com/FleX-d/ICL/tree/dev/libs/flexIPC/fleXdIPCCommon

### fleXdIPCConnector

The library for ICL client/server implementation. If application wants communicate through ICL it have to using interface of this class.

#### Link
https://github.com/FleX-d/ICL/tree/dev/libs/flexIPC/fleXdIPCConnector

### fleXdUDS

#### Link
https://github.com/FleX-d/ICL/tree/dev/libs/flexIPC/fleXdUDS
