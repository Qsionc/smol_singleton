cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 20)
set(UNIT_TEST ON)

if (UNIX AND NOT APPLE)
    add_compile_options(-Wall -Wextra)
    set(SINGLETON_INCLUDE_INSTALL_DIR "include/singleton")
    set(SINGLETON_CONFIG_INSTALL_DIR "lib/singleton/cmake")
endif ()

project(SingletonWrapper LANGUAGES CXX)

add_library(singleton INTERFACE ${CMAKE_SOURCE_DIR}/include/SingletonWrapper.h)
target_include_directories(singleton INTERFACE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${SINGLETON_INCLUDE_INSTALL_DIR}>)

# Small Google Test
if (UNIT_TEST)
    find_package(GTest)
    add_executable(singleton_test ${CMAKE_SOURCE_DIR}/source/singleton_test.cpp)
    target_link_libraries(singleton_test PRIVATE
            singleton
            GTest::GTest)
endif ()

install(TARGETS singleton EXPORT singleton-lib)
install(EXPORT singleton-lib
        FILE singleton-config.cmake
        NAMESPACE singleton::
        DESTINATION ${SINGLETON_CONFIG_INSTALL_DIR})
install(FILES ${CMAKE_SOURCE_DIR}/include/SingletonWrapper.h DESTINATION ${SINGLETON_INCLUDE_INSTALL_DIR})
