// main.cpp - Enhanced version in English
#include "merkle_tree.h"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <iomanip>

// Function to add delay in the demonstration
void sleep(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

// Display a progress bar for visual effect
void showProgressBar(const std::string& label, int totalSteps = 20, int delayMs = 50) {
    std::cout << label;
    for (int i = 0; i < totalSteps; ++i) {
        std::cout << "." << std::flush;
        sleep(delayMs);
    }
    std::cout << " Done!" << std::endl;
}

// Display program title and introduction
void showIntro() {
    std::cout << "\033[1;33m"; // Bold yellow
    std::cout << "===============================================" << std::endl;
    std::cout << "  Succinct Commitment via Merkle Tree - Demo  " << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << "\033[0m"; // Reset color

    std::cout << "\nThis demonstration will show how Merkle Trees can be used to create succinct commitments to data.\n";
    std::cout << "A Merkle Tree allows us to create a short commitment (root hash) to a set of data,\n";
    std::cout << "and then prove that any single data item belongs to that set with a logarithmic-sized proof.\n\n";

    sleep(1000); // Give users time to read the introduction
}

// Show the dataset
void showDataset(const std::vector<ByteArray>& data) {
    std::cout << "\033[1;36m"; // Bold cyan
    std::cout << "\nStep 1: Prepare the Dataset\n";
    std::cout << "\033[0m"; // Reset color
    sleep(500);

    std::cout << "We'll use the following data items to build our Merkle Tree:\n";
    for (size_t i = 0; i < data.size(); ++i) {
        std::string dataStr(data[i].begin(), data[i].end());
        std::cout << "  Data item " << i << ": \"" << dataStr << "\"" << std::endl;
        sleep(300); // Brief delay between data items
    }
    std::cout << "\nA total of " << data.size() << " data items will be used to build the Merkle Tree.\n";
    sleep(1000);
}

// Build the Merkle Tree
MerkleTree buildTree(const std::vector<ByteArray>& data) {
    std::cout << "\033[1;36m"; // Bold cyan
    std::cout << "\nStep 2: Build the Merkle Tree\n";
    std::cout << "\033[0m"; // Reset color
    sleep(500);

    std::cout << "Now building the Merkle Tree...\n";

    // Show progress bars
    showProgressBar("Hashing data items and creating leaf nodes", 10, 100);
    showProgressBar("Building tree structure and computing internal node hashes", 15, 100);

    // Actually build the tree
    MerkleTree tree(data);

    std::cout << "\nTree successfully built!\n";
    return tree;
}

// Show the Merkle Tree root hash
void showRootHash(const ByteArray& rootHash) {
    std::cout << "\033[1;36m"; // Bold cyan
    std::cout << "\nStep 3: Get the Merkle Tree Root Hash (Commitment)\n";
    std::cout << "\033[0m"; // Reset color
    sleep(500);

    std::cout << "The Merkle Tree root hash is a succinct commitment to the entire dataset.\n";
    std::cout << "No matter how large the dataset is, the root hash remains a fixed size.\n\n";

    sleep(500);
    std::cout << "Computing root hash";
    for (int i = 0; i < 5; i++) {
        std::cout << "." << std::flush;
        sleep(200);
    }
    std::cout << "\n\n";

    std::cout << "\033[1;32m"; // Bold green
    std::cout << "Root Hash: " << bytesToHexString(rootHash) << std::endl;
    std::cout << "\033[0m"; // Reset color

    std::cout << "\nThis hash can be published as a succinct commitment to the dataset.\n";
    sleep(1000);
}

// Generate and display proof
std::vector<ByteArray> generateAndShowProof(const MerkleTree& tree, size_t index, const std::vector<ByteArray>& data) {
    std::cout << "\033[1;36m"; // Bold cyan
    std::cout << "\nStep 4: Generate Inclusion Proof for a Data Item\n";
    std::cout << "\033[0m"; // Reset color
    sleep(500);

    std::string dataStr(data[index].begin(), data[index].end());
    std::cout << "We'll generate an inclusion proof for the data item at index " << index << ":\n";
    std::cout << "  Data item: \"" << dataStr << "\"\n\n";

    std::cout << "Generating a proof requires creating a path from the leaf to the root, including all sibling node hashes.\n";
    showProgressBar("Generating proof", 10, 100);

    // Actually generate the proof
    std::vector<ByteArray> proof = tree.generateProof(index);

    std::cout << "\nProof generation complete! The proof contains " << proof.size() << " hash values:\n";
    for (size_t i = 0; i < proof.size(); ++i) {
        std::cout << "  Hash " << i + 1 << ": " << bytesToHexString(proof[i]) << std::endl;
        sleep(300);
    }

    std::cout << "\nThe proof size is " << proof.size() * 32 << " bytes, rather than the size of the entire dataset.\n";
    std::cout << "This demonstrates the succinctness of Merkle Tree proofs - O(log n) where n is the number of data items.\n";
    sleep(1000);

    return proof;
}

// Verify the proof
void verifyProof(const ByteArray& rootHash, const ByteArray& data, const std::vector<ByteArray>& proof,
                 size_t index, size_t totalLeaves, bool shouldPass) {
    std::cout << "\033[1;36m"; // Bold cyan
    if (shouldPass) {
        std::cout << "\nStep 5: Verify the Inclusion Proof\n";
    } else {
        std::cout << "\nStep 6: Attempt Verification with Modified Data\n";
    }
    std::cout << "\033[0m"; // Reset color
    sleep(500);

    std::string dataStr(data.begin(), data.end());
    std::cout << "We'll verify whether the data item \"" << dataStr << "\" at index " << index << " is in the committed set.\n\n";

    std::cout << "The verification process recomputes the root hash using the data item, proof, and index.\n";
    std::cout << "If the computed root hash matches the original, the proof is valid.\n\n";

    showProgressBar("Verifying proof", 15, 100);

    // Actually verify the proof
    bool isValid = MerkleTree::verifyProof(rootHash, data, proof, index, totalLeaves);

    std::cout << "\nVerification result: ";
    if (isValid) {
        std::cout << "\033[1;32m✓ Proof verified successfully!\033[0m The data item is indeed in the committed set.\n";
    } else {
        std::cout << "\033[1;31m✗ Proof verification failed!\033[0m The data item is not in the set or has been modified.\n";
    }

    if (shouldPass && !isValid) {
        std::cout << "\n\033[1;31mNote: Verification should have succeeded but failed. This indicates a potential bug in the implementation.\033[0m\n";
    } else if (!shouldPass && isValid) {
        std::cout << "\n\033[1;31mNote: Verification should have failed but succeeded. This indicates a potential bug in the implementation.\033[0m\n";
    } else if (!shouldPass && !isValid) {
        std::cout << "\nThis is the expected result! Modified data should cause verification to fail.\n";
        std::cout << "This demonstrates the binding property of Merkle Trees - once committed, data cannot be changed.\n";
    }

    sleep(1000);
}

// Run the full demonstration
void runFullDemo() {
    showIntro();

    // Prepare test data
    std::vector<ByteArray> data = {
        stringToBytes("This is the first data item - important information"),
        stringToBytes("Second data item containing some sensitive data"),
        stringToBytes("Third data item is a secret message"),
        stringToBytes("Fourth data item is the final record")
    };

    showDataset(data);

    // Build Merkle Tree
    MerkleTree tree = buildTree(data);

    // Get root hash
    ByteArray rootHash = tree.getRootHash();
    showRootHash(rootHash);

    // Choose index of data item to prove
    size_t proofIndex = 2; // Corresponds to the third data item

    // Generate proof
    std::vector<ByteArray> proof = generateAndShowProof(tree, proofIndex, data);

    // Verify proof
    verifyProof(rootHash, data[proofIndex], proof, proofIndex, data.size(), true);

    // Try to verify with modified data
    ByteArray modifiedData = stringToBytes("This is a modified version of the third data item");
    verifyProof(rootHash, modifiedData, proof, proofIndex, data.size(), false);

    // Summary
    std::cout << "\033[1;33m"; // Bold yellow
    std::cout << "\n===============================================" << std::endl;
    std::cout << "                 Demo Summary                 " << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << "\033[0m"; // Reset color

    std::cout << "\nThis demonstration showed how Merkle Trees can be used to implement a succinct commitment scheme:\n\n";
    std::cout << "1. We built a Merkle Tree for a set of data items\n";
    std::cout << "2. We obtained a succinct commitment (root hash)\n";
    std::cout << "3. We generated a logarithmic-sized proof that a specific data item is included in the commitment\n";
    std::cout << "4. We verified the proof, confirming the data item is indeed in the committed set\n";
    std::cout << "5. We attempted verification with modified data, which failed, demonstrating the binding property\n\n";

    std::cout << "Merkle Tree commitment schemes are useful in many scenarios, including:\n";
    std::cout << "- Blockchain and distributed ledgers\n";
    std::cout << "- Secure audit logs\n";
    std::cout << "- Zero-knowledge proof systems\n";
    std::cout << "- Secure multiparty computation\n\n";

    std::cout << "Thank you for watching this demonstration!\n";
}

// Interactive menu
void showMenu() {
    int choice;

    while (true) {
        std::cout << "\033[1;33m"; // Bold yellow
        std::cout << "\n===============================================" << std::endl;
        std::cout << "  Succinct Commitment via Merkle Tree - Menu  " << std::endl;
        std::cout << "===============================================" << std::endl;
        std::cout << "\033[0m"; // Reset color

        std::cout << "1. Run Full Demonstration" << std::endl;
        std::cout << "2. Create Custom Dataset and Test" << std::endl;
        std::cout << "3. Show How Merkle Trees Work" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "\nPlease choose (1-4): ";

        std::cin >> choice;

        switch (choice) {
        case 1:
            runFullDemo();
            break;

        case 2: {
            // Create custom dataset
            std::vector<ByteArray> customData;
            int numItems;
            std::cout << "\nEnter number of data items: ";
            std::cin >> numItems;
            std::cin.ignore(); // Clear input buffer

            for (int i = 0; i < numItems; i++) {
                std::string item;
                std::cout << "Enter data item " << i << ": ";
                std::getline(std::cin, item);
                customData.push_back(stringToBytes(item));
            }

            // Build tree and show root hash
            MerkleTree customTree(customData);
            ByteArray customRootHash = customTree.getRootHash();
            std::cout << "\nCustom Merkle Tree root hash: " << bytesToHexString(customRootHash) << std::endl;

            // Choose index to generate proof
            size_t customIndex;
            std::cout << "\nEnter index of data item to prove (0-" << customData.size() - 1 << "): ";
            std::cin >> customIndex;

            if (customIndex >= customData.size()) {
                std::cout << "\nError: Index out of range!" << std::endl;
                break;
            }

            // Generate and verify proof
            std::vector<ByteArray> customProof = customTree.generateProof(customIndex);
            std::cout << "\nProof generation successful! Contains " << customProof.size() << " hash values." << std::endl;

            bool isCustomValid = MerkleTree::verifyProof(customRootHash, customData[customIndex],
                                                         customProof, customIndex, customData.size());
            if (isCustomValid) {
                std::cout << "\n✓ Proof verified successfully!" << std::endl;
            } else {
                std::cout << "\n✗ Proof verification failed!" << std::endl;
            }
            break;
        }

        case 3: {
            // Show how Merkle Trees work
            std::cout << "\n\033[1;36mHow Merkle Trees Work\033[0m\n\n";

            std::cout << "1. First, each data item is hashed, forming the leaf nodes of the tree.\n";
            sleep(500);

            std::cout << "2. Adjacent hash values are paired, combined, and hashed again to form parent nodes.\n";
            sleep(500);

            std::cout << "3. This process repeats recursively until a single root hash is obtained.\n";
            sleep(500);

            std::cout << "4. The root hash serves as a succinct commitment to the entire dataset.\n";
            sleep(500);

            std::cout << "\nFor example, with four data items, the tree structure looks like this:\n\n";
            std::cout << "             Root Hash           \n";
            std::cout << "             /      \\            \n";
            std::cout << "            /        \\           \n";
            std::cout << "     Hash(1,2)        Hash(3,4)  \n";
            std::cout << "      /    \\          /    \\     \n";
            std::cout << " Hash(1)  Hash(2)  Hash(3)  Hash(4)\n";
            std::cout << "    |        |        |        |  \n";
            std::cout << "  Data1    Data2    Data3    Data4\n\n";

            std::cout << "To generate a proof, we provide all sibling node hashes on the path from the leaf to the root.\n";
            std::cout << "For example, to prove Data3 is in the tree, the proof includes Hash(4) and Hash(1,2).\n\n";

            std::cout << "The verifier can use these hashes, along with Data3, to recompute the path:\n";
            std::cout << "1. Compute Hash(3) = Hash(Data3)\n";
            std::cout << "2. Compute Hash(3,4) = Hash(Hash(3) + Hash(4))\n";
            std::cout << "3. Compute RootHash' = Hash(Hash(1,2) + Hash(3,4))\n";
            std::cout << "4. Verify RootHash' == RootHash\n\n";

            std::cout << "If the final computed root hash matches the original root hash, the proof is valid.\n";
            break;
        }

        case 4:
            std::cout << "\nThank you for using the Merkle Tree Succinct Commitment Demonstration!\n";
            return;

        default:
            std::cout << "\nInvalid choice, please try again.\n";
        }
    }
}

int main() {
    showMenu();
    return 0;
}
