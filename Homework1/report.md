# 41243257、41243234

作業一

## 解題說明

本題要求實現五種排序的average-caseep跟worst-case。

---

## 解題策略

1. average-case:
   1.Insertion Sort是使用了迴圈的方式、讓元素透過對比來不斷調換位置。
   2.QuickSort則是先透過三數中位數選擇法來切割樞軸元素來劃分數列，分成左小右大數列，之後遞迴都會選擇一個新的樞軸來繼續劃分數列，直到所有子數列的長度為1或0為止。
   3.MergeSort也是透過不斷拆分的分治大數列不斷拆分直到最小的子數列，再透過合併來這些子數列大小完成排序。
   4.HeapSort是透過將數列分成最大堆然後不斷將最大的數跟最小的數調換，重複直到完成排序
   5.Composite Sort則是靠條件式來選擇最快的演算法，n<500採用Insertion Sort，n<=5000時採用QuickSort，超過5000採用MergeSort。
2. worst-case:
   1.Insertion Sort的測資按照[n, n-1, n-2, ... , 1]，將n的排列相反過來，每次插入元素時，所有的前面元素都需要移動一遍，這樣會導致最多的比較和交換。
   2.QuickSort的測資是採用升序[1, 2, 3, ..., n]，由於採用三數中位數選擇法，中位數會採用最大數造成分割不平衡。
   3.MergeSort將測資打亂的隨機資料。
   4.HeapSort採用逆序排列資料 (`n, n-1, ..., 2, 1`)。

## 程式實作

以下為**average-case**五種排序程式碼：

```cpp
#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <fstream>
#include <chrono>
#include <functional>
#include <Windows.h>
#include <Psapi.h>

using namespace std;

// 插入排序（傳值版本）
vector<int> insertionSort(vector<int> arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
    return arr;
}

// 三數中位數選擇樞軸
int medianOfThree(vector<int>& arr, int left, int right) {
    int mid = (left + right) / 2;
    if (arr[left] > arr[mid]) swap(arr[left], arr[mid]);
    if (arr[left] > arr[right]) swap(arr[left], arr[right]);
    if (arr[mid] > arr[right]) swap(arr[mid], arr[right]);
    swap(arr[mid], arr[right - 1]);
    return arr[right - 1];
}

// 快速排序
void quickSort(vector<int>& arr, int left, int right) {
    if (left + 10 <= right) {
        int pivot = medianOfThree(arr, left, right);
        int i = left + 1, j = right - 2;
        while (true) {
            while (arr[i] < pivot) ++i;
            while (arr[j] > pivot) --j;
            if (i < j) {
                swap(arr[i], arr[j]);
                ++i;
                --j;
            } else {
                break;
            }
        }
        swap(arr[i], arr[right - 1]);
        quickSort(arr, left, i - 1);
        quickSort(arr, i, right);
    }
}

// 快速排序包裝函數
vector<int> quickSortWrapper(vector<int> arr) {
    quickSort(arr, 0, arr.size() - 1);
    return arr;
}

// 迭代式合併排序（傳值版本）
vector<int> mergeSortIterative(vector<int> arr) {
    int n = arr.size();
    if (n <= 1) return arr;

    vector<int> temp(n);

    // 從小的區塊開始，將相鄰的兩個元素合併
    for (int size = 1; size < n; size *= 2) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = min(left + size, n);
            int right = min(left + 2 * size, n);

            // 合併 arr[left...mid-1] 和 arr[mid...right-1]
            int i = left, j = mid, k = left;
            while (i < mid && j < right) {
                if (arr[i] <= arr[j]) temp[k++] = arr[i++];
                else temp[k++] = arr[j++];
            }

            // 如果左邊的還有剩餘
            while (i < mid) temp[k++] = arr[i++];

            // 如果右邊的還有剩餘
            while (j < right) temp[k++] = arr[j++];

            // 把已經合併的結果複製回 arr
            for (int i = left; i < right; ++i) arr[i] = temp[i];
        }
    }

    return arr;
}

// 堆排序（傳值版本）
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

vector<int> heapSort(vector<int> arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; --i) heapify(arr, n, i);
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
    return arr;
}

// 複合排序
vector<int> compositeSort(vector<int> arr) {
    if (arr.size() < 500) return insertionSort(arr);
    else if (arr.size() <= 5000) return quickSortWrapper(arr);
    else return mergeSortIterative(arr);
}
```

以下為**average-case**時間、記憶體測量與主程式測資的程式碼:

