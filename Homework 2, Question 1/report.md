# 41243257、41243234
作業2的第一題

本題要求請撰寫一個ADT MinPQ 編寫一個類似於 ADT 5.2 的 C++ 抽象類，定義了一個最小優先權佇列。現在，寫一個 C++ 類別 MinHeap，繼承自此抽象類，並實作 MinPQ 的所有虛擬函數。
每個函數的複雜度應與 MaxHeap 中對應函數的複雜度相同。

---

## 解題策略

(1)先宣告兩個template分別是抽象基底類別 MinPQ、派生類別 MinHeap。

(2)將Push()、Pop()、isEmpty()、Top()實現在MinPQ。

(3)派生類別 MinHeap使用 `vector<T>`（動態陣列）儲存堆的節點。

(4)使用迴圈percolateUp(int i)跟percolateDown(int i)比較大小。

---

### 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}

    virtual bool isEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};

template <class T>
class MinHeap : public MinPQ<T> {
private:
    vector<T> heap;

    int parent(int i) const { return (i - 1) / 2; }
    int left(int i) const { return 2 * i + 1; }
    int right(int i) const { return 2 * i + 2; }

    void percolateUp(int i) {
        while (i > 0 && heap[parent(i)] > heap[i]) {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void percolateDown(int i) {
        int size = heap.size();
        while (left(i) < size) {
            int smallest = left(i);
            if (right(i) < size && heap[right(i)] < heap[smallest]) {
                smallest = right(i);
            }
            if (heap[i] <= heap[smallest]) break;
            swap(heap[i], heap[smallest]);
            i = smallest;
        }
    }

public:
    bool isEmpty() const override {
        return heap.empty();
    }

    const T& Top() const override {
        if (isEmpty()) throw underflow_error("MinHeap is empty");
        return heap[0];
    }

    void Push(const T& value) override {
        heap.push_back(value);
        percolateUp(heap.size() - 1);
    }

    void Pop() override {
        if (isEmpty()) throw std::underflow_error("MinHeap is empty");
        heap[0] = heap.back();
        heap.pop_back();
        if (!isEmpty()) percolateDown(0);
    }
};


int main() {
    MinHeap<int> pq;
    int data[] = { 7, 2, 5, 1, 9 };

    for (int x : data) {
        pq.Push(x);
    }

    cout << "MinHeap (ascending order): ";
    while (!pq.isEmpty()) {
        cout << pq.Top() << " ";
        pq.Pop();
    }
    cout << endl;

    return 0;
}
```

## 效能分析

1.時間複雜度：程式的時間複雜度為O(n log n)。
2.空間複雜度：空間複雜度為O(n)

## 測試與驗證

| 測試案例 | 輸入參數 \$n\$ | 輸入資料                | 預期輸出（由小到大） | 實際輸出         |
| ---------- | ---------------- | ------------------------- | ---------------------- | ------------------ |
| 測試 1   | 5              | {7, 2, 5, 1, 9}         | 1 2 5 7 9            | 1 2 5 7 9        |
| 測試 2   | 3              | {3, 1, 2}               | 1 2 3                | 1 2 3            |
| 測試 3   | 6              | {10, 20, 15, 30, 40, 5} | 5 10 15 20 30 40     | 5 10 15 20 30 40 |
| 測試 4   | 1              | {42}                    | 42                   | 42               |
| 測試 5   | 0              | {}                      | （無輸出）           | （無輸出）       |

### 編譯與執行指令

```shell
ISO C++11
```

### 結論

1.能夠將排序的數字由小到大排列。

2.產生的時間跟空間複雜度都跟Maxheap一樣。

3.能夠實現Push()、Pop()、isEmpty()、Top()。

---

## 申論及開發報告

---

這一個習題是課本上的最大堆積改成最小堆積，並提供一個最大堆積的抽象MaxPQ，所以我們透過這個範例進行，由於MaxPQ跟MinPQ都是優先權佇列所以只做命名修改

```cpp
template <class T>
class MinPQ {//修改名字
public:
    virtual ~MinPQ() {}//修改名字

    virtual bool isEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};
```

在這個程式中，`MinHeap` 是從抽象類別 `MinPQ` 派生出來的實作類別。
它會將輸入的數值依序加入 `heap`（底層是 `vector`），並在每次插入時利用 `percolateUp()` 維持最小堆的性質：也就是最小值始終在根（heap[0]）。

以輸入 `[7, 2, 5, 1, 9]` 為例，插入後堆的結構會變成 `[1, 2, 5, 7, 9]`（實際上是滿足最小堆特性，不一定完全排序）。

當我們呼叫 `Pop()` 時，會將根節點的最小值（此例中是 1）取出，並用最後一個元素（9）放到根的位置。此時堆變成 `[9, 2, 5, 7]`。

接著執行 `percolateDown(0)`，比較節點 9 和它的子節點 2、5，選出最小的 2 與之交換，繼續調整，直到恢復最小堆性質。

如此重複操作，每次都能取得目前堆中最小的元素，直到堆為空。

```cpp
void percolateUp(int i) {//percolateUp(i) 會從插入位進行父值比大小，交換位置，直到符合規則或爬到最上面。
        while (i > 0 && heap[parent(i)] > heap[i]) {//最大值是(i > 0 && heap[parent(i)] < heap[i]) 
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }
// 用來在 MinHeap 中向下調整指定索引 i 的元素
void percolateDown(int i) {
    int size = heap.size();  // 取得堆目前的大小

    // 當左子節點的索引仍在堆的範圍內，就繼續迴圈
    // 如果沒有左子節點，表示已是葉節點，不需要下沉
    while (left(i) < size) {
        int smallest = left(i);  // 先假設左子節點是比較小的

        // 如果右子節點存在，且右子節點比左子節點小
        // 則將 smallest 更新為右子節點的索引
        if (right(i) < size && heap[right(i)] < heap[smallest]) {
            smallest = right(i);
        }

        // 如果當前節點已經小於等於兩個子節點中較小的，不需要調整
        if (heap[i] <= heap[smallest]) break;

        // 否則就將當前節點與較小的子節點交換
        swap(heap[i], heap[smallest]);

        // 移動索引到剛才交換的位置，繼續往下比較
        i = smallest;
    }
}
```

而這個程式跟MaxHeap 中對應函數的複雜度相同，是因為只修改了比大小。
