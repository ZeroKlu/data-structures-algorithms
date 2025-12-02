#nullable enable

// ===== Helpers =====

// Print helper
void PrintArray(string label, int[] arr) =>
    Console.WriteLine(label + string.Join(" ", arr));

// Correct popMin for array-based min-heap using prefix size
int PopMin(int[] arr, ref int size) {
    if (size == 0) return int.MinValue;

    int root = arr[0];
    size--;

    if (size > 0) {
        arr[0] = arr[size];
        HeapifyDown(arr, size, 0);
    }

    return root;
}

// Local heapify-down using given size (not arr.Length)
void HeapifyDown(int[] arr, int size, int i) {
    while (true) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && arr[left] < arr[smallest]) smallest = left;
        if (right < size && arr[right] < arr[smallest]) smallest = right;

        if (smallest == i) break;

        (arr[i], arr[smallest]) = (arr[smallest], arr[i]);
        i = smallest;
    }
}

// ===== Your Test =====

int[] arr = [9, 4, 7, 1, 0, 3, 2];

PrintArray("Original array: ", arr);

HeapUtils.BuildHeap(arr);

PrintArray("After BuildHeap (min-heap array): ", arr);

// Extract values to test correctness
Console.WriteLine("Extracting elements in sorted order:");
int[] heapCopy = (int[])arr.Clone();
int size = heapCopy.Length;

List<int> output = new();

while (size > 0) {
    int v = PopMin(heapCopy, ref size);
    output.Add(v);
}

PrintArray("", output.ToArray());

class HeapUtils {
    private static void Heapify(int[] arr, int n, int i) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && arr[left] < arr[smallest]) smallest = left;
        if (right < n && arr[right] < arr[smallest]) smallest = right;
        if (smallest != i) {
            (arr[smallest], arr[i]) = (arr[i], arr[smallest]);
            Heapify(arr, n, smallest);
        }
    }

    public static void BuildHeap(int[] arr) {
        int n = arr.Length;
        for (int i = n / 2 - 1; i >= 0; i--) {
            Heapify(arr, n, i);
        }
    }
}
