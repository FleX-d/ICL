# Changelog
All notable changes to this project will be documented in this file.

## [Unreleased]
## [0.1.0] - 2018-06-18
### Added
- None

### Changed
- Small corrections [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-12
### Added
- None

### Changed
- Implemented findings from review [Martin Strenger]
- Updated server and client examples [Martin Strenger]

### Removed
- Moved common functions into fleXdIPCCommon library (FleXdIPCCommon.h/.cpp) [Martin Strenger]

## [0.1.0] - 2018-06-06
### Added
- None

### Changed
- Updated examples to reflect changes in IPC [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-05
### Added
- None

### Changed
- Consolidation of server/client proxy callbacks [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-01
### Added
- Base class FleXdIPC [Adrian Peniak]

### Changed
- Consolidation of namespaces for whole project [Adrian Peniak]
- Internal structure of naming of functions for FleXdUDS [Adrian Peniak]
- Add fd to FleXdIPCBuffer [Adrian Peniak]

### Removed
- Move IPCBuffer to IPCCommon [Adrian Peniak]
- Comment example for FleXdIPCUDS -> should be fixed  according new implementation [Adrian Peniak]

## [0.0.1] - 2018-05-30
### Added
- New function onClientDisconnect() for FleXdUDSServer [Martin Strenger]
- New functions onConnect() and onDisconnect() for FleXdUDSClient [Martin Strenger]

### Changed
- Renamed function onNewClient() to onClientConnect() for FleXdUDSServer [Martin Strenger]
- Updated examples with new functions [Martin Strenger]

### Removed
- None

## [0.0.1] - 2018-05-29
### Added
- None

### Changed
- sendMsg() for server [Martin Strenger]
- Changed examples [Martin Strenger]
- Bugfixes [Martin Strenger]

### Removed
- None

## [0.0.1] - 2018-04-12
### Added
- Implementation FleXdLogger [Matus Bodorik]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-04-05
### Added
- Inheritance between FleXdUDS(parent) and FleXdUDSClient(child), FleXdUDSServer(child) [Matus Bodorik]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-03-10
### Added
- Example of sending and receive IPC message between Client and Server [Matus Bodorik]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-03-10
### Added
- Implementation of onConnect() function at Client - Sending connnect to Server [Matus Bodorik]
- Implementation of onRead() function at Client - Reading message from Server and sending to buffer [Matus Bodorik]
- Implementation of onWrite() function at Client - Function to write IPC message to Server [Matus Bodorik]
- Implementation of onMsg() function at Client - Lambda fuction which is called by buffer when message is readed [Matus Bodorik]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-02-21
### Added
- Implementation of onAccept() function at Server - Accepting File descriptor of Client [Matus Bodorik]
- Implementation of onRead() function at Server - Reading message from Client and sending to buffer [Matus Bodorik]
- Implementation of onMsg() function at Server - Lambda fuction which is called by buffer when message is readed [Matus Bodorik]
- Implementation of sendMsg() function at Server - Function to write IPC message to Client [Matus Bodorik]
### Changed
- None

### Removed
- None

## [0.0.1] - 2018-02-02
### Added
- Initial version [Adrian Peniak]

### Changed
- None

### Removed
- None
