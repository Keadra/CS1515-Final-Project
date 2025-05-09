// hash.cpp
#include "hash.h"
#include <sstream>
#include <iomanip>

ByteArray sha256(const ByteArray& data) {
    ByteArray hash(SHA256_DIGEST_LENGTH);
    SHA256_CTX sha256Context;
    SHA256_Init(&sha256Context);
    SHA256_Update(&sha256Context, data.data(), data.size());
    SHA256_Final(hash.data(), &sha256Context);
    return hash;
}

ByteArray stringToBytes(const std::string& str) {
    ByteArray bytes(str.begin(), str.end());
    return bytes;
}

std::string bytesToHexString(const ByteArray& bytes) {
    std::stringstream ss;
    for (const auto& byte : bytes) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

ByteArray commit(const ByteArray& data) {
    return sha256(data);
}