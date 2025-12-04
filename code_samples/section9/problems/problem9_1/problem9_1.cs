static int LengthOfLIS(int[] nums) {
    var tails = new List<int>();
    foreach (int x in nums) {
        int lo = 0, hi = tails.Count;
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (tails[mid] < x) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        if (lo == tails.Count) {
            tails.Add(x);
        } else {
            tails[lo] = x;
        }
    }
    return tails.Count;
}

void Test(string name, int[] arr, int expected)
{
    int result = LengthOfLIS(arr);
    Console.WriteLine(name);
    Console.WriteLine($"Input: [{string.Join(",", arr)}]");
    Console.WriteLine($"LengthOfLIS = {result} (expected {expected})\n");
}

Console.WriteLine("=== Testing LengthOfLIS ===\n");

Test("Test 1: Increasing sequence", [1, 2, 3, 4, 5], 5);

Test("Test 2: Mixed sequence", [10, 9, 2, 5, 3, 7, 101, 18], 4);

Test("Test 3: All identical", [7, 7, 7, 7, 7], 1);

Test("Test 4: Empty array", Array.Empty<int>(), 0);

Test("Test 5: Decreasing sequence", [9, 7, 5, 3, 1], 1);

Test("Test 6: Random pattern", [4, 10, 4, 3, 8, 9], 3);
