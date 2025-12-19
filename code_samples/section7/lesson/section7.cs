// ===========================================================
// Functions Under Test
// ===========================================================

/// <summary>
/// Counts how many times each word appears in the input sequence.
/// </summary>
/// <param name="words">
/// Sequence of words/tokens to count. (May be an array, list, LINQ query, etc.)
/// </param>
/// <returns>
/// A Dictionary mapping each distinct word to its frequency.
/// </returns>
/// <remarks>
/// Complexity:
///  - Time:  O(n) average, where n is the number of input words
///  - Space: O(k), where k is the number of distinct words
///
/// Notes:
///  - This implementation uses TryGetValue to avoid inserting missing keys until needed.
///  - Alternative simpler pattern in modern C#:
///      freq[w] = freq.GetValueOrDefault(w) + 1;   // (.NET 5+)
/// </remarks>
static Dictionary<string, int> CountWords(IEnumerable<string> words) {
    // Frequency table: word -> count
    var freq = new Dictionary<string, int>();

    // Iterate once through the sequence and update counts.
    foreach (var w in words) {
        // TryGetValue reads without adding a key.
        // If the key exists, we update it; otherwise we initialize it.
        if (freq.TryGetValue(w, out int value)) {
            // Pre-increment local copy (value) then store it back into the dictionary.
            // Example: value=2 -> ++value => 3, then freq[w]=3.
            freq[w] = ++value;
        } else {
            // First time we've seen this word.
            freq[w] = 1;
        }
    }

    return freq;
}

/// <summary>
/// Finds the first duplicate value encountered when scanning left-to-right.
/// </summary>
/// <param name="arr">Array of integers to scan.</param>
/// <returns>
/// The first duplicated integer, or null if all values are unique.
/// </returns>
/// <remarks>
/// Complexity:
///  - Time:  O(n) average
///  - Space: O(n) worst case (when all are unique)
///
/// Notes:
///  - HashSet.Add(x) returns false if x is already present.
///  - Returning int? avoids ambiguous sentinels (like 0 or -1).
/// </remarks>
static int? FirstDuplicate(int[] arr) {
    // Tracks values we've already seen as we scan the array.
    var seen = new HashSet<int>();

    foreach (var x in arr) {
        // Add returns false if x is already present => we found a duplicate.
        if (!seen.Add(x)) return x;
    }

    // No duplicates found.
    return null;
}

/// <summary>
/// Solves the "Two Sum" problem: find indices (i, j) such that nums[i] + nums[j] == target.
/// </summary>
/// <param name="nums">Input array of integers.</param>
/// <param name="target">Desired sum.</param>
/// <returns>
/// An int[] of length 2 containing the indices if found; otherwise [-1, -1].
/// </returns>
/// <remarks>
/// Approach:
///  - Maintain a dictionary value -> index for values seen so far.
///  - For each nums[i], compute need = target - nums[i].
///    If need was previously seen, we have a solution.
///
/// Complexity:
///  - Time:  O(n) average
///  - Space: O(n)
///
/// Notes / Edge cases:
///  - If the same value appears multiple times, indexOf[value] is overwritten with
///    the most recent index. This is fine for typical TwoSum requirements (any valid pair).
///  - Returns a C# collection expression [idx, i] (C# 12+) for the found case,
///    and new[] { -1, -1 } for the missing case.
/// </remarks>
static int[] TwoSum(int[] nums, int target) {
    // Map: number value -> index where that value was last seen.
    var indexOf = new Dictionary<int, int>(); // value -> index

    // Scan the array once.
    for (int i = 0; i < nums.Length; i++) {
        // Value needed to reach target with nums[i]
        int need = target - nums[i];

        // TryGetValue does lookup without inserting a new entry.
        if (indexOf.TryGetValue(need, out int idx)) {
            // Found a previous index 'idx' such that nums[idx] == need.
            // Return the two indices that form the target sum.
            return [idx, i];
        }

        // Record the current value's index for future matches.
        // If nums[i] repeats later, this overwrites with the newer index.
        indexOf[nums[i]] = i;
    }

    // No pair found that sums to target.
    return new[] { -1, -1 };
}

// ===========================================================
// Helpers
// ===========================================================

