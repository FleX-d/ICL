cmake_minimum_required(VERSION 3.0)

add_subdirectory(fleXdEpoll)
add_subdirectory(fleXdIPCCommon)
add_subdirectory(fleXdUDS)
add_subdirectory(fleXdIPCConnector)

SET(fleXdEpollPublicInc "${fleXdEpollPublicInc}" PARENT_SCOPE)
SET(fleXdEpollLib "${fleXdEpollLib}" PARENT_SCOPE)
SET(fleXdIPCCommonPublicInc "${fleXdIPCCommonPublicInc}" PARENT_SCOPE)
SET(fleXdIPCCommonLib "${fleXdIPCCommonLib}" PARENT_SCOPE)
SET(fleXdUDSPublicInc "${fleXdIPCCommonPublicInc}" PARENT_SCOPE)
SET(fleXdUDSLib "${fleXdIPCCommonLib}" PARENT_SCOPE)