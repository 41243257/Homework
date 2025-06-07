#include <iostream>
#include <set>
#include <random>
#include <cmath>
#include <ctime> 

using namespace std;

struct BSTNode {
    int val;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

class BST {
private:
    BSTNode* root;
    
    BSTNode* insert(BSTNode* node, int val) {
        if (!node) return new BSTNode(val);
        if (val < node->val) node->left = insert(node->left, val);
        else if (val > node->val) node->right = insert(node->right, val);
        return node;
    }

    int height(BSTNode* node) const {
        if (!node) return 0;
        return 1 + max(height(node->left), height(node->right));
    }

    void freeTree(BSTNode* node) {
        if (!node) return;
        freeTree(node->left);
        freeTree(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}
    ~BST() { freeTree(root); }

    void insert(int val) {
        root = insert(root, val);
    }

    int height() const {
        return height(root);
    }
};

int main() {
    // 使用時間作為種子，避免每次結果都一樣
    mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    uniform_int_distribution<> dist(1, 10000000);

    cout << "n\tHeight\tlog2(n)\tHeight/log2(n)\n";
    cout << "----------------------------------------\n";

    int n = 100;
    while (n <= 10000) {
        BST tree;
        set<int> inserted;

        while ((int)inserted.size() < n) {
            int val = dist(gen);
            if (inserted.insert(val).second) {
                tree.insert(val);
            }
        }

        int h = tree.height();
        double log2n = log2(n);
        double ratio = h / log2n;

        cout << n << "\t" << h << "\t" << log2n << "\t" << ratio << endl;

        // 調整下一個 n
        if (n == 100) {
            n = 500;
        } else if (n < 1000) {
            n += 500;
        } else {
            n += 1000;
        }
    }

    return 0;
}



