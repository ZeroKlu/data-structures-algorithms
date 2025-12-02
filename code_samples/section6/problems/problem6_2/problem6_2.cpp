#include <iostream>
#include <vector>
using namespace std;

static void heapify(vector<int> &arr, int n, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left] < arr[smallest]) smallest = left;
    if (right < n && arr[right] < arr[smallest]) smallest = right;
    if (smallest != i) {
        swap(arr[i], arr[smallest]);
        heapify(arr, n, smallest);
    }
}

void buildHeap(vector<int> &arr) {
    int n = (int)arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

static int popMinFromHeapArray(vector<int> &arr, int &n) {
    // Simple pop from array-based min-heap (not part of spec, just for testing)
    if (n == 0) return 0;
    int root = arr[0];
    n--;
    if (n > 0) {
        arr[0] = arr[n];
        heapify(arr, n, 0);
    }
    return root;
}

static void printArray(const string &label, const vector<int> &arr) {
    cout << label;
    for (int x : arr) cout << x << " ";
    cout << "\n";
}

int main() {
    vector<int> arr1 = {9, 4, 7, 1, 0, 3, 2};
    int n1 = arr1.size();

    printArray("Original array: ", arr1);

    buildHeap(arr1);

    printArray("After buildHeap (min-heap array): ", arr1);

    cout << "Extracting elements in sorted order: ";
    int size = n1;
    // Use a temporary copy since extraction mutates the heap
    vector<int> heapCopy = arr1;
    while (size > 0) {
        int x = popMinFromHeapArray(heapCopy, size);
        cout << x << " ";
    }
    cout << "\n";

    return 0;
}
