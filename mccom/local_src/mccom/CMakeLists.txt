cmake_minimum_required(VERSION 2.6.0)

set(CMAKE_CXX_STANDARD 14)



file(GLOB SRC_FILES *.cpp)

add_library(mccom SHARED
	${SRC_FILES}
)



find_library(BOOST_SYSTEM_PATH libboost_system-mt.a)
add_library(libboost_system STATIC IMPORTED)
set_target_properties(libboost_system PROPERTIES
	IMPORTED_LOCATION ${BOOST_SYSTEM_PATH}
)

target_link_libraries(mccom
	libboost_system
)


#<hack purpose="workaround for outdated libc/libstdcpp on the target">
add_library(libstdcpp STATIC IMPORTED)
set_target_properties(libstdcpp PROPERTIES
	IMPORTED_LOCATION /opt/OSELAS.Toolchain-2016.06.1/i586-unknown-linux-gnu/gcc-5.4.0-glibc-2.23-binutils-2.26-kernel-4.6-sanitized/i586-unknown-linux-gnu/lib/libstdc++.a
#	IMPORTED_LOCATION /opt/OSELAS.Toolchain-2014.12.2/arm-v7a-linux-gnueabihf/gcc-4.9.2-glibc-2.20-binutils-2.24-kernel-3.16-sanitized/arm-v7a-linux-gnueabihf/lib/libstdc++.a
)

add_library(libc STATIC IMPORTED)
set_target_properties(libc PROPERTIES
	IMPORTED_LOCATION /opt/OSELAS.Toolchain-2016.06.1/i586-unknown-linux-gnu/gcc-5.4.0-glibc-2.23-binutils-2.26-kernel-4.6-sanitized/sysroot-i586-unknown-linux-gnu/usr/lib/libc.a
#	IMPORTED_LOCATION /opt/OSELAS.Toolchain-2014.12.2/arm-v7a-linux-gnueabihf/gcc-4.9.2-glibc-2.20-binutils-2.24-kernel-3.16-sanitized/sysroot-arm-v7a-linux-gnueabihf/usr/lib/libc.a
)

target_link_libraries(mccom
	libstdcpp
	libc
)
#</hack>


set_target_properties(mccom PROPERTIES
	LINK_FLAGS "-Wl,--strip-all"
#	LINK_FLAGS "-Wl,--version-script,${PROJECT_SOURCE_DIR}/export.map"
)



set_target_properties(mccom PROPERTIES
	PREFIX ""
	OUTPUT_NAME "MCCOM"
	SUFFIX ".O"
)

install(
	TARGETS mccom
	DESTINATION /FFS0/SSMC
)
install(
	FILES MCCOM.LIB
	DESTINATION /FFS0/SSMC
)
