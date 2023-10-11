/*Merkle Tree Implementation in C++ - Documentation

Introduction:

The Merkle Tree is a fundamental data structure frequently utilized in blockchain technology for the efficient verification and storage of transactions within a block. This documentation aims to elucidate the C++ implementation of a Merkle Tree and its key functionalities.

Class Overview:

Node Class:

The Node class signifies a node within a binary tree.
It encapsulates data, left-child, and right-child pointers.
You can verify whether a node is complete (having two children) or a leaf node (lacking children).
MerkleTree Class:

The MerkleTree class is responsible for constructing, hashing, and validating the Merkle Tree.
It encompasses a private method for computing SHA-256 hashes and public methods for building, computing, and validating the tree.
Building the Merkle Tree:

To establish a Merkle Tree, you begin by initializing an instance of the MerkleTree class.

Employ the buildTreeFromVector method to construct the tree from a list of transactions (expressed as strings). The requisite number of nodes for the tree is automatically ascertained.

Calculating the Merkle Root:

Subsequent to constructing the tree, you can utilize the calculateMerkleRoot method to derive the Merkle root. This operation involves recursively hashing the data of child nodes.
Obtaining the Merkle Root:

You can retrieve the Merkle root via the getMerkleRoot method. It returns the computed Merkle root as a hexadecimal string.
Verifying Transactions:

To verify whether a list of transactions corresponds to the Merkle root, employ the verifyTransactions method.
This method generates a new Merkle Tree from a list of transactions earmarked for verification, calculates its Merkle root, and compares it to the original Merkle root.
Example Usage:

In the provided example, a sample list of transactions is generated.
An instance of the Merkle Tree is initialized, and the tree is constructed from the list of transactions.
The Merkle root is computed and displayed.
Subsequently, the transactions are scrutinized against the Merkle root.*/

#include <iostream>
#include <vector>
#include <string>
#include <openssl/sha.h>
#include <cmath>

// Node class representing a node of a binary tree
class Node {
public:
    std::string data;
    Node* left;
    Node* right;

    Node(const std::string& data) : data(data), left(nullptr), right(nullptr) {}

    bool isFull() const {
        return (left != nullptr) && (right != nullptr);
    }

    bool isLeaf() const {
        return (left == nullptr) && (right == nullptr);
    }
};

// MerkleTree class for the actual implementation of the tree
class MerkleTree {
private:
    Node* root;
    std::string merkleRoot;

    // Private utility function to calculate the SHA-256 hash of a string
    std::string calculateHash(const std::string& str) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, str.c_str(), str.size());
        SHA256_Final(hash, &sha256);

        char hashString[SHA256_DIGEST_LENGTH * 2 + 1];
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            sprintf(hashString + (i * 2), "%02x", hash[i]);
        }

        return std::string(hashString);
    }

    // Utility function to perform an inorder traversal of the tree
    void inorderTraversal(Node* node) {
        if (node == nullptr) {
            return;
        }

        inorderTraversal(node->left);
        std::cout << node->data << std::endl;
        inorderTraversal(node->right);
    }

    // Function to calculate the Merkle root of the tree recursively
    Node* calculateMerkleRoot(Node* node) {
        if (node->isLeaf()) {
            return node;
        }

        Node* leftChild = calculateMerkleRoot(node->left);
        Node* rightChild = calculateMerkleRoot(node->right);

        // Concatenate and hash the data of left and right children
        std::string combinedData = leftChild->data + rightChild->data;
        node->data = calculateHash(combinedData);

        return node;
    }

public:
    MerkleTree() : root(nullptr), merkleRoot("") {}

    // Function to build the Merkle tree from a list of transactions
    void buildTreeFromVector(const std::vector<std::string>& transactions) {
        // Calculate the number of nodes required for the tree
        int numNodes = 2 * transactions.size() - 1;
        std::vector<int> nodeIndices(numNodes);

        for (int i = 0; i < numNodes; i++) {
            nodeIndices[i] = i + 1;
        }

        // Build the binary tree
        root = buildTree(nodeIndices, 0, numNodes);

        // Add transaction data to leaf nodes
        addLeafData(transactions, root);
    }

    // Private function to build the binary tree
    Node* buildTree(const std::vector<int>& indices, int index, int numNodes) {
        if (index < numNodes) {
            Node* node = new Node(std::to_string(indices[index]));
            node->left = buildTree(indices, 2 * index + 1, numNodes);
            node->right = buildTree(indices, 2 * index + 2, numNodes);
            return node;
        }
        return nullptr;
    }

    // Private function to add transaction data to leaf nodes
    void addLeafData(std::vector<std::string>& transactions, Node* node) {
        if (node == nullptr) {
            return;
        }

        addLeafData(transactions, node->left);

        if (node->isLeaf()) {
            node->data = calculateHash(transactions.back());
            transactions.pop_back();
        }

        addLeafData(transactions, node->right);
    }

    // Function to calculate the Merkle root of the tree
    void calculateMerkleRoot() {
        if (root == nullptr) {
            return;
        }

        Node* merkleRootNode = calculateMerkleRoot(root);
        merkleRoot = merkleRootNode->data;
    }

    // Function to get the Merkle root of the tree
    std::string getMerkleRoot() {
        return merkleRoot;
    }

    // Function to verify if a list of transactions matches the Merkle root
    bool verifyTransactions(const std::vector<std::string>& transactionsToVerify) {
        MerkleTree newTree;
        newTree.buildTreeFromVector(transactionsToVerify);
        newTree.calculateMerkleRoot();
        std::string newMerkleRoot = newTree.getMerkleRoot();

        if (merkleRoot == newMerkleRoot) {
            std::cout << "Transactions verified successfully" << std::endl;
            return true;
        }
        else {
            std::cout << "Transactions have been tampered" << std::endl;
            return false;
        }
    }
};

int main() {
    // Sample list of transactions
    std::vector<std::string> transactions = {"txn1", "txn2", "txn3", "txn4"};

    // Create a MerkleTree instance
    MerkleTree merkleTree;

    // Build the Merkle tree from the list of transactions
    merkleTree.buildTreeFromVector(transactions);

    // Calculate the Merkle root
    merkleTree.calculateMerkleRoot();

    // Get the Merkle root
    std::string merkleRoot = merkleTree.getMerkleRoot();

    std::cout << "Merkle Root: " << merkleRoot << std::endl;

    // Verify transactions
    std::vector<std::string> transactionsToVerify = {"txn1", "txn2", "txn3", "txn4"};
    merkleTree.verifyTransactions(transactionsToVerify);

    return 0;
}
