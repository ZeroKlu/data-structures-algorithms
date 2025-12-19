/*
    This file demonstrates how to find the k smallest elements in an array
    using a MAX-HEAP implemented via .NET's PriorityQueue<TElement, TPriority>.

    Core idea:
      - Maintain a max-heap of size at most k.
      - The heap always contains the k smallest elements seen so far.
      - The largest of those k elements sits at the root (Peek()).
      - When a new element x arrives:
          * If heap has fewer than k elements → insert x.
          * Else if x < current maximum → remove max, insert x.
          * Else → ignore x.

    Time complexity:
      - O(n log k), where n = arr.Length
    Space complexity:
      - O(k)

    Important edge cases:
      - k <= 0 → return empty list
      - k > n  → return all elements
      - arr is null or empty → return empty list
*/

/*
    KSmallest
    ---------
    Returns the k smallest elements from arr using a max-heap.

    NOTE:
      - This method assumes k > 0 and arr is non-null.
      - KSmallestSafe() wraps this method to handle edge cases.
*/
static IList<int> KSmallest(int[] arr, int k) {

    /*
        PriorityQueue<TElement, TPriority> is a MIN-HEAP by default.
        To turn it into a MAX-HEAP, we reverse the comparison:
            b.CompareTo(a)
        This causes larger integers to be treated as "smaller" priorities,
        making Peek() return the maximum element.
    */
    var pq = new PriorityQueue<int, int>(
        Comparer<int>.Create((a, b) => b.CompareTo(a))  // max-heap behavior
    );

    /*
        Iterate through the input array and maintain a heap of size <= k.
    */
    foreach (var x in arr) {

        if (pq.Count < k) {
            // Heap not full yet: just insert the element.
            // Element and priority are both x.
            pq.Enqueue(x, x);

        } else if (x < pq.Peek()) {
            /*
                Heap already has k elements.
                pq.Peek() returns the LARGEST of the k smallest elements so far.
                If x is smaller than that, x belongs in the k smallest set.
            */
            pq.Dequeue();       // Remove the current largest
            pq.Enqueue(x, x);   // Insert the smaller value
        }

        // Else:
        //   x is >= pq.Peek(), so it cannot be among the k smallest elements.
        //   Ignore it.
    }

    /*
        Extract elements from the heap.
        Since this is a max-heap, elements will come out in descending order.
        Order is not guaranteed or required by the problem.
    */
    var result = new List<int>();
    while (pq.Count > 0) {
        result.Add(pq.Dequeue());
    }

    return result;
}

/*
    KSmallestSafe
    -------------
    A defensive wrapper around KSmallest that handles:
      - k <= 0
      - null array
      - empty array

    Returns an empty list in those cases.
*/
static IList<int> KSmallestSafe(int[] arr, int k) {
    if (k <= 0 || arr == null || arr.Length == 0)
        return [];

    return KSmallest(arr, k);
}

// --------- Helpers ---------

/*
    PrintArray
    ----------
    Prints an integer array with a label.
*/
static void PrintArray(string label, int[] arr) {
    Console.Write(label);
    foreach (var x in arr) Console.Write(x + " ");
    Console.WriteLine();
}

/*
    PrintList
    ---------
    Prints any IEnumerable<int> with a label.
*/
static void PrintList(string label, IEnumerable<int> list) {
    Console.Write(label);
    foreach (var x in list) Console.Write(x + " ");
    Console.WriteLine();
}

/*
    TestKSmallest
    -------------
    Test harness that exercises KSmallestSafe with:
      1) k <= n
      2) k > n
      3) k = 0

    For readability, results are also sorted after extraction
    so correctness is easy to visually verify.
*/
static void TestKSmallest() {
    var arr = new[] { 7, 2, 9, 4, 1, 5, 8, 3, 6 };

    // ---- Test 1: k <= n ----
    Console.WriteLine("=== Test 1: k <= n ===");
    PrintArray("Input array: ", arr);

    var k = 4;
    Console.WriteLine("k = " + k);

    var res1 = KSmallestSafe(arr, k);
    PrintList("KSmallest result (heap order): ", res1);

    // Sort for easier verification
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

// Execute the test harness
TestKSmallest();
