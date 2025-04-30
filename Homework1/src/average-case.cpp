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

// 合併排序（傳值版本）
vector<int> mergeSort(vector<int> arr) {
    if (arr.size() <= 1) return arr;
    int mid = arr.size() / 2;
    vector<int> left(arr.begin(), arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.end());
    left = mergeSort(left);
    right = mergeSort(right);

    vector<int> result;
    size_t i = 0, j = 0;
    while (i < left.size() && j < right.size()) {
        if (left[i] < right[j]) result.push_back(left[i++]);
        else result.push_back(right[j++]);
    }
    while (i < left.size()) result.push_back(left[i++]);
    while (j < right.size()) result.push_back(right[j++]);
    return result;
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
    else return mergeSort(arr);
}

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
        {"MergeSort", mergeSort},
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






