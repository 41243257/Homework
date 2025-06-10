#include <iostream>
#include <cmath>

using namespace std;

// �p�� I/O �`��J�ɶ��禡
double calculateIOTime(long long n, int S, int k, double ts, double tl, double t) {
    // Buffer �j�p
    double B = (double)S / (k + 1);
    // I/O ����
    long long num_io = (long long)ceil(n / B);
    // �p�� I/O �ɶ�
    return num_io * (ts + tl) + n * t;
}

int main() {
    
    const long long n = 1000000;   // �`������
    const int m = 16;             // runs �ƶq
    const int S = 4000;           // �O����e�q (records)
    const double ts = 0.05;       // seek time (��)
    const double tl = 0.01;       // latency time (��)
    const double t = 0.0005;       // �ǿ�ɶ� (��/����)
    const double tCPU = 300;    // CPU �X�֮ɶ� (��)

    int k = m;

    // �p�� I/O ��J�ɶ�
    double io_time = calculateIOTime(n, S, k, ts, tl, t);

    // �`�ɶ� (I/O + CPU�A���Ҽ{���|)
    double total_time = io_time + tCPU;

    // ��X���G
    cout << "=== �~���ƧǲĤG���q�ɶ����R ===" << endl;
    cout << "�`������ n = " << n << endl;
    cout << "runs �ƶq m = " << m << endl;
    cout << "�O����e�q S = " << S << endl;
    cout << "seek time ts = " << ts << " ��" << endl;
    cout << "latency time tl = " << tl << " ��" << endl;
    cout << "�ǿ�ɶ� t = " << t << " ��/����" << endl;
    cout << "CPU �X�֮ɶ� tCPU = " << tCPU << " ��" << endl;
    cout << "k-way merge k = " << k << endl;
    cout << "Buffer �j�p B = " << (double)S / (k + 1) << " ��" << endl;
    cout << "�` I/O ���� = " << (long long)ceil((double)n / ((double)S / (k + 1))) << endl;
    cout << "-----------------------------" << endl;
    cout << "I/O �`��J�ɶ� = " << io_time << " ��" << endl;
    cout << "CPU �X�֮ɶ� = " << tCPU << " ��" << endl;
    cout << "����ĤG���q�`�ɶ� = " << total_time << " ��" << endl;

    return 0;
}


