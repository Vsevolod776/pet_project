cmake_minimum_required(VERSION 3.26)
project(pet_project_on_c C)

set(CMAKE_C_STANDARD 17)

set(OPENSSL_ROOT_DIR /usr/local/opt/openssl@1.1)
set(OPENSSL_INCLUDE_DIR ${OPENSSL_ROOT_DIR}/include)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

add_executable(pet_project_on_c main.c
        blowfish_alhorithm.h
        blowfish_alhorithm.c)
target_link_libraries(pet_project_on_c OpenSSL::Crypto)
