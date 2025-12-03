using System;
using System.Collections.Generic;
using System.Linq;

// ===========================================================
// Functions Under Test
// ===========================================================

static Dictionary<string, int> CountWords(IEnumerable<string> words) {
    var freq = new Dictionary<string, int>();
    foreach (var w in words) {
        if (freq.ContainsKey(w)) freq[w]++;
        else freq[w] = 1;
    }
    return freq;
}

static int? FirstDuplicate(int[] arr) {
    var seen = new HashSet<int>();
    foreach (var x in arr) {
        if (!seen.Add(x)) return x;  // Add returns false if duplicate
    }
    return null;
}

static int[] TwoSum(int[] nums, int target) {
    var indexOf = new Dictionary<int, int>(); // value -> index
    for (int i = 0; i < nums.Length; i++) {
        int need = target - nums[i];
        if (indexOf.TryGetValue(need, out int idx)) {
            return new[] { idx, i };
        }
        indexOf[nums[i]] = i;
    }
    return new[] { -1, -1 };
}

// ===========================================================
// Helpers
// ===========================================================

static void PrintMap(string label, Dictionary<string, int> map, params string[] keys) {
    Console.WriteLine(label);
    foreach (var k in keys) {
        if (map.TryGetValue(k, out int v))
            Console.WriteLine($"  '{k}' -> {v}");
        else
            Console.WriteLine($"  '{k}' -> (not found)");
    }
}

static void PrintArray(string label, int[] arr) {
    Console.WriteLine(label + "[" + string.Join(", ", arr) + "]");
}

// ===========================================================
// TOP-LEVEL TESTS (no Main())
// ===========================================================

Console.WriteLine("=== Test: demoHash ===");
var freq0 = new Dictionary<string, int>();
freq0["apple"] = 1;
freq0["banana"] = 2;
int n0 = freq0.ContainsKey("apple") ? freq0["apple"] : 0;
Console.WriteLine($"apple count = {n0} (expected 1)\n");

// -----------------------------------------------------------

Console.WriteLine("=== Test: CountWords (word frequency) ===");

var words = new[] { "apple", "banana", "apple", "orange", "banana", "apple" };
var freq = CountWords(words);

PrintMap("Frequencies:", freq, "apple", "banana", "orange", "pear");
Console.WriteLine("Expected: apple=3, banana=2, orange=1, pear not found\n");

// -----------------------------------------------------------

Console.WriteLine("=== Test: FirstDuplicate ===");

int[] arr1 = { 2, 5, 1, 2, 3, 5, 1 };
PrintArray("Array 1: ", arr1);
var dup1 = FirstDuplicate(arr1);
Console.WriteLine($"First duplicate: {dup1} (expected 2)\n");

int[] arr2 = { 1, 2, 3, 4, 5 };
PrintArray("Array 2: ", arr2);
var dup2 = FirstDuplicate(arr2);
Console.WriteLine($"First duplicate: {dup2} (expected null)\n");

// -----------------------------------------------------------

Console.WriteLine("=== Test: TwoSum ===");

int[] nums = { 2, 7, 11, 15 };
int target = 9;
PrintArray("Array: ", nums);
Console.WriteLine("Target: " + target);

var pair1 = TwoSum(nums, target);
Console.WriteLine("twoSum result indices: [" + string.Join(", ", pair1) + "]");
Console.WriteLine("Expected: [0, 1] or [1, 0]\n");

// Negative test
Console.WriteLine("--- Negative case (no pair) ---");
int[] nums2 = { 1, 2, 3 };
int target2 = 100;
PrintArray("Array: ", nums2);
Console.WriteLine("Target: " + target2);

var pair2 = TwoSum(nums2, target2);
Console.WriteLine("twoSum result indices: [" + string.Join(", ", pair2) + "]");
Console.WriteLine("Expected: [-1, -1]\n");
