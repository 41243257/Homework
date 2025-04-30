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
    // ---------- �ƧǨ�� ----------
    // ���J�ƧǪk�]Insertion Sort�^
    static void insertionSort(vector<int> arr) {
        for (int i = 1; i < arr.size(); ++i) {
            int key = arr[i], j = i - 1;
            while (j >= 0 && arr[j] > key) arr[j + 1] = arr[j--];
            arr[j + 1] = key;
        }
    }

    // �ֳt�ƧǪk���Ψӿ�ܤT�̤���ƪ���Ǥ���
    static int medianOfThree(vector<int>& a, int l, int r) {
        int m = (l + r) / 2;
        if (a[l] > a[m]) swap(a[l], a[m]);
        if (a[l] > a[r]) swap(a[l], a[r]);
        if (a[m] > a[r]) swap(a[m], a[r]);
        swap(a[m], a[r - 1]); // �N����ƥ洫��˼ƲĤG�Ӧ�m�@�����
        return a[r - 1];
    }

    // �ֳt�ƧǪk����@
    static void quickSortImpl(vector<int>& a, int l, int r) {
        if (l + 10 <= r) {
            int pivot = medianOfThree(a, l, r); // �ϥΤ���Ƨ@�����
            int i = l, j = r - 1;
            while (true) {
                while (a[++i] < pivot) {} // ���j���Ǫ�����
                while (a[--j] > pivot) {} // ���p���Ǫ�����
                if (i < j) swap(a[i], a[j]); // �洫��Ӥ���
                else break; // ����Х�e����
            }
            swap(a[i], a[r - 1]); // �N��ǩ�쥿�T����m
            quickSortImpl(a, l, i - 1); // ���j�Ƨǥ��b����
            quickSortImpl(a, i + 1, r); // ���j�Ƨǥk�b����
        } else {
            insertionSort(a); // �Y�϶����p�A�ϥδ��J�ƧǪk�B�z
        }
    }

    // �ֳt�ƧǨ��
    static void quickSort(vector<int> arr) {
        if (!arr.empty()) {
            quickSortImpl(arr, 0, (int)arr.size() - 1);
        }
    }

    // �X�ֱƧǪk�����X�־ާ@
    static void mergeRange(vector<int>& a, int l, int m, int r) {
        vector<int> tmp;
        int i = l, j = m + 1;
        while (i <= m && j <= r)
            tmp.push_back(a[i] <= a[j] ? a[i++] : a[j++]); // ����ⳡ���������å[�J���G
        while (i <= m) tmp.push_back(a[i++]); // �Y���b�����٦������A�[�J���G
        while (j <= r) tmp.push_back(a[j++]); // �Y�k�b�����٦������A�[�J���G
        copy(tmp.begin(), tmp.end(), a.begin() + l); // �ƻs�X�᪺֫���G
    }

    // �X�ֱƧǪk����@
    static void mergeSortImpl(vector<int>& a, int l, int r) {
        if (l >= r) return; // �Y�u���@�Ӥ����A������^
        int m = (l + r) / 2;
        mergeSortImpl(a, l, m); // ���j�Ƨǥ��b����
        mergeSortImpl(a, m + 1, r); // ���j�Ƨǥk�b����
        mergeRange(a, l, m, r); // �X�֨ⳡ��
    }

    // �X�ֱƧ�
    static void mergeSort(vector<int> arr) {
        if (!arr.empty()) {
            mergeSortImpl(arr, 0, (int)arr.size() - 1);
        }
    }

    // ��ƧǪk������ƾާ@
    static void heapify(vector<int>& a, int n, int i) {
        int largest = i, L = 2 * i + 1, R = 2 * i + 2;
        if (L < n && a[L] > a[largest]) largest = L; // ���l�`�I���
        if (R < n && a[R] > a[largest]) largest = R; // �k�l�`�I���
        if (largest != i) {
            swap(a[i], a[largest]); // �洫����
            heapify(a, n, largest); // �~����
        }
    }

    // ��Ƨ�
    static void heapSort(vector<int> arr) {
        int n = arr.size();
        for (int i = n / 2 - 1; i >= 0; --i) heapify(arr, n, i); // �ذ�
        for (int i = n - 1; i > 0; --i) {
            swap(arr[0], arr[i]); // �N�ﳻ�����P�̫�@�Ӥ����洫
            heapify(arr, i, 0); // ���s���
        }
    }

    // ---------- ��ƥͦ� ----------
    // �ھڤ��P�ƧǤ�k�ͦ��̮t���p���
    static vector<int> generateWorstCaseData(int n, const string& method) {
        vector<int> data;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 100000);

        if (method == "insertion" || method == "heap") {
            for (int i = n; i >= 1; --i) data.push_back(i); // �f�ǱƦC���
        } else if (method == "merge") {
            for (int i = 0; i < n; ++i) data.push_back(dis(gen)); // �H�����
        } else if (method == "quick") {
            for (int i = 1; i <= n; ++i) data.push_back(i); // ���ǱƦC���
        }
        return data;
    }

    // ---------- ��X�PŪ����� ----------
    // �N��ƫO�s���ɮ�
    static void saveDataToFile(const vector<int>& data, const string& filename) {
        ofstream outFile(filename);
        for (int num : data) {
            outFile << num << endl; // �C�ӼƦr�@��
        }
        outFile.close();
    }

    // �q�ɮ�Ū�����
    static vector<int> loadDataFromFile(const string& filename) {
        ifstream inFile(filename);
        vector<int> data;
        int num;
        while (inFile >> num) {
            data.push_back(num); // �N���Ū�J�}�C
        }
        inFile.close();
        return data;
    }

    // ---------- �O����P�ɶ����q ----------
    // ��ܰO�����T
    static void printMemoryUsage(size_t& workingSet, size_t& peakWorkingSet, size_t& pagefileUsage) {
        PROCESS_MEMORY_COUNTERS memInfo;
        GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
        workingSet = memInfo.WorkingSetSize / 1024;  // �ഫ�� KB
        peakWorkingSet = memInfo.PeakWorkingSetSize / 1024;  // �ഫ�� KB
        pagefileUsage = memInfo.PagefileUsage / 1024;  // �ഫ�� KB
    }

    // �p��ê�^�C���ƧǪ������ɶ�
    template<typename F>
    static double averageSortTime(const vector<int>& data, F sortFunc) {
        double total = 0;
        for (int i = 0; i < 100; ++i) {
            vector<int> a = data; // �O�d��l��Ƥ���
            auto t0 = chrono::steady_clock::now(); // �O���ƧǶ}�l�ɶ�
            sortFunc(a); // �ǭȩI�s�ƧǨ��
            auto t1 = chrono::steady_clock::now(); // �O���Ƨǵ����ɶ�
            total += chrono::duration<double, milli>(t1 - t0).count(); // �p��ɶ��t�ò֥[
        }
        return total / 100.0; // ��^100�����ժ������ɶ�
    }

    // �u�ƫ᪺�q�αƧǲέp���
    template<typename F>
    static void printSortStats(const string& method, const vector<int>& data, F sortFunc, int n) {
        double t = averageSortTime(data, sortFunc); // �p�⥭���ƧǮɶ�
        size_t ws, peak, pf;
        printMemoryUsage(ws, peak, pf); // ��ܰO�����T
        // ��X�έp���G
        cout << setw(5) << n << "\t" 
             << setw(15) << method << "\t"
             << fixed << setprecision(2) << setw(10) << t << "\t"
             << setw(8) << ws << " KB\t"
             << setw(8) << peak << " KB\t"
             << setw(8) << pf << " KB\n";
    }