```cpp
// 顯示記憶體資訊
void printMemoryUsage(size_t& workingSet, size_t& peakWorkingSet, size_t& pagefileUsage) {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    workingSet = memInfo.WorkingSetSize / 1024;
    peakWorkingSet = memInfo.PeakWorkingSetSize / 1024;
    pagefileUsage = memInfo.PagefileUsage / 1024;
}

// 輸出測試資料到 .txt 檔案
void saveDataToFile(const vector<int>& data, const string& filename) {
    ofstream outFile(filename);
    for (int num : data) {
        outFile << num << endl;
    }
    outFile.close();
}

// 從 .txt 檔案讀取測試資料
vector<int> loadDataFromFile(const string& filename) {
    ifstream inFile(filename);
    vector<int> data;
    int num;
    while (inFile >> num) {
        data.push_back(num);
    }
    inFile.close();
    return data;
}

int main() {
    vector<int> ns = {500, 1000, 2000, 3000, 4000, 5000};
    int repeat = 100;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 1000000);

    vector<pair<string, function<vector<int>(vector<int>)>>> sorts = {
        {"InsertionSort", insertionSort},
        {"QuickSort", quickSortWrapper},
        {"MergeSort", mergeSortIterative},  // 使用迭代式合併排序
        {"HeapSort", heapSort},
        {"CompositeSort", compositeSort}
    };

    // 表頭
    cout << left << setw(8)  << "n"
         << setw(20) << "Method"
         << setw(16) << "Avg Time(ms)"
         << setw(20) << "Working Set Size"
         << setw(24) << "Peak Working Set Size"
         << "Pagefile Usage" << endl;

    cout << string(88, '-') << endl;

    // 輸出測試資料至 .txt 檔案
    for (int n : ns) {
        vector<int> data(n);
        for (auto& x : data) x = dis(gen);
        string filename = "test_data_" + to_string(n) + ".txt";
        saveDataToFile(data, filename);  // 儲存測試資料到檔案
    }

    // 讀取資料並執行排序
    for (int n : ns) {
        string filename = "test_data_" + to_string(n) + ".txt";
        vector<int> data = loadDataFromFile(filename);  // 從檔案讀取資料

        for (auto& sortPair : sorts) {
            auto arr = data;
            size_t workingSet = 0, peakWorkingSet = 0, pagefileUsage = 0;

            auto start = chrono::high_resolution_clock::now();
            for (int i = 0; i < repeat; ++i) {
                auto copy = arr;
                auto result = sortPair.second(copy);
            }
            auto end = chrono::high_resolution_clock::now();
            double avgTime = chrono::duration<double, milli>(end - start).count() / repeat;

            printMemoryUsage(workingSet, peakWorkingSet, pagefileUsage);

            cout << left << setw(8)  << n
                 << setw(20) << sortPair.first
                 << setw(16) << fixed << setprecision(2) << avgTime
                 << setw(20) << workingSet << "KB"
                 << setw(24) << peakWorkingSet << "KB"
                 << pagefileUsage << "KB" << endl;
        }
    }

    return 0;
}
```

以下是**worst-case**的標頭檔與排序程式碼:

