// ===== Helper to print arrays =====

/*
    PrintArray
    ----------
    Utility method to print an integer array with a label.

    Parameters:
      - label: text printed before the array contents
      - arr:   array of integers to print

    Output format:
      label followed by each element separated by spaces.
*/
static void PrintArray(string label, int[] arr)
{
    Console.Write(label);
    foreach (var x in arr)
        Console.Write(x + " ");
    Console.WriteLine();
}

// ===== Test Runner =====

/*
    TestKthLargest
    --------------
    Test harness for the KthLargest class.

    Scenario:
      - Initialize KthLargest with k = 3 and initial numbers {4, 5, 8, 2}
      - Add values one by one and print the resulting k-th largest element

    This mirrors the classic "Kth Largest in a Stream" problem.
*/
static void TestKthLargest()
{
    Console.WriteLine("=== Test KthLargest ===");

    // Initial stream values
    int[] nums = [4, 5, 8, 2];
    int k = 3;

    PrintArray("Initial nums: ", nums);
    Console.WriteLine("k = " + k);
    Console.WriteLine();

    // Create the KthLargest object using the initial numbers
    var kth = new KthLargest(k, nums);

    // Values to be added incrementally
    int[] toAdd = [3, 5, 10, 9, 4];

    Console.WriteLine("Adding values:");
    foreach (var val in toAdd)
    {
        int result = kth.Add(val);
        Console.WriteLine($"Add({val}) -> kth largest = {result}");
    }

    /*
        Expected results for k = 3:

            Add(3)  -> 4
            Add(5)  -> 5
            Add(10) -> 5
            Add(9)  -> 8
            Add(4)  -> 8

        Explanation:
          - The PriorityQueue stores only the current 3 largest values.
          - It is a MIN-HEAP, so Peek() always returns the smallest of those 3,
            which corresponds to the 3rd largest overall.
    */
}

// Call the test runner (works in top-level scripts or inside Main)
TestKthLargest();

/*
    KthLargest
    ----------
    Maintains the k-th largest element in a stream of integers.

    Core idea:
      - Keep a MIN-HEAP of size at most k.
      - The heap stores the k largest values seen so far.
      - The smallest value in the heap is the k-th largest overall.
*/
class KthLargest
{
    // Desired rank (k-th largest)
    private readonly int k;

    /*
        PriorityQueue<int, int> pq

        .NET PriorityQueue is a MIN-HEAP by default:
          - The element with the smallest priority is dequeued first.
        Here:
          - Element = value
          - Priority = value
        So pq.Peek() returns the smallest value currently stored.
    */
    private readonly PriorityQueue<int, int> pq;

    /*
        Constructor
        -----------
        Initializes the KthLargest structure.

        Parameters:
          - k:    which largest element to track
          - nums: initial stream values

        Strategy:
          - Reuse Add() logic so initialization and incremental updates
            follow the same rules.
    */
    public KthLargest(int k, int[] nums)
    {
        this.k = k;
        pq = new PriorityQueue<int, int>();

        // Add initial values using the same logic as streaming inserts
        foreach (var n in nums)
        {
            Add(n);
        }
    }

    /*
        Add
        ---
        Adds a new value to the stream and returns the current k-th largest value.

        Algorithm:
          1) If heap size < k:
                - Insert the value unconditionally.
          2) Otherwise:
                - Compare with the smallest value in the heap (pq.Peek()).
                - If val > smallest:
                      * Remove the smallest
                      * Insert val
                - Else:
                      * Ignore val (not in the top k)
          3) Return pq.Peek(), which represents the k-th largest element.

        Time complexity:
          - O(log k) per insertion
        Space complexity:
          - O(k)
    */
    public int Add(int val)
    {
        if (pq.Count < k)
        {
            // Not enough elements yet; just add
            pq.Enqueue(val, val);
        }
        else
        {
            // Heap already contains k elements
            int min = pq.Peek();

            // Only replace if the new value belongs in the top k
            if (val > min)
            {
                pq.Dequeue();
                pq.Enqueue(val, val);
            }
        }

        // The smallest of the k largest values is the k-th largest
        return pq.Peek();
    }
}
