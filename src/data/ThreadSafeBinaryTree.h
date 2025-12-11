#ifndef THREAD_SAFE_BINARY_TREE_H
#define THREAD_SAFE_BINARY_TREE_H

#include <memory>
#include <mutex>
#include <sstream>

template <typename T>
class ThreadSafeBinaryTree {
private:
    struct TreeNode {
        T data;
        std::shared_ptr<TreeNode> left;
        std::shared_ptr<TreeNode> right;
        TreeNode(const T& value) : data(value) {}
    };

    using TreeNodePtr = std::shared_ptr<TreeNode>;
    TreeNodePtr root;
     mutable std::mutex mtx;

    // Recursive insert
    TreeNodePtr insertNode(TreeNodePtr node, const T& value) {
        if (!node) return std::make_shared<TreeNode>(value);
        if (value < node->data)
            node->left = insertNode(node->left, value);
        else if (value > node->data)
            node->right = insertNode(node->right, value);
        return node;
    }

    // Recursive search
    bool searchNode(TreeNodePtr node, const T& value) const {
        if (!node) return false;
        if (value == node->data) return true;
        if (value < node->data)
            return searchNode(node->left, value);
        else
            return searchNode(node->right, value);
    }

    // Find minimum node
    TreeNodePtr findMin(TreeNodePtr node) {
        while (node && node->left)
            node = node->left;
        return node;
    }

    // Recursive remove
    TreeNodePtr removeNode(TreeNodePtr node, const T& value) {
        if (!node) return nullptr;
        if (value < node->data)
            node->left = removeNode(node->left, value);
        else if (value > node->data)
            node->right = removeNode(node->right, value);
        else {
            if (!node->left) return node->right;
            if (!node->right) return node->left;
            TreeNodePtr temp = findMin(node->right);
            node->data = temp->data;
            node->right = removeNode(node->right, temp->data);
        }
        return node;
    }

    // Traversal helpers
    void inOrderTraversal(TreeNodePtr node) const {
        if (!node) return;
        inOrderTraversal(node->left);
        std::ostringstream oss;
        oss << node->data << " ";
        Logger::Debug(oss.str());
        inOrderTraversal(node->right);
    }

    void preOrderTraversal(TreeNodePtr node) const {
        if (!node) return;
        std::ostringstream oss;
        oss << node->data << " ";
        Logger::Debug(oss.str());
        preOrderTraversal(node->left);
        preOrderTraversal(node->right);
    }

    void postOrderTraversal(TreeNodePtr node) const {
        if (!node) return;
        postOrderTraversal(node->left);
        postOrderTraversal(node->right);
        std::ostringstream oss;
        oss << node->data << " ";
        Logger::Debug(oss.str());
    }

public:
    ThreadSafeBinaryTree() : root(nullptr) {}

    bool isEmpty() {
        std::lock_guard<std::mutex> lock(mtx);
        return root == nullptr;
    }

    void insert(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        root = insertNode(root, value);
    }

    bool search(const T& value) const {
        std::lock_guard<std::mutex> lock(mtx);
        return searchNode(root, value);
    }

    void remove(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        root = removeNode(root, value);
    }

    void inOrder() const {
        std::lock_guard<std::mutex> lock(mtx);
        Logger::Debug("[Consumer] print In-Order : ");
        inOrderTraversal(root);
    }

    void preOrder() const {
        std::lock_guard<std::mutex> lock(mtx);
        Logger::Debug("[Consumer] print Pre-Order : ");
        preOrderTraversal(root);
    }

    void postOrder() const {
        std::lock_guard<std::mutex> lock(mtx);
        Logger::Debug("[Consumer] print Post-Order : ");
        postOrderTraversal(root);
    }
};

#endif // THREAD_SAFE_BINARY_TREE_H