```CPP
#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>
#include <string>
#include <algorithm>
#include <iomanip>
#include <windows.h>
#include <psapi.h>

using namespace std;

class SortTester {
private:
    // ---------- 排序函數 ----------
    // 插入排序法（Insertion Sort）
    static void insertionSort(vector<int> arr) {
        for (int i = 1; i < arr.size(); ++i) {
            int key = arr[i], j = i - 1;
            while (j >= 0 && arr[j] > key) arr[j + 1] = arr[j--];
            arr[j + 1] = key;
        }
    }

    // 快速排序法中用來選擇三者中位數的基準元素
    static int medianOfThree(vector<int>& a, int l, int r) {
        int m = (l + r) / 2;
        if (a[l] > a[m]) swap(a[l], a[m]);
        if (a[l] > a[r]) swap(a[l], a[r]);
        if (a[m] > a[r]) swap(a[m], a[r]);
        swap(a[m], a[r - 1]); // 將中位數交換到倒數第二個位置作為基準
        return a[r - 1];
    }

    // 快速排序法的實作
    static void quickSortImpl(vector<int>& a, int l, int r) {
        if (l + 10 <= r) {
            int pivot = medianOfThree(a, l, r); // 使用中位數作為基準
            int i = l, j = r - 1;
            while (true) {
                while (a[++i] < pivot) {} // 找到大於基準的元素
                while (a[--j] > pivot) {} // 找到小於基準的元素
                if (i < j) swap(a[i], a[j]); // 交換兩個元素
                else break; // 兩指標交叉停止
            }
            swap(a[i], a[r - 1]); // 將基準放到正確的位置
            quickSortImpl(a, l, i - 1); // 遞迴排序左半部分
            quickSortImpl(a, i + 1, r); // 遞迴排序右半部分
        } else {
            insertionSort(a); // 若區間較小，使用插入排序法處理
        }
    }

    // 快速排序函數
    static void quickSort(vector<int> arr) {
        if (!arr.empty()) {
            quickSortImpl(arr, 0, (int)arr.size() - 1);
        }
    }

    // 合併排序法中的合併操作
    static void mergeRange(vector<int>& a, int l, int m, int r) {
        vector<int> tmp;
        int i = l, j = m + 1;
        while (i <= m && j <= r)
            tmp.push_back(a[i] <= a[j] ? a[i++] : a[j++]); // 比較兩部分的元素並加入結果
        while (i <= m) tmp.push_back(a[i++]); // 若左半部分還有元素，加入結果
        while (j <= r) tmp.push_back(a[j++]); // 若右半部分還有元素，加入結果
        copy(tmp.begin(), tmp.end(), a.begin() + l); // 複製合併後的結果
    }

    // 合併排序法的實作
    static void mergeSortImpl(vector<int>& a, int l, int r) {
        if (l >= r) return; // 若只有一個元素，直接返回
        int m = (l + r) / 2;
        mergeSortImpl(a, l, m); // 遞迴排序左半部分
        mergeSortImpl(a, m + 1, r); // 遞迴排序右半部分
        mergeRange(a, l, m, r); // 合併兩部分
    }

    // 合併排序
    static void mergeSort(vector<int> arr) {
        if (!arr.empty()) {
            mergeSortImpl(arr, 0, (int)arr.size() - 1);
        }
    }

    // 堆排序法中的堆化操作
    static void heapify(vector<int>& a, int n, int i) {
        int largest = i, L = 2 * i + 1, R = 2 * i + 2;
        if (L < n && a[L] > a[largest]) largest = L; // 左子節點比較
        if (R < n && a[R] > a[largest]) largest = R; // 右子節點比較
        if (largest != i) {
            swap(a[i], a[largest]); // 交換元素
            heapify(a, n, largest); // 繼續堆化
        }
    }

    // 堆排序
    static void heapSort(vector<int> arr) {
        int n = arr.size();
        for (int i = n / 2 - 1; i >= 0; --i) heapify(arr, n, i); // 建堆
        for (int i = n - 1; i > 0; --i) {
            swap(arr[0], arr[i]); // 將堆頂元素與最後一個元素交換
            heapify(arr, i, 0); // 重新堆化
        }
    }
```

以下為**worst-case**時間、記憶體測量測資與主程式的程式碼:

