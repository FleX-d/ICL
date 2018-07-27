# Changelog
All notable changes to this project will be documented in this file.

## [Unreleased]
## [0.1.0] - 2018-07-27
### Added
- None

### Changed
- [fleXdEpoll] Changed FleXdEvent to be able to trigger event with custom values [Martin Strenger]
- [fleXdEpoll] Updated examples for FleXdEvent [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-07-26
### Added
- [fleXdIPCCommon] New virtual functions isServer() and isClient() for FleXdIPC class [Martin Strenger]
- [fleXdIPCConnector] Reconnection struct [Martin Strenger]

### Changed
- [fleXdEpoll] Updated examples for FleXdTimer [Martin Strenger]
- [fleXdIPCCommon] New virtual functions isServer() and isClient() for FleXdIPC class [Martin Strenger]
- [fleXdIPCConnector] Changed behaviour for clinets on disconnection from server [Martin Strenger]
- [fleXdIPCConnector] Changed behaviour for generic clinets on disconnection from server [Martin Strenger]
- [fleXdIPCConnector] Updated unit tests [Martin Strenger]
- [fleXdUDS] Update regarding changes in FleXdIPCCommon [Martin Strenger]

### Removed
- [fleXdEpoll] Removed FleXdSignalHandler class (singleton) [Martin Strenger]

## [0.1.0] - 2018-07-17
### Added
- [fleXdEpoll] New setOn... functions for FleXdEvent and FleXdTimer [Martin Strenger]
- [fleXdEpoll] Added FleXdEvent::uninit() function [Martin Strenger]
- [fleXdIPCConnector] New virtual functions [Martin Strenger]

### Changed
- [fleXdEpoll] Fixed missing file descriptor closing in FleXdTimer::stop() function [Martin Strenger]
- [fleXdEpoll] Fixed behaviour for multiple calling of FleXdEvent::init() function [Martin Strenger]
- [fleXdEpoll] Fixed behaviour for multiple calling of FleXdTimer::start() and FleXdTimer::stop() functions [Martin Strenger]
- [fleXdEpoll] Fixed epoll_wait deadlock [Martin Strenger]
- [fleXdIPCCommon] Fixed segmentation fault when proxy was deleted on the run (example: switching from client to server) [Martin Strenger]
- [fleXdIPCConnector] Fixed message sending, sending before handshake is no more possible [Martin Strenger]
- [fleXdIPCConnector] Changed fleXdIPCConnector examples [Martin Strenger]
- [fleXdIPCConnector] Fixed fleXdIPCConnector unit tests [Martin Strenger]
- [fleXdUDS] Fixed missing file descriptor closing [Martin Strenger]
- [fleXdUDS] Fixed missing event removal from epoll [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-07-04
### Added
- Unit test [Jakub Pekar]

### Changed
- Solution for client crash after server disconnection [Jakub Pekar]

### Removed
- None

## [0.1.0] - 2018-06-28
### Added
- fleXdIPCCommon: Unit tests for messages bigger than 8192 bytes [Martin Strenger]

### Changed
- fleXdIPCCommon: Fixed finding non-corrupted message for messages bigger than 8192 bytes [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-27
### Added
- Google tests for IPC Connector[Jakub Pekar]

### Changed
- Changing client into server mode after server crashing [Jakub Pekar]

### Removed
- None

## [0.1.0] - 2018-06-18
### Added
- Generic interface for IPCConnector [Martin Strenger]

### Changed
- Various small corrections [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-12
### Added
- CMakeLists option to enable examples [Martin Strenger]
- New functionalities FleXdTimer, FleXdEvent, FleXdTermEvent, FleXdSignalHandler [Martin Strenger]
- Added FleXdIPCCommon.h/.cpp to fleXdIPCCommon library [Martin Strenger]

### Changed
- Implemented findings from review [Martin Strenger]
- Moved common functions into fleXdIPCCommon library (FleXdIPCCommon.h/.cpp) [Martin Strenger]
- Updated UDS server/client examples [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-06
### Added
- None

### Changed
- Updated fleXdUDS examples to reflect changes in IPC [Martin Strenger]
- Updated CMakeLists.txt for FleXdIPCCommon unit tests to reflect name and code changes [Martin Strenger]
- Updated unit tests for FleXdIPCCommon [Martin Strenger]

### Removed
- None

## [0.1.1] - 2018-06-05
### Added
- None

### Changed
- Fix FleXdIPCConnector handshake and extend examples [Adrian Peniak]

### Removed
- None

## [0.1.0] - 2018-06-05
### Added
- None

### Changed
- Renamed proxy functions connectIPC() to connnect() and disconnectIPC() to disconnect() [Martin Strenger]
- Consolidation of server/client proxy callbacks [Martin Strenger]

### Removed
- None

## [0.1.0] - 2018-06-01
### Added
- Draft for IPCConnector [Adrian Peniak]
- Base class FleXdIPC [Adrian Peniak]
- Proxy FleXdIPCProxy and FleXdIPCProxyBuilder [Adrian Peniak]
- Simple example of handshake between 3 App's for FleXdIPCConnector [Adrian Peniak]

### Changed
- Consolidation of namespaces for whole project [Adrian Peniak]
- Internal structure of naming of functions for FleXdUDS [Adrian Peniak]
- Add fd to FleXdIPCBuffer [Adrian Peniak]

### Removed
- Move IPCBuffer to IPCCommon [Adrian Peniak]
- Comment example for FleXdIPCUDS -> should be fixed  according new implementation [Adrian Peniak]

## [0.0.1] - 2018-04-19
### Added
- None

### Changed
- Fix review findings [Matus Bodorik]

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
- UnitTests for FleXdFactory, FleXdMessage, FleXdBuffer [Matus Bodorik]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-04-05
### Added
- Inheritance at FleXdUDS directory, between FleXdUDSClient, FleXdUDSServer and FleXdUDS [Matus Bodorik]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-03-20
### Added
- Function to parse corrupted data and try to find next non-corrupted message at class FleXdFactory [Matus Bodorik]

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-02-01
### Added
- Initial version [Adrian Peniak]

### Changed
- None

### Removed
- None
