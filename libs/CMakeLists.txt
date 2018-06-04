cmake_minimum_required(VERSION 3.0)
add_subdirectory(jsonParser)
add_subdirectory(flexIPC)

SET(jsonParserPublicInc ${jsonParserPublicInc} PARENT_SCOPE)
SET(jsonParserLib ${jsonParserLib} PARENT_SCOPE)