```cpp
// ---------- 資料生成 ----------
    // 根據不同排序方法生成最差情況資料
    static vector<int> generateWorstCaseData(int n, const string& method) {
        vector<int> data;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 100000);

        if (method == "insertion" || method == "heap") {
            for (int i = n; i >= 1; --i) data.push_back(i); // 逆序排列資料
        } else if (method == "merge") {
            for (int i = 0; i < n; ++i) data.push_back(dis(gen)); // 隨機資料
        } else if (method == "quick") {
            for (int i = 1; i <= n; ++i) data.push_back(i); // 順序排列資料
        }
        return data;
    }

    // ---------- 輸出與讀取資料 ----------
    // 將資料保存至檔案
    static void saveDataToFile(const vector<int>& data, const string& filename) {
        ofstream outFile(filename);
        for (int num : data) {
            outFile << num << endl; // 每個數字一行
        }
        outFile.close();
    }

    // 從檔案讀取資料
    static vector<int> loadDataFromFile(const string& filename) {
        ifstream inFile(filename);
        vector<int> data;
        int num;
        while (inFile >> num) {
            data.push_back(num); // 將資料讀入陣列
        }
        inFile.close();
        return data;
    }

    // ---------- 記憶體與時間測量 ----------
    // 顯示記憶體資訊
    static void printMemoryUsage(size_t& workingSet, size_t& peakWorkingSet, size_t& pagefileUsage) {
        PROCESS_MEMORY_COUNTERS memInfo;
        GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
        workingSet = memInfo.WorkingSetSize / 1024;  // 轉換成 KB
        peakWorkingSet = memInfo.PeakWorkingSetSize / 1024;  // 轉換成 KB
        pagefileUsage = memInfo.PagefileUsage / 1024;  // 轉換成 KB
    }

    // 計算並返回每次排序的平均時間
    template<typename F>
    static double averageSortTime(const vector<int>& data, F sortFunc) {
        double total = 0;
        for (int i = 0; i < 100; ++i) {
            vector<int> a = data; // 保留原始資料不變
            auto t0 = chrono::steady_clock::now(); // 記錄排序開始時間
            sortFunc(a); // 傳值呼叫排序函數
            auto t1 = chrono::steady_clock::now(); // 記錄排序結束時間
            total += chrono::duration<double, milli>(t1 - t0).count(); // 計算時間差並累加
        }
        return total / 100.0; // 返回100次測試的平均時間
    }

    // 優化後的通用排序統計函數
    template<typename F>
    static void printSortStats(const string& method, const vector<int>& data, F sortFunc, int n) {
        double t = averageSortTime(data, sortFunc); // 計算平均排序時間
        size_t ws, peak, pf;
        printMemoryUsage(ws, peak, pf); // 顯示記憶體資訊
        // 輸出統計結果
        cout << setw(5) << n << "\t" 
             << setw(15) << method << "\t"
             << fixed << setprecision(2) << setw(10) << t << "\t"
             << setw(8) << ws << " KB\t"
             << setw(8) << peak << " KB\t"
             << setw(8) << pf << " KB\n";
    }

public:
    // 執行所有測試並顯示結果
    static void runTests(const vector<int>& Ns) {
        cout << "n\tMethod\t\tAvg Time(ms)\tWorking Set Size\tPeak Working Set Size\tPagefile Usage\n";
        cout << "-------------------------------------------------------------------------------------------\n";

        // 迭代不同的資料大小進行測試
        for (int n : Ns) {
            // 生成測試資料並保存到檔案
            auto v1 = generateWorstCaseData(n, "insertion");
            saveDataToFile(v1, "insertion_Worstdata.txt");

            auto v2 = generateWorstCaseData(n, "quick");
            saveDataToFile(v2, "quick_Worstdata.txt");

            auto v3 = generateWorstCaseData(n, "merge");
            saveDataToFile(v3, "merge_Worstdata.txt");

            auto v4 = generateWorstCaseData(n, "heap");
            saveDataToFile(v4, "heap_Worstdata.txt");

            // 讀取檔案並排序
            v1 = loadDataFromFile("insertion_Worstdata.txt");
            v2 = loadDataFromFile("quick_Worstdata.txt");
            v3 = loadDataFromFile("merge_Worstdata.txt");
            v4 = loadDataFromFile("heap_Worstdata.txt");

            // 輸出排序統計結果
            printSortStats("InsertionSort", v1, insertionSort, n);
            printSortStats("QuickSort", v2, quickSort, n);
            printSortStats("MergeSort", v3, mergeSort, n);
            printSortStats("HeapSort", v4, heapSort, n);
        }
    }
};

int main() {
    vector<int> Ns = {500, 1000, 2000, 3000, 4000, 5000}; // 設定不同的測試資料大小
    SortTester::runTests(Ns); // 執行測試
    return 0;
}
```

## 效能分析

1. average-case的時間複雜度：
   InsertionSort​：O(n²)
   QuickSort​：O(n log n)
   MergeSort​：O(n log n)
   HeapSort​：O(n log n)
   CompositeSort：O(n log n)
2. average-case的空間複雜度:
   InsertionSort：O(1)
   QuickSort：O(log n)
   MergeSort：O(n)
   HeapSort：O(1)
   CompositeSort：O(n)
