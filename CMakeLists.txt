cmake_minimum_required(VERSION 2.6.0)

set(OSELASTC /opt/OSELAS.Toolchain-2014.12.1/i586-unknown-linux-gnu/gcc-4.9.2-glibc-2.20-binutils-2.24-kernel-3.16-sanitized/sysroot-i586-unknown-linux-gnu)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SRC_FILES *.cpp)


find_library(BOOST_SYSTEM_PATH libboost_system-mt.a)
add_library(libboost_system STATIC IMPORTED)
set_target_properties(libboost_system PROPERTIES
	IMPORTED_LOCATION ${BOOST_SYSTEM_PATH}
)

add_library(libstdcpp STATIC IMPORTED)
set_target_properties(libstdcpp PROPERTIES
	IMPORTED_LOCATION ${OSELASTC}/../i586-unknown-linux-gnu/lib/libstdc++.a
)

add_library(libc STATIC IMPORTED)
set_target_properties(libc PROPERTIES
	IMPORTED_LOCATION ${OSELASTC}/usr/lib/libc.a
)


add_library(stxmccom SHARED
	${SRC_FILES}
)

target_link_libraries(stxmccom
	libboost_system
	libstdcpp
	libc
)

set_target_properties(stxmccom PROPERTIES
	LINK_FLAGS "-Wl,--strip-all"
#	LINK_FLAGS "-Wl,--version-script,${PROJECT_SOURCE_DIR}/export.map"
)


install(TARGETS stxmccom 
	DESTINATION lib
)

install(
	DIRECTORY mcbasic/
	DESTINATION mcbasic
)
