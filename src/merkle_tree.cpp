// merkle_tree.cpp
#include "merkle_tree.h"
#include <cmath>
#include <functional> 
#include <stdexcept>

MerkleTree::MerkleTree(const std::vector<ByteArray>& data) {
    if (data.empty()) {
        throw std::invalid_argument("Cannot create Merkle tree with empty data.");
    }
    
    // 创建叶子节点
    std::vector<std::shared_ptr<Node>> leaves;
    for (const auto& item : data) {
        leaves.push_back(std::make_shared<Node>(item));
    }
    
    numLeaves = leaves.size();
    
    // 构建树
    root = buildTree(leaves, 0, leaves.size() - 1);
}

std::shared_ptr<MerkleTree::Node> MerkleTree::buildTree(
    const std::vector<std::shared_ptr<Node>>& leaves, size_t start, size_t end) {
    
    // 如果只剩一个节点，直接返回它
    if (start == end) {
        return leaves[start];
    }
    
    // 计算中间位置
    size_t mid = start + (end - start) / 2;
    
    // 递归构建左右子树
    auto left = buildTree(leaves, start, mid);
    auto right = buildTree(leaves, mid + 1, end);
    
    // 创建并返回新的父节点
    return std::make_shared<Node>(left, right);
}

ByteArray MerkleTree::getRootHash() const {
    return root->hash;
}

std::vector<ByteArray> MerkleTree::generateProof(size_t index) const {
    if (index >= numLeaves) {
        throw std::out_of_range("Index out of range");
    }
    
    std::vector<ByteArray> proof;
    size_t currentIndex = index;
    size_t levelSize = numLeaves;
    
    // 计算树的高度
    size_t height = static_cast<size_t>(std::ceil(std::log2(numLeaves)));
    size_t nodesInCurrentLevel = numLeaves;
    
    // 当前层级中的节点数组
    std::vector<std::shared_ptr<Node>> currentLevel = std::vector<std::shared_ptr<Node>>(nodesInCurrentLevel);
    
    // 填充初始层级（叶子节点）
    std::function<void(std::shared_ptr<Node>, size_t, size_t, size_t)> fillLevel = 
        [&](std::shared_ptr<Node> node, size_t nodeIndex, size_t start, size_t end) {
            if (start == end) {
                currentLevel[nodeIndex] = node;
                return;
            }
            
            size_t mid = start + (end - start) / 2;
            fillLevel(node->left, 2 * nodeIndex, start, mid);
            fillLevel(node->right, 2 * nodeIndex + 1, mid + 1, end);
        };
    
    fillLevel(root, 0, 0, numLeaves - 1);
    
    // 遍历树并收集证明
    for (size_t i = 0; i < height; i++) {
        size_t siblingIndex = (currentIndex % 2 == 0) ? currentIndex + 1 : currentIndex - 1;
        
        if (siblingIndex < nodesInCurrentLevel) {
            proof.push_back(currentLevel[siblingIndex]->hash);
        }
        
        // 移动到下一层
        currentIndex /= 2;
        nodesInCurrentLevel = (nodesInCurrentLevel + 1) / 2;
        
        // 更新当前层级的节点
        std::vector<std::shared_ptr<Node>> parentLevel(nodesInCurrentLevel);
        for (size_t j = 0; j < nodesInCurrentLevel; j++) {
            size_t leftIdx = 2 * j;
            size_t rightIdx = 2 * j + 1;
            
            if (rightIdx < currentLevel.size()) {
                parentLevel[j] = std::make_shared<Node>(currentLevel[leftIdx], currentLevel[rightIdx]);
            } else {
                parentLevel[j] = currentLevel[leftIdx];
            }
        }
        
        currentLevel = parentLevel;
    }
    
    return proof;
}

bool MerkleTree::verifyProof(const ByteArray& rootHash, 
                           const ByteArray& data,
                           const std::vector<ByteArray>& proof,
                           size_t index,
                           size_t totalLeaves) {
    // 开始从叶子节点的哈希
    ByteArray computedHash = commit(data);
    size_t currentIndex = index;
    
    // 遍历证明路径
    for (const auto& siblingHash : proof) {
        ByteArray combined;
        
        // 确定当前节点是左子节点还是右子节点
        if (currentIndex % 2 == 0) { // 左子节点
            combined.insert(combined.end(), computedHash.begin(), computedHash.end());
            combined.insert(combined.end(), siblingHash.begin(), siblingHash.end());
        } else { // 右子节点
            combined.insert(combined.end(), siblingHash.begin(), siblingHash.end());
            combined.insert(combined.end(), computedHash.begin(), computedHash.end());
        }
        
        // 计算父节点哈希
        computedHash = sha256(combined);
        
        // 移动到父节点索引
        currentIndex /= 2;
    }
    
    // 检查计算出的根哈希是否与给定的根哈希匹配
    return computedHash == rootHash;
}

size_t MerkleTree::nextPowerOfTwo(size_t n) {
    size_t power = 1;
    while (power < n) {
        power *= 2;
    }
    return power;
}