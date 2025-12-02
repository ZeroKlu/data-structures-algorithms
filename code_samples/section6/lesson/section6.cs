#nullable enable

static void HeapifyDownArray(int[] a, int n, int i) {
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left < n && a[left] < a[smallest]) smallest = left;
        if (right < n && a[right] < a[smallest]) smallest = right;
        if (smallest == i) break;
        int tmp = a[i];
        a[i] = a[smallest];
        a[smallest] = tmp;
        i = smallest;
    }
}

static void BuildMinHeap(int[] a) {
    int n = a.Length;
    for (int i = n / 2 - 1; i >= 0; i--) {
        HeapifyDownArray(a, n, i);
    }
}

static void HeapsortDescMinHeap(int[] a) {
    BuildMinHeap(a);
    for (int end = a.Length - 1; end > 0; end--) {
        int tmp = a[0];
        a[0] = a[end];
        a[end] = tmp;
        HeapifyDownArray(a, end, 0);
    }
}

// --- Helper to print arrays ---
void PrintArray(int[] a) => Console.WriteLine(string.Join(" ", a));

int[] valuesToInsert = { 5, 3, 8, 1, 7, 2 };

// --- Test MinHeap.Insert (no extract) ---
Console.WriteLine("=== Testing MinHeap.Insert ===");

var h1 = new MinHeap();
Console.Write("Inserting into MinHeap (no extract available): ");
foreach (int v in valuesToInsert) {
    Console.Write(v + " ");
    h1.Insert(v); // just exercising insert
}
Console.WriteLine();
Console.WriteLine();

// --- Test MinHeapWithExtract.Insert / ExtractMin ---
Console.WriteLine("=== Testing MinHeapWithExtract Insert / ExtractMin ===");

var h2 = new MinHeapWithExtract();
Console.Write("Inserting: ");
foreach (int v in valuesToInsert) {
    Console.Write(v + " ");
    h2.Insert(v);
}
Console.WriteLine();

Console.Write("Extracting in sorted order: ");
while (true) {
    try {
        int minVal = h2.ExtractMin();
        Console.Write(minVal + " ");
    }
    catch (InvalidOperationException) {
        break; // heap is empty
    }
}
Console.WriteLine();
Console.WriteLine();

// --- Test BuildMinHeap ---
Console.WriteLine("=== Testing BuildMinHeap ===");

int[] arr1 = { 9, 4, 7, 1, 0, 3, 2 };
Console.Write("Original array: ");
PrintArray(arr1);

BuildMinHeap(arr1);
Console.Write("After BuildMinHeap (min-heap array): ");
PrintArray(arr1);
Console.WriteLine();

// --- Test HeapsortDescMinHeap ---
Console.WriteLine("=== Testing HeapsortDescMinHeap (descending sort) ===");

int[] arr2 = { 12, 3, 17, 8, 34, 25, 1 };
Console.Write("Original array: ");
PrintArray(arr2);

HeapsortDescMinHeap(arr2);
Console.Write("After HeapsortDescMinHeap (descending): ");
PrintArray(arr2);

class MinHeap {
    private readonly List<int> _data = new List<int>();

    public void Insert(int value) {
        _data.Add(value);
        int i = _data.Count - 1;
        // heapify up
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (_data[parent] <= _data[i]) break;
            (_data[i], _data[parent]) = (_data[parent], _data[i]);
            i = parent;
        }
    }
}

class MinHeapWithExtract {
    private readonly List<int> _data = new List<int>();

    public void Insert(int value) {
        _data.Add(value);
        int i = _data.Count - 1;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (_data[parent] <= _data[i]) break;
            (_data[i], _data[parent]) = (_data[parent], _data[i]);
            i = parent;
        }
    }

    public int ExtractMin() {
        if (_data.Count == 0) {
            throw new System.InvalidOperationException("heap underflow");
        }
        int minVal = _data[0];
        int last = _data[_data.Count - 1];
        _data.RemoveAt(_data.Count - 1);
        if (_data.Count == 0) {
            return minVal;
        }
        _data[0] = last;

        int n = _data.Count;
        int i = 0;
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;
            if (left < n && _data[left] < _data[smallest]) {
                smallest = left;
            }
            if (right < n && _data[right] < _data[smallest]) {
                smallest = right;
            }
            if (smallest == i) break;
            (_data[smallest], _data[i]) = (_data[i], _data[smallest]);
            i = smallest;
        }
        return minVal;
    }
}

