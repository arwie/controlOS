cmake_minimum_required(VERSION 2.6.0)

file(GLOB SRC_FILES *.cpp)

add_library(stxmcws SHARED
	${SRC_FILES}
)

install(TARGETS stxmcws 
	DESTINATION lib
)

