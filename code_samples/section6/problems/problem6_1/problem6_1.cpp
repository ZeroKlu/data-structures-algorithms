#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

class MinHeap {
    vector<int> data;

    void bubbleUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (data[i] >= data[p]) break;
            swap(data[i], data[p]);
            i = p;
        }
    }

    void heapifyDown(int i) {
        int n = (int)data.size();
        while (true) {
            int smallest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            if (left < n && data[left] < data[smallest]) smallest = left;
            if (right < n && data[right] < data[smallest]) smallest = right;
            if (smallest == i) break;
            swap(data[i], data[smallest]);
            i = smallest;
        }
    }

public:
    void insert(int x) {
        data.push_back(x);
        bubbleUp((int)data.size() - 1);
    }

    int peek() const {
        if (data.empty()) throw runtime_error("empty heap");
        return data[0];
    }

    int pop() {
        if (data.empty()) throw runtime_error("empty heap");
        int root = data[0];
        data[0] = data.back();
        data.pop_back();
        if (!data.empty()) heapifyDown(0);
        return root;
    }

    bool empty() const { return data.empty(); }
};

int main() {
    cout << "=== Testing MinHeap ===\n";

    MinHeap h;
    vector<int> values = {5, 3, 8, 1, 7, 2};

    cout << "Inserting values: ";
    for (int v : values) {
        cout << v << " ";
        h.insert(v);
        cout << "\n  After insert " << v
             << " -> peek (min): " << h.peek();
    }
    cout << "\n\n";

    cout << "Extracting values in sorted order: ";
    while (!h.empty()) {
        cout << h.pop() << " ";
    }
    cout << "\n\n";

    // Try peeking/popping from an empty heap to test exceptions
    cout << "Testing peek/pop on empty heap:\n";

    try {
        cout << "peek(): " << h.peek() << "\n";
    } catch (const exception &e) {
        cout << "Caught exception on peek(): " << e.what() << "\n";
    }

    try {
        cout << "pop(): " << h.pop() << "\n";
    } catch (const exception &e) {
        cout << "Caught exception on pop(): " << e.what() << "\n";
    }

    cout << "\nReinserting values: 10, 4, 6\n";
    h.insert(10);
    h.insert(4);
    h.insert(6);

    cout << "peek(): " << h.peek() << "\n";

    cout << "Final pop sequence: ";
    while (!h.empty()) {
        cout << h.pop() << " ";
    }
    cout << "\n";

    return 0;
}
