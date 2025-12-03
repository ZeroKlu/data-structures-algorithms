// ===== Helper to print arrays =====
static void PrintArray(string label, int[] arr)
{
    Console.Write(label);
    foreach (var x in arr) Console.Write(x + " ");
    Console.WriteLine();
}

// ===== Test Runner =====
static void TestKthLargest()
{
    Console.WriteLine("=== Test KthLargest ===");

    int[] nums = { 4, 5, 8, 2 };
    int k = 3;

    PrintArray("Initial nums: ", nums);
    Console.WriteLine("k = " + k);
    Console.WriteLine();

    var kth = new KthLargest(k, nums);

    int[] toAdd = { 3, 5, 10, 9, 4 };

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

        Explanation: the PriorityQueue is a min-heap of the current 3 largest values.
    */
}

// Call the test runner (works in top-level scripts or inside Main)
TestKthLargest();

class KthLargest {
    private readonly int k;
    private readonly PriorityQueue<int, int> pq; // min-heap

    public KthLargest(int k, int[] nums) {
        this.k = k;
        pq = new PriorityQueue<int, int>();
        foreach (var n in nums) {
            Add(n);
        }
    }

    public int Add(int val) {
        if (pq.Count < k) {
            pq.Enqueue(val, val);
        } else {
            int min = pq.Peek();
            if (val > min) {
                pq.Dequeue();
                pq.Enqueue(val, val);
            }
        }
        return pq.Peek();
    }
}
