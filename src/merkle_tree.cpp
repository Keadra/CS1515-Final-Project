// merkle_tree.cpp
#include "merkle_tree.h"
#include <cmath>
#include <functional> 
#include <stdexcept>

MerkleTree::MerkleTree(const std::vector<ByteArray>& data) {
    if (data.empty()) {
        throw std::invalid_argument("Cannot create Merkle tree with empty data.");
    }
    
    std::vector<std::shared_ptr<Node>> leaves;
    for (const auto& item : data) {
        leaves.push_back(std::make_shared<Node>(item));
    }
    
    numLeaves = leaves.size();
    
    root = buildTree(leaves, 0, leaves.size() - 1);
}

std::shared_ptr<MerkleTree::Node> MerkleTree::buildTree(
    const std::vector<std::shared_ptr<Node>>& leaves, size_t start, size_t end) {
    
    if (start == end) {
        return leaves[start];
    }
    
    size_t mid = start + (end - start) / 2;
    
    auto left = buildTree(leaves, start, mid);
    auto right = buildTree(leaves, mid + 1, end);
    
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
    

    size_t height = static_cast<size_t>(std::ceil(std::log2(numLeaves)));
    size_t nodesInCurrentLevel = numLeaves;
    
    std::vector<std::shared_ptr<Node>> currentLevel = std::vector<std::shared_ptr<Node>>(nodesInCurrentLevel);
    
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
    
    for (size_t i = 0; i < height; i++) {
        size_t siblingIndex = (currentIndex % 2 == 0) ? currentIndex + 1 : currentIndex - 1;
        
        if (siblingIndex < nodesInCurrentLevel) {
            proof.push_back(currentLevel[siblingIndex]->hash);
        }
        
        currentIndex /= 2;
        nodesInCurrentLevel = (nodesInCurrentLevel + 1) / 2;
        
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

    ByteArray computedHash = commit(data);
    size_t currentIndex = index;
    
    for (const auto& siblingHash : proof) {
        ByteArray combined;

        if (currentIndex % 2 == 0) { 
            combined.insert(combined.end(), computedHash.begin(), computedHash.end());
            combined.insert(combined.end(), siblingHash.begin(), siblingHash.end());
        } else {
            combined.insert(combined.end(), siblingHash.begin(), siblingHash.end());
            combined.insert(combined.end(), computedHash.begin(), computedHash.end());
        }
        
        computedHash = sha256(combined);
        
        currentIndex /= 2;
    }
    
    return computedHash == rootHash;
}

size_t MerkleTree::nextPowerOfTwo(size_t n) {
    size_t power = 1;
    while (power < n) {
        power *= 2;
    }
    return power;
}