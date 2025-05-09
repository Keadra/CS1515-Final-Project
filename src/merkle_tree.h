// merkle_tree.h
#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include "hash.h"
#include <vector>
#include <memory>

class MerkleTree {
public:
    MerkleTree(const std::vector<ByteArray>& data);
    
    ByteArray getRootHash() const;
    
    std::vector<ByteArray> generateProof(size_t index) const;
    
    static bool verifyProof(const ByteArray& rootHash, 
                           const ByteArray& data,
                           const std::vector<ByteArray>& proof,
                           size_t index,
                           size_t totalLeaves);

private:
    struct Node {
        ByteArray hash;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        
        Node(const ByteArray& data) : hash(commit(data)), left(nullptr), right(nullptr) {}
        
        Node(std::shared_ptr<Node> left, std::shared_ptr<Node> right) 
            : left(left), right(right) {

            ByteArray combined;
            combined.insert(combined.end(), left->hash.begin(), left->hash.end());
            combined.insert(combined.end(), right->hash.begin(), right->hash.end());
            hash = sha256(combined);
        }
    };
    
    std::shared_ptr<Node> root;
    size_t numLeaves;
    
    std::shared_ptr<Node> buildTree(const std::vector<std::shared_ptr<Node>>& leaves, 
                                    size_t start, size_t end);

    size_t nextPowerOfTwo(size_t n);
};

#endif // MERKLE_TREE_H