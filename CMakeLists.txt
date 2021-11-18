cmake_minimum_required(VERSION 3.15)

project(jnipp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_library(jnipp INTERFACE)

if(DEFINED ENV{JAVA_HOME})
    message("Defaulting JDK_LOCATION to JAVA_HOME: $ENV{JAVA_HOME}")
    set(JDK_LOCATION "$ENV{JAVA_HOME}")
    else()
    message("No JAVA_HOME variable set.")
endif()

if(DEFINED JDK_LOCATION)
    target_include_directories(jnipp INTERFACE "${JDK_LOCATION}/include")
endif()

target_include_directories(jnipp INTERFACE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${JDK_LOCATION}/include"
    "${JDK_LOCATION}/include/win32"
)

target_link_libraries(jnipp INTERFACE "${JDK_LOCATION}/lib/jvm.lib")

add_subdirectory("app")