public:
    // ����Ҧ����ը���ܵ��G
    static void runTests(const vector<int>& Ns) {
        cout << "n\tMethod\t\tAvg Time(ms)\tWorking Set Size\tPeak Working Set Size\tPagefile Usage\n";
        cout << "-------------------------------------------------------------------------------------------\n";

        // ���N���P����Ƥj�p�i�����
        for (int n : Ns) {
            // �ͦ����ո�ƨëO�s���ɮ�
            auto v1 = generateWorstCaseData(n, "insertion");
            saveDataToFile(v1, "insertion_Worstdata.txt");

            auto v2 = generateWorstCaseData(n, "quick");
            saveDataToFile(v2, "quick_Worstdata.txt");

            auto v3 = generateWorstCaseData(n, "merge");
            saveDataToFile(v3, "merge_Worstdata.txt");

            auto v4 = generateWorstCaseData(n, "heap");
            saveDataToFile(v4, "heap_Worstdata.txt");

            // Ū���ɮרñƧ�
            v1 = loadDataFromFile("insertion_Worstdata.txt");
            v2 = loadDataFromFile("quick_Worstdata.txt");
            v3 = loadDataFromFile("merge_Worstdata.txt");
            v4 = loadDataFromFile("heap_Worstdata.txt");

            // ��X�Ƨǲέp���G
            printSortStats("InsertionSort", v1, insertionSort, n);
            printSortStats("QuickSort", v2, quickSort, n);
            printSortStats("MergeSort", v3, mergeSort, n);
            printSortStats("HeapSort", v4, heapSort, n);
        }
    }
};

int main() {
    vector<int> Ns = {500, 1000, 2000, 3000, 4000, 5000}; // �]�w���P�����ո�Ƥj�p
    SortTester::runTests(Ns); // �������
    return 0;
}
