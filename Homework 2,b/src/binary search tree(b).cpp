#include <iostream>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// ���J�`�I
TreeNode* insert(TreeNode* root, int key) {
    if (!root) return new TreeNode(key);
    if (key < root->val)
        root->left = insert(root->left, key);
    else
        root->right = insert(root->right, key);
    return root;
}

// ���ǹM��
void inorder(TreeNode* root) {
    if (!root) return;
    inorder(root->left);
    cout << root->val << " ";
    inorder(root->right);
}

// ��k�l��̤p�`�I�]���ǫ��~�^
TreeNode* findMin(TreeNode* node) {
    while (node && node->left)
        node = node->left;
    return node;
}

// �R���`�I�A�ϥΥk�l��̤p�`�I���N
TreeNode* deleteNode(TreeNode* root, int key) {
    if (!root) return nullptr;

    if (key < root->val) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->val) {
        root->right = deleteNode(root->right, key);
    } else {
        // ���n�R�����`�I

        // �L�l�`�I�Υu���@�Ӥl�`�I
        if (!root->left) {
            TreeNode* rightChild = root->right;
            delete root;
            return rightChild;
        } else if (!root->right) {
            TreeNode* leftChild = root->left;
            delete root;
            return leftChild;
        }

        // ��Ӥl�`�I�A��k�l��̤p�`�I�]���ǫ��~�^
        TreeNode* minNode = findMin(root->right);

        // �Ϋ��~�`�I�Ȩ��N�ثe�`�I��
        root->val = minNode->val;

        // �R���k�l�𤤸ӫ��~�`�I
        root->right = deleteNode(root->right, minNode->val);
    }
    return root;
}

int main() {
    TreeNode* root = nullptr;
    int keys[] = {50, 30, 70, 20, 40, 60, 80};
    for (int k : keys)
        root = insert(root, k);

    cout << "���ǹM���]�R���e�^: ";
    inorder(root);
    cout << endl;

    int deleteKey = 20;
    cout << "�R���`�I: " << deleteKey << endl;
    root = deleteNode(root, deleteKey);

    cout << "���ǹM���]�R����^: ";
    inorder(root);
    cout << endl;

    return 0;
}


