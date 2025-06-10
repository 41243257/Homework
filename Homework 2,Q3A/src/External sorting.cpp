#include <iostream>
#include <cmath>

using namespace std;

// 計算 I/O 總輸入時間函式
double calculateIOTime(long long n, int S, int k, double ts, double tl, double t) {
    // Buffer 大小
    double B = (double)S / (k + 1);
    // I/O 次數
    long long num_io = (long long)ceil(n / B);
    // 計算 I/O 時間
    return num_io * (ts + tl) + n * t;
}

int main() {
    
    const long long n = 1000000;   // 總紀錄數
    const int m = 16;             // runs 數量
    const int S = 4000;           // 記憶體容量 (records)
    const double ts = 0.05;       // seek time (秒)
    const double tl = 0.01;       // latency time (秒)
    const double t = 0.0005;       // 傳輸時間 (秒/紀錄)
    const double tCPU = 300;    // CPU 合併時間 (秒)

    int k = m;

    // 計算 I/O 輸入時間
    double io_time = calculateIOTime(n, S, k, ts, tl, t);

    // 總時間 (I/O + CPU，不考慮重疊)
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
    cout << "Buffer 大小 B = " << (double)S / (k + 1) << " 筆" << endl;
    cout << "總 I/O 次數 = " << (long long)ceil((double)n / ((double)S / (k + 1))) << endl;
    cout << "-----------------------------" << endl;
    cout << "I/O 總輸入時間 = " << io_time << " 秒" << endl;
    cout << "CPU 合併時間 = " << tCPU << " 秒" << endl;
    cout << "整體第二階段總時間 = " << total_time << " 秒" << endl;

    return 0;
}


