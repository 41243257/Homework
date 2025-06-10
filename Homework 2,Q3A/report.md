# 41243257、41243234

作業二的第三題(a)

## 解題說明

本題要求寫一個 C++ 外部排序第二階段（Phase Two）的總輸入時間，運用類似(buffering)的演算法，算出第二階段總時間。

### 解題策略

(2)按照公式進行運算

(3)主程式呼叫公式進行運算輸出

---

### 計畫實作

以下為主要課程代碼：

```c++
#include <iostream>
#include <cmath>

using namespace std;

int main() {
    // 常數參數
    const int n = 500000;        // 總紀錄數
    const int m = 64;            // runs 數量
    const int S = 2000;          // 記憶體容量 (records)
    const double ts = 0.08;      // seek time (秒)
    const double tl = 0.02;      // latency time (秒)
    const double t = 0.001;      // 傳輸時間 (秒/紀錄)
    const double tCPU = 500.0;   // CPU 合併時間 (秒)

    // k-way merge 這裡假設 k = m 
    int k = m;

    // Buffer 大小：S / (k + 1)，k個輸入 buffer + 1個輸出 buffer
    int B = S / (k + 1);

    // 總 I/O 次數 (讀完全部 n 筆資料需要的次數)
    long long num_io = (long long)ceil((double)n / B);

    // 計算第二階段的總輸入時間（seek + latency + 傳輸時間）
    double io_time = num_io * (ts + tl) + n * t;

    // 總時間 = I/O時間 + CPU 合併時間（假設不重疊）
    double total_time = io_time + tCPU;

    // 輸出結果
    cout << "=== 外部排序第二階段時間分析 ===" << endl;
    cout << "總紀錄數 n = " << n << endl;
    cout << "runs 數量 m = " << m << endl;
    cout << "記憶體容量 S = " << S << endl;
    cout << "seek time ts = " << ts << " 秒" << endl;
    cout << "latency time tl = " << tl << " 秒" << endl;
    cout << "傳輸時間 t = " << t << " 秒/紀錄" << endl;
    cout << "CPU 合併時間 tCPU = " << tCPU << " 秒" << endl;
    cout << "k-way merge k = " << k << endl;
    cout << "Buffer 大小 B = " << B << " 筆" << endl;
    cout << "總 I/O 次數 = " << num_io << endl;
    cout << "-----------------------------" << endl;
    cout << "I/O 總輸入時間 = " << io_time << " 秒" << endl;
    cout << "CPU 合併時間 = " << tCPU << " 秒" << endl;
    cout << "整體第二階段總時間 = " << total_time << " 秒" << endl;

    return 0;
}
```

## 狀況分析

1. **時間複雜度**:O( Bn​ ⋅(ts+tl)+n⋅t+nlogk)。
2. **空間複雜度**:O(S)


## 測試與驗證

### 測試案例

| 總紀錄數 n | runs數量 m | 記憶體容量 S | seek time ts (秒) | latency tl (秒) | 傳輸時間 t (秒/筆) | CPU合併時間 tCPU (秒) | Buffer大小 B (筆) | 總I/O次數 | I/O總輸入時間 (秒) | 整體第二階段總時間 (秒) |
| ------------ | ------------ | -------------- | ------------------- | ----------------- | -------------------- | ----------------------- | ------------------- | ----------- | -------------------- | ------------------------- |
| 1,000,000  | 16         | 4,000        | 0.05              | 0.01            | 0.0005             | 300                   | 235.29            | 4251      | 755.06             | 1055.06                 |
| 500,000    | 64         | 2,000        | 0.08              | 0.02            | 0.001              | 500                   | 30.77             | 16250     | 2125               | 2625                    |
| 50,000     | 8          | 2,000        | 0.08              | 0.02            | 0.001              | 500                   | 222.22            | 225       | 72.5               | 572.5                   |
| 100,000    | 10         | 3,000        | 0.06              | 0.015           | 0.0008             | 350                   | 272.73            | 367       | 120.06             | 470.06                  |
| 2,000,000  | 32         | 5,000        | 0.07              | 0.02            | 0.0007             | 400                   | 151.52            | 13199     | 963.93             | 1363.93                 |

### 編譯與執行指令

````shell
g++ -o bst_program binary search tree(b).cpp
````

### 結論

1. 透過公式能夠計算出答案
2. 每增加 I/O 次數就會加總這部分時間，而傳輸時間與資料量成正比:總 I/O 時間 = I/O 次數 × (seek + latency) + 傳輸時間 × n。
3. K的數字越大越會降低每個 Buffer 大小，造成更多的 I/O 次數，延長 I/O 時間，但 k 的大小受 runs 數量限制，且 k-way merge 通常選擇所有 runs 一起合併。

## 申論及開發報告

這一次是透過計算公式來執行程式，比較容易出現的錯誤在於計算方面，由於他是公式，因此我們按照Buffering的步驟跟因為是按照理論進行操作，沒有碰觸到合併排序的程式碼。

```

```

