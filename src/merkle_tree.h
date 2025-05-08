// merkle_tree.h
#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include "hash.h"
#include <vector>
#include <memory>

class MerkleTree {
public:
    // 构造函数，从数据项列表构建Merkle树
    MerkleTree(const std::vector<ByteArray>& data);
    
    // 获取树的根哈希（承诺）
    ByteArray getRootHash() const;
    
    // 为数据项生成包含证明
    std::vector<ByteArray> generateProof(size_t index) const;
    
    // 验证证明
    static bool verifyProof(const ByteArray& rootHash, 
                           const ByteArray& data,
                           const std::vector<ByteArray>& proof,
                           size_t index,
                           size_t totalLeaves);

private:
    // 树节点结构
    struct Node {
        ByteArray hash;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        
        // 叶子节点构造函数
        Node(const ByteArray& data) : hash(commit(data)), left(nullptr), right(nullptr) {}
        
        // 内部节点构造函数
        Node(std::shared_ptr<Node> left, std::shared_ptr<Node> right) 
            : left(left), right(right) {
            // 连接左右子节点的哈希并计算新哈希
            ByteArray combined;
            combined.insert(combined.end(), left->hash.begin(), left->hash.end());
            combined.insert(combined.end(), right->hash.begin(), right->hash.end());
            hash = sha256(combined);
        }
    };
    
    // 树的根节点
    std::shared_ptr<Node> root;
    
    // 叶子节点数量
    size_t numLeaves;
    
    // 构建树（递归）
    std::shared_ptr<Node> buildTree(const std::vector<std::shared_ptr<Node>>& leaves, 
                                    size_t start, size_t end);
    
    // 获取下一个2的幂（用于处理非2的幂数量的数据项）
    size_t nextPowerOfTwo(size_t n);
};

#endif // MERKLE_TREE_H