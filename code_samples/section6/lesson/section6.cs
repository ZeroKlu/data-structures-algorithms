#nullable enable

/* ============================================================
   ARRAY HEAP HELPERS (for build-heap + heapsort)
   ============================================================ */

/*
Heap stored in an array using standard binary heap indexing:
  parent(i) = (i - 1) / 2
  left(i)   = 2*i + 1
  right(i)  = 2*i + 2

Min-heap property:
  every parent <= its children
*/

static void HeapifyDownArray(int[] a, int n, int i)
{
    // "Heapify down" (a.k.a. sift down):
    // Push the element at index i down until the min-heap property holds.
    //
    // n controls the heap size we consider (heap region is a[0..n-1]).
    while (true)
    {
        int left = 2 * i + 1;   // index of left child
        int right = 2 * i + 2;  // index of right child
        int smallest = i;       // assume current node is smallest

        // If left child exists and is smaller, update smallest
        if (left < n && a[left] < a[smallest]) smallest = left;

        // If right child exists and is smaller than current smallest, update
        if (right < n && a[right] < a[smallest]) smallest = right;

        // If neither child is smaller, heap property is satisfied
        if (smallest == i) break;

        // Otherwise swap current element with the smaller child
        int tmp = a[i];
        a[i] = a[smallest];
        a[smallest] = tmp;

        // Continue heapifying from the child position
        i = smallest;
    }
}

static void BuildMinHeap(int[] a)
{
    // Bottom-up heap construction in O(n):
    // All nodes from n/2 to n-1 are leaves (already heaps),
    // so we start from the last parent (n/2 - 1) down to 0.
    int n = a.Length;
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        HeapifyDownArray(a, n, i);
    }
}

static void HeapsortDescMinHeap(int[] a)
{
    // Heapsort using a MIN-heap results in DESCENDING order:
    // Each step moves the minimum element (root) to the end.
    //
    // Steps:
    // 1) Build min-heap
    // 2) Repeatedly swap root (min) with the last element in the heap region
    // 3) Shrink heap region and heapify down from root

    BuildMinHeap(a);

    for (int end = a.Length - 1; end > 0; end--)
    {
        // Move current min (a[0]) to the end of the active heap region
        (a[end], a[0]) = (a[0], a[end]);

        // Restore min-heap property in the reduced region [0..end-1]
        HeapifyDownArray(a, end, 0);
    }
}

/* ----------------------------
   Helper to print arrays
   ---------------------------- */

void PrintArray(int[] a) => Console.WriteLine(string.Join(" ", a));

/* ============================================================
   TEST HARNESS
   ============================================================ */

int[] valuesToInsert = { 5, 3, 8, 1, 7, 2 };

// --- Test MinHeap.Insert (no extract) ---
Console.WriteLine("=== Testing MinHeap.Insert ===");

var h1 = new MinHeap();

// This test only exercises Insert + heapify-up;
// since MinHeap has no extract/peek, we just ensure it runs.
Console.Write("Inserting into MinHeap (no extract available): ");
foreach (int v in valuesToInsert)
{
    Console.Write(v + " ");
    h1.Insert(v);
}
Console.WriteLine();
Console.WriteLine();

// --- Test MinHeapWithExtract.Insert / ExtractMin ---
Console.WriteLine("=== Testing MinHeapWithExtract Insert / ExtractMin ===");

var h2 = new MinHeapWithExtract();

// Insert elements
Console.Write("Inserting: ");
foreach (int v in valuesToInsert)
{
    Console.Write(v + " ");
    h2.Insert(v);
}
Console.WriteLine();

// Extract until empty; output should be in ascending order
Console.Write("Extracting in sorted order: ");
while (true)
{
    try
    {
        int minVal = h2.ExtractMin();
        Console.Write(minVal + " ");
    }
    catch (InvalidOperationException)
    {
        // Heap is empty => done
        break;
    }
}
Console.WriteLine();
Console.WriteLine();

// --- Test BuildMinHeap ---
Console.WriteLine("=== Testing BuildMinHeap ===");

int[] arr1 = [9, 4, 7, 1, 0, 3, 2];

Console.Write("Original array: ");
PrintArray(arr1);

// Transform arr1 into a min-heap in-place
BuildMinHeap(arr1);

Console.Write("After BuildMinHeap (min-heap array): ");
PrintArray(arr1);
Console.WriteLine();

// --- Test HeapsortDescMinHeap ---
Console.WriteLine("=== Testing HeapsortDescMinHeap (descending sort) ===");

int[] arr2 = [12, 3, 17, 8, 34, 25, 1];

Console.Write("Original array: ");
PrintArray(arr2);

// Sort descending using min-heap heapsort
HeapsortDescMinHeap(arr2);

Console.Write("After HeapsortDescMinHeap (descending): ");
PrintArray(arr2);

/* ============================================================
   MIN-HEAP (INSERT ONLY)
   ============================================================ */

class MinHeap
{
    // Underlying storage: heap elements in level-order
    private readonly List<int> _data = new List<int>();

    // Insert value into the min-heap.
    //
    // Steps:
    // 1) Add to end
    // 2) Heapify up (bubble up) while parent > child
    public void Insert(int value)
    {
        _data.Add(value);

        // i is the index of the new value
        int i = _data.Count - 1;

        // Heapify up
        while (i > 0)
        {
            int parent = (i - 1) / 2;

            // If parent is already <= child, heap property holds
            if (_data[parent] <= _data[i]) break;

            // Otherwise swap with parent and continue upward
            (_data[i], _data[parent]) = (_data[parent], _data[i]);
            i = parent;
        }
    }
}

/* ============================================================
   MIN-HEAP (INSERT + EXTRACT MIN)
   ============================================================ */

class MinHeapWithExtract
{
    // Underlying storage: heap elements in level-order
    private readonly List<int> _data = new List<int>();

    // Insert value into heap (same heapify-up logic as MinHeap)
    public void Insert(int value)
    {
        _data.Add(value);
        int i = _data.Count - 1;

        while (i > 0)
        {
            int parent = (i - 1) / 2;
            if (_data[parent] <= _data[i]) break;
            (_data[i], _data[parent]) = (_data[parent], _data[i]);
            i = parent;
        }
    }

    // Removes and returns the minimum element (root).
    //
    // Steps:
    // 1) Save root value
    // 2) Move last element to root
    // 3) Remove last element
    // 4) Heapify down from root
    public int ExtractMin()
    {
        if (_data.Count == 0)
        {
            throw new InvalidOperationException("heap underflow");
        }

        // Minimum is always at root index 0
        int minVal = _data[0];

        // Grab last element and remove it
        int last = _data[_data.Count - 1];
        _data.RemoveAt(_data.Count - 1);

        // If heap is now empty, return the old min
        if (_data.Count == 0)
        {
            return minVal;
        }

        // Move last element to the root, then heapify down
        _data[0] = last;

        int n = _data.Count;
        int i = 0;

        // Heapify down (bubble down)
        while (true)
        {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;

            // Choose smallest among current node and its children
            if (left < n && _data[left] < _data[smallest])
            {
                smallest = left;
            }
            if (right < n && _data[right] < _data[smallest])
            {
                smallest = right;
            }

            // If current node is already smallest, heap property holds
            if (smallest == i) break;

            // Otherwise swap with smallest child and continue downward
            (_data[smallest], _data[i]) = (_data[i], _data[smallest]);
            i = smallest;
        }

        return minVal;
    }
}