/// <summary>
/// Prints selected keys from a word-frequency dictionary in a readable format.
/// </summary>
/// <param name="label">Heading printed before the key/value lines.</param>
/// <param name="map">Dictionary to query.</param>
/// <param name="keys">Keys to display (some may not exist in the map).</param>
/// <remarks>
/// Why TryGetValue:
///  - Avoids inserting missing keys (unlike map[k] which would throw if missing).
/// </remarks>
static void PrintMap(string label, Dictionary<string, int> map, params string[] keys) {
    Console.WriteLine(label);

    foreach (var k in keys) {
        // Lookup without modifying the dictionary.
        if (map.TryGetValue(k, out int v))
            Console.WriteLine($"  '{k}' -> {v}");
        else
            Console.WriteLine($"  '{k}' -> (not found)");
    }
}

/// <summary>
/// Prints an integer array as: "Label[1, 2, 3]".
/// </summary>
/// <param name="label">Prefix before the formatted array.</param>
/// <param name="arr">Array to print.</param>
static void PrintArray(string label, int[] arr) {
    // string.Join formats the array elements separated by ", "
    Console.WriteLine(label + "[" + string.Join(", ", arr) + "]");
}

// ===========================================================
// TOP-LEVEL TESTS (no Main())
// ===========================================================
//
// This file uses top-level statements (C# 9+), so code here executes immediately
// when the program runs—no explicit Main() method is needed.
//

Console.WriteLine("=== Test: demoHash ===");

// Example dictionary initialization using collection initializer syntax.
// Here we pre-populate counts as if they were produced by counting logic.
var freq0 = new Dictionary<string, int>
{
    ["apple"] = 1,
    ["banana"] = 2
};

// Demonstrates safe read: TryGetValue returns false if key is missing.
// If missing, we use 0 as the fallback.
int n0 = freq0.TryGetValue("apple", out int value) ? value : 0;

Console.WriteLine($"apple count = {n0} (expected 1)\n");

// -----------------------------------------------------------

Console.WriteLine("=== Test: CountWords (word frequency) ===");

// Input words: apple x3, banana x2, orange x1
var words = new[] { "apple", "banana", "apple", "orange", "banana", "apple" };

// Run the function under test
var freq = CountWords(words);

// Print counts for a few keys, including one not present ("pear")
PrintMap("Frequencies:", freq, "apple", "banana", "orange", "pear");

Console.WriteLine("Expected: apple=3, banana=2, orange=1, pear not found\n");

// -----------------------------------------------------------

Console.WriteLine("=== Test: FirstDuplicate ===");

// Case 1: duplicates exist; the first repeated value encountered is 2
int[] arr1 = [2, 5, 1, 2, 3, 5, 1];
PrintArray("Array 1: ", arr1);

var dup1 = FirstDuplicate(arr1);
// dup1 is nullable; interpolating prints the value or blank if null.
Console.WriteLine($"First duplicate: {dup1} (expected 2)\n");

// Case 2: no duplicates; expect null
int[] arr2 = [1, 2, 3, 4, 5];
PrintArray("Array 2: ", arr2);

var dup2 = FirstDuplicate(arr2);
Console.WriteLine($"First duplicate: {dup2} (expected null)\n");

// -----------------------------------------------------------

Console.WriteLine("=== Test: TwoSum ===");

// Positive case: 2 + 7 = 9, expected indices (0,1) (order depends on implementation)
int[] nums = [2, 7, 11, 15];
int target = 9;

PrintArray("Array: ", nums);
Console.WriteLine("Target: " + target);

var pair1 = TwoSum(nums, target);
Console.WriteLine("twoSum result indices: [" + string.Join(", ", pair1) + "]");
Console.WriteLine("Expected: [0, 1] or [1, 0]\n");

// Negative test: no pair sums to 100 in {1,2,3}
Console.WriteLine("--- Negative case (no pair) ---");
int[] nums2 = [1, 2, 3];
int target2 = 100;

PrintArray("Array: ", nums2);
Console.WriteLine("Target: " + target2);

var pair2 = TwoSum(nums2, target2);
Console.WriteLine("twoSum result indices: [" + string.Join(", ", pair2) + "]");
Console.WriteLine("Expected: [-1, -1]\n");
