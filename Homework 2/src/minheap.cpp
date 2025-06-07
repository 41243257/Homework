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
    int data[] = { 5, 1, 9 };

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