3. worst-case的時間複雜度:
    InsertionSort: O(n²)
    QuickSort: O(n log n
    MergeSort: O(n log n)
    HeapSort: O(n log n)

4. worst-case的空間複雜度:
   HeapSort :   O(1)
   MergeSort:   O(n)
   QuickSort:    O(log n)
   InsertionSort:   O(1)


## 測試與驗證

average-case:


| 測資   | InsertionSort | QuickSort | MergeSort | HeapSort | CompositeSort |
| ------ | --------------- | ----------- | ----------- | ---------- | --------------- |
| 500  | 0.38          | 0.04      | 1.21      | 0.12     | 0.05          |
| 1000 | 1.48          | 0.10      | 2.11      | 0.26     | 0.11          |
| 2000 | 5.88          | 0.23      | 4.29      | 0.56     | 0.25          |
| 3000 | 13.53         | 0.35      | 6.54      | 0.88     | 0.39          |
| 4000 | 23.30         | 0.51      | 8.39      | 1.22     | 0.54          |
| 5000 | 37.64         | 0.64      | 11.35     | 1.61     | 0.70          |

| 排序方法                | 平均時間（ms） | 理論時間複雜度       | 實際時間複雜度                          |
| ------------------------- | ---------------- | ---------------------- | ----------------------------------------- |
| **InsertionSort** | 13.12          | O(n²)               | O(n²)                                  |
| **QuickSort**     | 0.28           | O(n log n)           | O(n log n)                              |
| **MergeSort**     | 5.48           | O(n log n)           | O(n log n)                              |
| **HeapSort**      | 0.78           | O(n log n)           | O(n log n)                              |

worst-case:

| 測資   | InsertionSort | QuickSort | MergeSort | HeapSort |
| ------ | --------------- | ----------- | ----------- | ---------- |
| 500  | 0.68            | 0.64      | 0.44      | 0.10     |
| 1000 | 2.71           | 2.49      | 0.91      | 0.22     |
| 2000 | 10.73         | 9.86      | 1.87      | 0.47     |
| 3000 | 23.97         | 25.11     | 3.19      | 0.74     |
| 4000 | 42.90         | 38.93     | 3.90      | 1.01     |
| 5000 | 66.60         | 48.86     | 5.89      | 1.29     |

| 排序方法                | 平均時間（ms） | 理論時間複雜度 | 實際時間複雜度 |
| ------------------------- | ---------------- | ---------------- | ---------------- |
| **InsertionSort** | 24.43          | O(n²)         | O(n²)         |
| **QuickSort**     | 25.48          |  O(n²)  | O(n log n)     |
| **MergeSort**     | 2.45           | O(n log n)     | O(n log n)     |
| **HeapSort**      | 0.62           | O(n log n)     | O(n log n)     |

### 編譯與執行指令

```shell
g++ -std=c++11 -static-libgcc -lpsapi average-case.cpp -o average-case
```

```shell
g++ -std=c++11 -static-libgcc -lpsapi worstcase.cpp -o worstcase
```

### 結論

1. 計時方式探討說明

在chrono上，我使用了std::chrono::high_resolution_clock，他是負責高精度測量的，他能夠承擔average-case跟worstcase的大量計算測資，std::chrono::high_resolution_clock能夠準確知道這些排序跑完測資的時間，不會造成誤差。
而std::chrono::duration\_cast是負責把時間差轉換為「微秒」單位。
範例:

```cpp
#include <iostream>
#include <chrono>
using namespace std;

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    // 假設這裡是排序程式或其他測試
    for (int i = 0; i < 1000000; ++i) { /* 模擬工作 */ }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "執行時間為: " << duration.count() << " 微秒" << endl;
```

2. Heap Sort 的測試資料產生細節:
   我們的測資是在generateWorstCaseData裡面產生的，當method == "heap" 時，                               generateWorstCaseData 函數會生成 n 個隨機的數字，使用 std::random_device和 std::mt19937（梅森旋轉演算法）確保每次測試的測資是隨機的，這樣測試就不會有特定順序（例如逆序或順序）而偏向某些排序方法，uniform_int_distribution<> dis(1, 100000) 則是用來確保隨機數字是介於 1和 100000之間。
   因為有五個測資500, 1000, 2000, 3000, 4000, 5000，所以每一次呼叫
   generateWorstCaseData總共隨機產生了六次。
3. 說明 Merge Sort 的測試資料產生細節:
   同樣是在generateWorstCaseData裡面，當method == "merge" 時，generateWorstCaseData 函數會生成 n 個隨機的數字，也使用 std::random_device和 std::mt19937（梅森旋轉演算法）確保每次測試的測資是隨機的，uniform_int_distribution<> dis(1, 100000) 則是用來確保隨機數字是介於 1和 100000之間。
   
   ## 申論及開發報告

我的排序都是按照網路上的教法以及課本，比較麻煩的是在寫這些程式的過程中，我們用來打程式跟報告的電腦壞掉，花了一些找了其他人借電腦才完成。

在過程中我們沒辦法把兩個情況的程式組在一起，再加上時間的來不及，所以採用了分成兩個.CPP檔，不過我們是分開來做的，所以worst-case是用到了CLASS來方便管理排序跟時間跟記憶體，而average-caseep沒有注意到這個點，所以比較雜亂。





