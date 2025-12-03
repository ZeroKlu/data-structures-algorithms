static IList<int> KSmallest(int[] arr, int k) {
    var pq = new PriorityQueue<int, int>(
        Comparer<int>.Create((a, b) => b.CompareTo(a))  // max-heap
    );
    foreach (var x in arr) {
        if (pq.Count < k) {
            pq.Enqueue(x, x);
        } else if (x < pq.Peek()) {
            pq.Dequeue();
            pq.Enqueue(x, x);
        }
    }
    var result = new List<int>();
    while (pq.Count > 0) {
        result.Add(pq.Dequeue());
    }
    return result;
}

// Safe wrapper to handle k <= 0 or empty arrays
static IList<int> KSmallestSafe(int[] arr, int k) {
    if (k <= 0 || arr == null || arr.Length == 0)
        return new List<int>();
    return KSmallest(arr, k);
}

// --------- Helpers ---------
static void PrintArray(string label, int[] arr) {
    Console.Write(label);
    foreach (var x in arr) Console.Write(x + " ");
    Console.WriteLine();
}

static void PrintList(string label, IEnumerable<int> list) {
    Console.Write(label);
    foreach (var x in list) Console.Write(x + " ");
    Console.WriteLine();
}

// This is the function that tests KSmallest / KSmallestSafe
static void TestKSmallest() {
    var arr = new[] { 7, 2, 9, 4, 1, 5, 8, 3, 6 };

    // ---- Test 1: k <= n ----
    Console.WriteLine("=== Test 1: k <= n ===");
    PrintArray("Input array: ", arr);
    var k = 4;
    Console.WriteLine("k = " + k);

    var res1 = KSmallestSafe(arr, k);
    PrintList("KSmallest result (heap order): ", res1);
    var sorted1 = new List<int>(res1);
    sorted1.Sort();
    PrintList("KSmallest result (sorted): ", sorted1);
    Console.WriteLine("(Expected: 1 2 3 4)");
    Console.WriteLine();

    // ---- Test 2: k > n ----
    Console.WriteLine("=== Test 2: k > n ===");
    k = 20;
    PrintArray("Input array: ", arr);
    Console.WriteLine("k = " + k);

    var res2 = KSmallestSafe(arr, k);
    PrintList("KSmallest result (heap order): ", res2);
    var sorted2 = new List<int>(res2);
    sorted2.Sort();
    PrintList("KSmallest result (sorted): ", sorted2);
    Console.WriteLine("(Expected: all elements sorted)");
    Console.WriteLine();

    // ---- Test 3: k = 0 ----
    Console.WriteLine("=== Test 3: k = 0 ===");
    k = 0;
    Console.WriteLine("k = " + k);

    var res3 = KSmallestSafe(arr, k);
    PrintList("KSmallest result: ", res3);
    Console.WriteLine("(Expected: empty list)");
    Console.WriteLine();
}

TestKSmallest();
