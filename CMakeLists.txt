# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(MerkleCommitment VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找OpenSSL包（用于SHA-256）
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# 添加源文件目录
include_directories(${PROJECT_SOURCE_DIR}/src)

# 主要可执行文件
add_executable(merkle_demo
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/hash.cpp
    ${PROJECT_SOURCE_DIR}/src/merkle_tree.cpp
)

# 链接OpenSSL库
target_link_libraries(merkle_demo ${OPENSSL_LIBRARIES})