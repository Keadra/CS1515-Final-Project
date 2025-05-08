// hash.h
#ifndef HASH_H
#define HASH_H

#include <string>
#include <vector>
#include <openssl/sha.h>

// 定义字节类型
typedef unsigned char byte;
typedef std::vector<byte> ByteArray;

// SHA-256哈希函数
ByteArray sha256(const ByteArray& data);

// 字符串转换为字节数组
ByteArray stringToBytes(const std::string& str);

// 字节数组转换为十六进制字符串（用于调试和显示）
std::string bytesToHexString(const ByteArray& bytes);

// 基本承诺函数：对数据进行哈希
ByteArray commit(const ByteArray& data);

#endif // HASH_H