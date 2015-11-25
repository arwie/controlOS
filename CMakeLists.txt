cmake_minimum_required(VERSION 2.6.0)

file(GLOB SRC_FILES *.cpp)

add_library(stxmccom SHARED
	${SRC_FILES}
)

install(TARGETS stxmccom 
	DESTINATION lib
)

