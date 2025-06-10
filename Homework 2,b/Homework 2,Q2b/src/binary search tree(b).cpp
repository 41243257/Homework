#include <iostream>
#include <cmath> // for ceil(log2(m) / log2(k))

// Function to compute total input time for k-way merge phase
double compute_total_input_time(
    int n,        // Total records to sort
    int S,        // Memory capacity (records)
    int m,        // Initial number of runs
    int k,        // k-way merge
    double ts,    // Seek time per disk access
    double t,     // Latency + transmission time per record
    int b         // Block size (records per buffer)
) {
    // Compute the number of merge passes (log_k m)
    double num_passes = std::ceil(std::log(m) / std::log(k));

    // Total blocks read = (n / b) * passes
    double total_blocks = (n / static_cast<double>(b)) * num_passes;

    // Time per block = seek + latency + transmission (b records)
    double time_per_block = ts + t + (b * t);

    // Total input time = blocks * time per block
    return total_blocks * time_per_block;
}

int main() {
    // Example usage
    int n = 1.000.000;  // 1M records
    int S = 10.000;     // Memory holds 10K records
    int m = 100;        // 100 initial runs
    int k = 4;          // 4-way merge
    double ts = 5.0;    // 5ms seek time
    double t = 0.01;    // 0.01ms per record (latency + transmission)
    int b = 1000;       // Block size (1K records per buffer)

    double total_time = compute_total_input_time(n, S, m, k, ts, t, b);

    std::cout << "Total input time for merge phase: " 
              << total_time << " ms" << std::endl;

    return 0;
}



