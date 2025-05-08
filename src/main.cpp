// main.cpp
#include "merkle_tree.h"
#include <iostream>

#include <string>
#include <vector>

int main() {
    std::cout << "Merkle树简洁承诺方案演示" << std::endl;
    std::cout << "==========================" << std::endl << std::endl;
    
    // 准备一些测试数据
    std::vector<ByteArray> data = {
        stringToBytes("数据项 1"),
        stringToBytes("数据项 2"),
        stringToBytes("数据项 3"),
        stringToBytes("数据项 4")
    };
    
    // 构建Merkle树
    std::cout << "构建Merkle树..." << std::endl;
    MerkleTree tree(data);
    
    // 获取根哈希（承诺）
    ByteArray rootHash = tree.getRootHash();
    std::cout << "Merkle树根哈希: " << bytesToHexString(rootHash) << std::endl << std::endl;
    
    // 选择要证明的数据项索引
    size_t proofIndex = 2; // 对应 "数据项 3"
    std::cout << "为索引 " << proofIndex << " 的数据项生成证明..." << std::endl;
    
    // 生成证明
    std::vector<ByteArray> proof = tree.generateProof(proofIndex);
    
    std::cout << "证明包含 " << proof.size() << " 个哈希值:" << std::endl;
    for (size_t i = 0; i < proof.size(); i++) {
        std::cout << "  哈希 " << i + 1 << ": " << bytesToHexString(proof[i]) << std::endl;
    }
    std::cout << std::endl;
    
    // 验证证明
    std::cout << "验证证明..." << std::endl;
    bool isValid = MerkleTree::verifyProof(rootHash, data[proofIndex], proof, proofIndex, data.size());
    
    if (isValid) {
        std::cout << "✓ 证明验证成功！数据项确实在承诺集中。" << std::endl;
    } else {
        std::cout << "✗ 证明验证失败！" << std::endl;
    }
    
    // 尝试用错误的数据验证
    std::cout << std::endl << "尝试用修改后的数据项验证..." << std::endl;
    ByteArray modifiedData = stringToBytes("修改后的数据项 3");
    bool shouldFail = MerkleTree::verifyProof(rootHash, modifiedData, proof, proofIndex, data.size());
    
    if (!shouldFail) {
        std::cout << "✓ 如预期的那样，修改后的数据验证失败。" << std::endl;
    } else {
        std::cout << "✗ 验证错误地成功了！" << std::endl;
    }
    
    return 0;
}