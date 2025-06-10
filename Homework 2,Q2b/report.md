# 41243257、41243234

作業二的第二題(b)

## 解題說明

本題要求寫一個 C++ 函數，從二元搜尋樹中刪除鍵為 k 的元素對。函數的時間複雜度是多少。

### 解題策略

(1)建立一個二元搜尋樹。

(2)呼叫使用遞迴實作inorder() 函式來進行中序遍歷。

(3)主程式呼叫遞迴函式insert()跟deleteNode()來進行操作輸出。

---

### 計畫實作

以下為主要課程代碼：

```c++
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

    int deleteKey = 50;
    cout << "刪除節點: " << deleteKey << endl;
    root = deleteNode(root, deleteKey);

    cout << "中序遍歷（刪除後）: ";
    inorder(root);
    cout << endl;

    return 0;
}
```

## 狀況分析

1. 時間複雜度：程式的時間複雜度O(h)。
2. 空間複雜度：空間複雜度為O(h)。

## 測試與驗證

### 測試案例

| 測試案例編號 | 輸入參數 (鍵值序列)        | 要刪除的節點 key | 預期輸出（中序遍歷刪除後） | 實際輸出（中序遍歷刪除後） |
| -------------- | ---------------------------- | ------------------ | ---------------------------- | ---------------------------- |
| 1            | 50, 30, 70, 20, 40, 60, 80 | 50               | 20 30 40 60 70 80          | 20 30 40 60 70 80   |
| 2            | 50, 30, 70, 20, 40, 60, 80 | 30               | 20 40 50 60 70 80          | 20 40 50 60 70 80   |
| 3            | 50, 30, 70, 20, 40, 60, 80 | 20               | 30 40 50 60 70 80          | 30 40 50 60 70 80   |
| 4            | 50, 30, 70, 20, 40, 60, 80 | 80               | 20 30 40 50 60 70          |  20 30 40 50 60 70 |

### 編譯與執行指令

````shell
g++ -o bst_program binary search tree(b).cpp
````

### 結論

1. 能夠將鍵值序列刪除輸出。
2. 符合理論上的高度為`h的樹中，時間複雜度為 ​O(h)，刪除整個過程中，最多做兩次高度為 h的向下遍歷，也就是尋找刪除節點 + 找後繼節點 + 刪除後繼節點，因此時間複雜度仍是 ​O(h)。

## 申論及開發報告

在本程式中由於二元搜尋樹，​本身就是遞迴定義的結構​，並且呼叫同一個函式比較簡單。

選擇中序後繼是後序遍歷是刪除整棵樹或子樹時最安全且自然的遍歷方式，因為它能先處理完所有子節點，最後才處理父節點，在查詢網站時也都是示範中序後繼。

```cpp
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

```
