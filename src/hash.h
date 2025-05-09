// hash.h
#ifndef HASH_H
#define HASH_H

#include <string>
#include <vector>
#include <openssl/sha.h>

typedef unsigned char byte;
typedef std::vector<byte> ByteArray;

ByteArray sha256(const ByteArray& data);

ByteArray stringToBytes(const std::string& str);

std::string bytesToHexString(const ByteArray& bytes);

ByteArray commit(const ByteArray& data);

#endif // HASH_H