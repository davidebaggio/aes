cmake_minimum_required(VERSION 3.10)

project(AES128Demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Encryptor library ---
add_library(encryptor src/encryptor.cpp src/pipeline.cpp src/utils.cpp)
target_include_directories(encryptor PUBLIC include)

# --- Decryptor library ---
add_library(decryptor src/decryptor.cpp src/pipeline.cpp src/utils.cpp)
target_include_directories(decryptor PUBLIC include)

add_executable(aes src/aes.cpp)
add_executable(server src/server.cpp)

target_link_libraries(aes PRIVATE encryptor decryptor)
target_link_libraries(server PRIVATE encryptor decryptor)
target_include_directories(server PUBLIC include)

if(ENABLE_TLS)
  find_package(OpenSSL REQUIRED)  # provides OpenSSL::SSL and OpenSSL::Crypto
  target_compile_definitions(server PRIVATE CPPHTTPLIB_OPENSSL_SUPPORT)
  target_link_libraries(server PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif()
