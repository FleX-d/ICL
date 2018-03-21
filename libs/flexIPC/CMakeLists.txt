cmake_minimum_required(VERSION 3.0)

add_subdirectory(fleXdEpoll)
add_subdirectory(fleXdIPCBuffer)
add_subdirectory(fleXdUDS)
add_subdirectory(fleXdIPCConnector)

SET(fleXdEpollPublicInc "${fleXdEpollPublicInc}" PARENT_SCOPE)
SET(fleXdEpollLib "${fleXdEpollLib}" PARENT_SCOPE)
SET(fleXdIPCBufferPublicInc "${fleXdIPCBufferPublicInc}" PARENT_SCOPE)
SET(fleXdIPCBufferLib "${fleXdIPCBufferLib}" PARENT_SCOPE)