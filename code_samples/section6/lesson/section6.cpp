#include <iostream>
#include <vector>
#include <stdexcept>

class MinHeap {
public:
    void insert(int value) {
        data_.push_back(value);
        int i = (int)data_.size() - 1;
        // heapify up
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (data_[parent] <= data_[i]) break;
            std::swap(data_[parent], data_[i]);
            i = parent;
        }
    }

private:
    std::vector<int> data_;
};

class MinHeapWithExtract {
public:
    void insert(int value) {
        data_.push_back(value);
        int i = (int)data_.size() - 1;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (data_[parent] <= data_[i]) break;
            std::swap(data_[parent], data_[i]);
            i = parent;
        }
    }

    int extractMin() {
        if (data_.empty()) {
            throw std::runtime_error("heap underflow");
        }
        int minVal = data_.front();
        data_[0] = data_.back();
        data_.pop_back();

        int n = (int)data_.size();
        int i = 0;
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;
            if (left < n && data_[left] < data_[smallest]) {
                smallest = left;
            }
            if (right < n && data_[right] < data_[smallest]) {
                smallest = right;
            }
            if (smallest == i) break;
            std::swap(data_[i], data_[smallest]);
            i = smallest;
        }
        return minVal;
    }

private:
    std::vector<int> data_;
};

void heapifyDownArray(std::vector<int>& a, int n, int i) {
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left < n && a[left] < a[smallest]) smallest = left;
        if (right < n && a[right] < a[smallest]) smallest = right;
        if (smallest == i) break;
        std::swap(a[i], a[smallest]);
        i = smallest;
    }
}

void buildMinHeap(std::vector<int>& a) {
    int n = (int)a.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapifyDownArray(a, n, i);
    }
}

void heapsortDescMinHeap(std::vector<int>& a) {
    buildMinHeap(a);
    for (int end = (int)a.size() - 1; end > 0; end--) {
        std::swap(a[0], a[end]);
        heapifyDownArray(a, end, 0);
    }
}

int main() {
    // ---------- Test MinHeap::insert (no extract) ----------
    std::cout << "=== Testing MinHeap::insert ===\n";

    MinHeap h1;
    std::vector<int> valuesToInsert = { 5, 3, 8, 1, 7, 2 };

    std::cout << "Inserting into MinHeap (no extract available): ";
    for (int v : valuesToInsert) {
        std::cout << v << " ";
        h1.insert(v);   // just exercising the method
    }
    std::cout << "\n\n";

    // ---------- Test MinHeapWithExtract::insert / extractMin ----------
    std::cout << "=== Testing MinHeapWithExtract insert / extractMin ===\n";

    MinHeapWithExtract h2;

    std::cout << "Inserting: ";
    for (int v : valuesToInsert) {
        std::cout << v << " ";
        h2.insert(v);
    }
    std::cout << "\n";

    std::cout << "Extracting in sorted order: ";
    while (true) {
        try {
            int minVal = h2.extractMin();
            std::cout << minVal << " ";
        } catch (const std::runtime_error&) {
            // heap underflow => done
            break;
        }
    }
    std::cout << "\n\n";

    // ---------- Test buildMinHeap (uses heapifyDownArray) ----------
    std::cout << "=== Testing buildMinHeap ===\n";

    std::vector<int> arr1 = { 9, 4, 7, 1, 0, 3, 2 };

    std::cout << "Original array: ";
    for (int x : arr1) std::cout << x << " ";
    std::cout << "\n";

    buildMinHeap(arr1);

    std::cout << "After buildMinHeap (min-heap array): ";
    for (int x : arr1) std::cout << x << " ";
    std::cout << "\n\n";

    // ---------- Test heapsortDescMinHeap (also uses buildMinHeap & heapifyDownArray) ----------
    std::cout << "=== Testing heapsortDescMinHeap (descending sort) ===\n";

    std::vector<int> arr2 = { 12, 3, 17, 8, 34, 25, 1 };

    std::cout << "Original array: ";
    for (int x : arr2) std::cout << x << " ";
    std::cout << "\n";

    heapsortDescMinHeap(arr2);

    std::cout << "After heapsortDescMinHeap (descending): ";
    for (int x : arr2) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
