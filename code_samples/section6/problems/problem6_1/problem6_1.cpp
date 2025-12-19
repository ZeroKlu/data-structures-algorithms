#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

/*
    MinHeap (Priority Queue) for integers using a contiguous array (std::vector).

    Key min-heap invariant:
        For every node i (except the root), data[parent(i)] <= data[i].

    With this property:
        - Minimum element is always at data[0].
        - insert: O(log n) via bubbleUp
        - peek:   O(1)
        - pop:    O(log n) via heapifyDown

    Array index mapping (0-based):
        parent(i) = (i - 1) / 2
        left(i)   = 2*i + 1
        right(i)  = 2*i + 2

    Error handling:
        - This implementation throws std::runtime_error when peek/pop are called
          on an empty heap. (This avoids the "sentinel value" ambiguity.)
*/

class MinHeap {
    /*
        The heap storage.

        We store elements in "heap order" within a vector:
            - data[0] is the root (minimum)
            - children / parent are computed via index math
    */
    vector<int> data;

    /*
        bubbleUp
        --------
        Restores the heap property after an insertion.

        Insertion strategy:
            1) push_back(newValue) places the value at the end.
            2) That new value may be smaller than its parent, violating the invariant.
            3) bubbleUp repeatedly swaps the value with its parent until:
                 - it reaches the root, OR
                 - it is >= its parent.

        Complexity: O(log n) in the worst case (heap height).
    */
    void bubbleUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;          // parent index
            if (data[i] >= data[p]) break; // heap property satisfied
            swap(data[i], data[p]);        // move smaller value upward
            i = p;                         // continue at parent's position
        }
    }

    /*
        heapifyDown
        -----------
        Restores the heap property after removing the minimum element.

        Removal strategy (pop):
            1) Save root (the minimum).
            2) Move last element into root position.
            3) Remove the last element (now duplicated at root).
            4) The moved element might be larger than its children, violating the invariant.
            5) heapifyDown swaps it downward with the smaller of its children until:
                 - it is <= both children, OR
                 - it becomes a leaf.

        Complexity: O(log n) worst case.
    */
    void heapifyDown(int i) {
        int n = (int)data.size(); // current number of elements
        while (true) {
            int smallest = i;     // assume current node is smallest
            int left = 2 * i + 1; // left child index
            int right = 2 * i + 2;// right child index

            // If left child exists and is smaller, it becomes the candidate
            if (left < n && data[left] < data[smallest]) smallest = left;

            // If right child exists and is smaller than the best candidate, update
            if (right < n && data[right] < data[smallest]) smallest = right;

            // If neither child is smaller, heap property is satisfied here
            if (smallest == i) break;

            // Otherwise swap with the smallest child and continue downward
            swap(data[i], data[smallest]);
            i = smallest;
        }
    }

public:
    /*
        insert
        ------
        Adds a new value to the heap.

        Steps:
            1) Append value to end.
            2) bubbleUp from the appended index to restore heap invariant.

        Complexity: O(log n).
    */
    void insert(int x) {
        data.push_back(x);
        bubbleUp((int)data.size() - 1);
    }

    /*
        peek
        ----
        Returns the minimum element without removing it.

        If empty:
            - throws std::runtime_error to clearly signal misuse.

        Complexity: O(1).
    */
    int peek() const {
        if (data.empty()) throw runtime_error("empty heap");
        return data[0];
    }

    /*
        pop
        ---
        Removes and returns the minimum element.

        If empty:
            - throws std::runtime_error.

        Steps:
            1) Save root (min).
            2) Replace root with last element.
            3) pop_back() removes last element.
            4) If still non-empty, heapifyDown from root.

        Complexity: O(log n) when non-empty.
    */
    int pop() {
        if (data.empty()) throw runtime_error("empty heap");

        int root = data[0];        // minimum value
        data[0] = data.back();     // move last element to root
        data.pop_back();           // remove last element

        // If elements remain, restore heap property
        if (!data.empty()) heapifyDown(0);

        return root;
    }

    /*
        empty
        -----
        Convenience method to check if the heap has no elements.
        Useful for loops that pop until empty.

        Complexity: O(1).
    */
    bool empty() const { return data.empty(); }
};

/*
    Test harness / demo program.

    Exercises:
        - Insert a fixed set of values, printing the running minimum.
        - Pop all values (should come out in ascending order).
        - Attempt peek/pop on empty heap to confirm exceptions.
        - Reinsert values and pop again.
*/
int main() {
    cout << "=== Testing MinHeap ===\n";

    MinHeap h;
    vector<int> values = {5, 3, 8, 1, 7, 2};

    cout << "Inserting values: ";
    for (int v : values) {
        cout << v << " ";
        h.insert(v);

        // After each insert, the minimum should be available at the root
        cout << "\n  After insert " << v
             << " -> peek (min): " << h.peek();
    }
    cout << "\n\n";

    // Popping repeatedly should produce values in ascending (sorted) order
    cout << "Extracting values in sorted order: ";
    while (!h.empty()) {
        cout << h.pop() << " ";
    }
    cout << "\n\n";

    // Try peeking/popping from an empty heap to test exception behavior
    cout << "Testing peek/pop on empty heap:\n";

    try {
        cout << "peek(): " << h.peek() << "\n";
    } catch (const exception &e) {
        // runtime_error derives from std::exception; e.what() provides message text
        cout << "Caught exception on peek(): " << e.what() << "\n";
    }

    try {
        cout << "pop(): " << h.pop() << "\n";
    } catch (const exception &e) {
        cout << "Caught exception on pop(): " << e.what() << "\n";
    }

    // Confirm the heap can be reused after becoming empty
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
