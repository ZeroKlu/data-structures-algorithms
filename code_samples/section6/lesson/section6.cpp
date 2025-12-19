#include <iostream>
#include <vector>
#include <stdexcept>

// ==========================
// MIN-HEAP (INSERT ONLY)
// ==========================
//
// This class demonstrates how to insert into a min-heap using a vector.
// It does NOT provide extraction, so it’s mainly for showing heapify-up.
class MinHeap {
public:
    // Insert a value into the heap.
    //
    // Steps:
    // 1) Push value to the end of the array
    // 2) "Heapify up" (bubble up) until min-heap property holds:
    //    parent <= child
    void insert(int value) {
        // 1) Add new value at the end (next free position)
        data_.push_back(value);

        // i is the index of the newly inserted element
        int i = (int)data_.size() - 1;

        // 2) Heapify up:
        // While the node is smaller than its parent, swap upward.
        while (i > 0) {
            int parent = (i - 1) / 2; // parent index

            // If heap property holds, stop
            if (data_[parent] <= data_[i]) break;

            // Otherwise swap with parent and continue moving up
            std::swap(data_[parent], data_[i]);
            i = parent;
        }
    }

private:
    // Underlying array storing heap elements in level-order
    std::vector<int> data_;
};

// ==========================
// MIN-HEAP (INSERT + EXTRACT MIN)
// ==========================
//
// Same min-heap concept, but also supports extractMin().
// That makes it possible to remove elements in sorted order.
class MinHeapWithExtract {
public:
    // Insert value into the heap (same algorithm as above).
    void insert(int value) {
        data_.push_back(value);
        int i = (int)data_.size() - 1;

        // Heapify up
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (data_[parent] <= data_[i]) break;
            std::swap(data_[parent], data_[i]);
            i = parent;
        }
    }

    // Remove and return the minimum element (root).
    //
    // Steps:
    // 1) Save the root value (minimum)
    // 2) Move last element to the root
    // 3) Pop the last element (shrinks heap)
    // 4) Heapify down to restore heap property
    //
    // Throws if the heap is empty.
    int extractMin() {
        if (data_.empty()) {
            throw std::runtime_error("heap underflow");
        }

        // 1) Minimum is always at index 0
        int minVal = data_.front();

        // 2) Move last element into the root slot
        data_[0] = data_.back();

        // 3) Remove last element
        data_.pop_back();

        // 4) Heapify down from root to restore min-heap property
        int n = (int)data_.size();
        int i = 0;

        while (true) {
            int left = 2 * i + 1;   // left child index
            int right = 2 * i + 2;  // right child index
            int smallest = i;       // assume current node is smallest

            // Compare with left child
            if (left < n && data_[left] < data_[smallest]) {
                smallest = left;
            }

            // Compare with right child
            if (right < n && data_[right] < data_[smallest]) {
                smallest = right;
            }

            // If no child is smaller, heap property holds
            if (smallest == i) break;

            // Otherwise swap down and continue
            std::swap(data_[i], data_[smallest]);
            i = smallest;
        }

        return minVal;
    }

private:
    std::vector<int> data_;
};

// ==========================
// HEAPIFY DOWN (ARRAY VERSION)
// ==========================
//
// Heapify-down procedure on a vector that represents a heap in-place.
// This is used by buildMinHeap() and heapsortDescMinHeap().
//
// Parameters:
// - a: vector holding heap elements
// - n: size of the heap region (a[0..n-1])
// - i: index where heapify-down begins
void heapifyDownArray(std::vector<int>& a, int n, int i) {
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        // Find smallest among node and children
        if (left < n && a[left] < a[smallest]) smallest = left;
        if (right < n && a[right] < a[smallest]) smallest = right;

        // If i is already smallest, stop
        if (smallest == i) break;

        // Swap with the smaller child and continue
        std::swap(a[i], a[smallest]);
        i = smallest;
    }
}

// ==========================
// BUILD MIN-HEAP (BOTTOM-UP)
// ==========================
//
// Converts an arbitrary array into a min-heap in O(n) time.
// Start from the last internal node (n/2 - 1) and heapify-down each.
void buildMinHeap(std::vector<int>& a) {
    int n = (int)a.size();

    // Nodes from n/2 to n-1 are leaves, so start at n/2 - 1
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapifyDownArray(a, n, i);
    }
}

// ==========================
// HEAPSORT (DESCENDING VIA MIN-HEAP)
// ==========================
//
// Heapsort typically uses a max-heap to sort ascending.
// Here we use a min-heap; each iteration moves the minimum to the end,
// producing DESCENDING order.
//
// Steps:
// 1) Build min-heap
// 2) Swap root (min) with last element in heap region
// 3) Shrink heap region by 1
// 4) Heapify down from root
void heapsortDescMinHeap(std::vector<int>& a) {
    buildMinHeap(a);

    // end marks last index of current heap region [0..end]
    for (int end = (int)a.size() - 1; end > 0; end--) {
        // Move current min (a[0]) to its final position at 'end'
        std::swap(a[0], a[end]);

        // Restore heap property in reduced heap [0..end-1]
        heapifyDownArray(a, end, 0);
    }
}

// ==========================
// TEST HARNESS
// ==========================

int main() {
    // ---------- Test MinHeap::insert (no extract) ----------
    std::cout << "=== Testing MinHeap::insert ===\n";

    MinHeap h1;
    std::vector<int> valuesToInsert = { 5, 3, 8, 1, 7, 2 };

    // We can insert, but we can't verify order by extracting
    // (this is just exercising insert + heapify-up).
    std::cout << "Inserting into MinHeap (no extract available): ";
    for (int v : valuesToInsert) {
        std::cout << v << " ";
        h1.insert(v);
    }
    std::cout << "\n\n";

    // ---------- Test MinHeapWithExtract::insert / extractMin ----------
    std::cout << "=== Testing MinHeapWithExtract insert / extractMin ===\n";

    MinHeapWithExtract h2;

    // Insert same values
    std::cout << "Inserting: ";
    for (int v : valuesToInsert) {
        std::cout << v << " ";
        h2.insert(v);
    }
    std::cout << "\n";

    // Extract until heap is empty; output should be sorted ascending
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

    // Convert arr1 into a min-heap in-place
    buildMinHeap(arr1);

    std::cout << "After buildMinHeap (min-heap array): ";
    for (int x : arr1) std::cout << x << " ";
    std::cout << "\n\n";

    // ---------- Test heapsortDescMinHeap ----------
    std::cout << "=== Testing heapsortDescMinHeap (descending sort) ===\n";

    std::vector<int> arr2 = { 12, 3, 17, 8, 34, 25, 1 };

    std::cout << "Original array: ";
    for (int x : arr2) std::cout << x << " ";
    std::cout << "\n";

    // Sort descending using min-heap heapsort
    heapsortDescMinHeap(arr2);

    std::cout << "After heapsortDescMinHeap (descending): ";
    for (int x : arr2) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
