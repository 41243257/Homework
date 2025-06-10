#include <iostream>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// 插入節點
TreeNode* insert(TreeNode* root, int key) {
    if (!root) return new TreeNode(key);
    if (key < root->val)
        root->left = insert(root->left, key);
    else
        root->right = insert(root->right, key);
    return root;
}

// 中序遍歷
void inorder(TreeNode* root) {
    if (!root) return;
    inorder(root->left);
    cout << root->val << " ";
    inorder(root->right);
}

// 找右子樹最小節點（中序後繼）
TreeNode* findMin(TreeNode* node) {
    while (node && node->left)
        node = node->left;
    return node;
}

// 刪除節點，使用右子樹最小節點替代
TreeNode* deleteNode(TreeNode* root, int key) {
    if (!root) return nullptr;

    if (key < root->val) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->val) {
        root->right = deleteNode(root->right, key);
    } else {
        // 找到要刪除的節點

        // 無子節點或只有一個子節點
        if (!root->left) {
            TreeNode* rightChild = root->right;
            delete root;
            return rightChild;
        } else if (!root->right) {
            TreeNode* leftChild = root->left;
            delete root;
            return leftChild;
        }

        // 兩個子節點，找右子樹最小節點（中序後繼）
        TreeNode* minNode = findMin(root->right);

        // 用後繼節點值取代目前節點值
        root->val = minNode->val;

        // 刪除右子樹中該後繼節點
        root->right = deleteNode(root->right, minNode->val);
    }
    return root;
}

int main() {
    TreeNode* root = nullptr;
    int keys[] = {50, 30, 70, 20, 40, 60, 80};
    for (int k : keys)
        root = insert(root, k);

    cout << "中序遍歷（刪除前）: ";
    inorder(root);
    cout << endl;

    int deleteKey = 20;
    cout << "刪除節點: " << deleteKey << endl;
    root = deleteNode(root, deleteKey);

    cout << "中序遍歷（刪除後）: ";
    inorder(root);
    cout << endl;

    return 0;
}


