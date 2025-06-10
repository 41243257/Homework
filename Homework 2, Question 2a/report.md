# 41243257、41243234
作業二的第二題(a)
## 解題說明

本題要求撰寫一個程式，從一棵初始為空的二元搜尋樹開始，並進行 `n` 次隨機插入操作。使用均勻亂數產生器來產生要插入的數值。
測量最終生成的二元搜尋樹的高度，並計算這個高度除以 log₂(n)的比值，請針對以下的 n值進行測試：n = 100、500、1000、2000、3000、...、10000。 將每次的`height / log₂(n)比值隨n 變化繪製成圖，這個比值應該會大致上是常數（大約為 2）。
請驗證是否真的是如此。

---

### 解題策略

(1)使用遞迴函式讓二元搜尋樹進行拆分。

(2)主程式產生不同規模 n的隨機整數集合，並插入BST中，建構不同大小的搜尋樹，插入結束後，呼叫 height()取得樹高，計算比例並輸出。

## 程式實作

以下為主要代碼

```cpp
#include <iostream>
#include <set>
#include <random>
#include <cmath>
#include <ctime> // 引入時間標頭

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

```

## 效能分析

1. 時間複雜度：程式的時間複雜度為:插入 n 個節點約需 O(n log n)，計算高度需 O(n)，整體約 O(n log n)。
2.  空間複雜度：空間複雜度為 O(n)，因為程式需要儲存 n 個節點，以及用來避免重複插入的資料結構也佔用 O(n) 空間。

## 測試與驗證

第一次輸出結果:

| n     | Height | log₂(n) | Height / log₂(n) |
| ------- | -------- | ---------- | ------------------- |
| 100   | 13     | 6.64     | 1.96              |
| 500   | 19     | 8.97     | 2.12              |
| 1000  | 21     | 9.97     | 2.11              |
| 2000  | 22     | 10.97    | 2.01              |
| 3000  | 28     | 11.55    | 2.42              |
| 4000  | 26     | 11.97    | 2.17              |
| 5000  | 31     | 12.29    | 2.52              |
| 6000  | 30     | 12.55    | 2.39              |
| 7000  | 29     | 12.77    | 2.27              |
| 8000  | 27     | 12.97    | 2.08              |
| 9000  | 32     | 13.14    | 2.44              |
| 10000 | 33     | 13.29    | 2.48              |
第二次輸出結果:
| n     | Height | log₂(n) | Height / log₂(n) |
| ------- | -------- | ---------- | ------------------- |
| 100   | 12     | 6.64     | 1.81              |
| 500   | 20     | 8.97     | 2.23              |
| 1000  | 24     | 9.97     | 2.41              |
| 2000  | 25     | 10.97    | 2.28              |
| 3000  | 29     | 11.55    | 2.51              |
| 4000  | 28     | 11.97    | 2.34              |
| 5000  | 28     | 12.29    | 2.28              |
| 6000  | 29     | 12.55    | 2.31              |
| 7000  | 32     | 12.77    | 2.51              |
| 8000  | 30     | 12.97    | 2.31              |
| 9000  | 29     | 13.14    | 2.21              |
| 10000 | 30     | 13.29    | 2.26              |

### 編譯與執行指令

```shell
ISO C++11
```

### 結論

(1)程式能夠成功呼叫並計算出n的Height 、log2(n)、 Height / log2(n)

(2)輸出結果證明隨著n的次數增加與隨機產生器每次的輸出結果都出大約在2的常數內。

## 申論及開發報告

這次的程式中採用遞迴，因為二元搜尋樹是一個遞迴定義的資料結構，每個節點都有左右子樹，而每個子樹又是一棵 BST，而且使用遞迴撰寫插入與高度計算的邏輯會更加方便簡單。

例如這一段就直接表達先插進左子樹或右子樹的意思

```cpp
if (val < node->val)
    node->left = insert(node->left, val);
else
    node->right = insert(node->right, val);
```

而且我使用一開始輸出時是使用了亂數產生器，但輸出結果都是一樣的，後面發現是因為我在網路上查詢的亂數產生器不一樣，他是使用了固定種子。

這會導致我的輸出都是採用固定的亂數，我後面才改成1到10000的亂數。

```cpp
random_device rd;
mt19937 gen(rd());
//修改後
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dist(1, 10000000);
```
