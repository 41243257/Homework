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
