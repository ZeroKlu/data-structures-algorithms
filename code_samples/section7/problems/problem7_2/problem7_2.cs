/**
 * GroupAnagrams
 *
 * Purpose:
 *   Groups strings that are anagrams of each other.
 *
 * Definition:
 *   Two strings are anagrams if they contain the same characters with the same
 *   multiplicities (frequencies), possibly in a different order.
 *
 * Parameters:
 *   strs - Array of input strings to group.
 *
 * Returns:
 *   A list of groups (IList<IList<string>>), where each inner list contains
 *   strings that are anagrams of one another.
 *
 * Approach:
 *   - Use a Dictionary<string, List<string>> where:
 *       key   = canonical representation of the word
 *       value = list of original words sharing that key
 *
 *   - Canonical key chosen here:
 *       Sort the characters in the string and use the sorted string as the key.
 *       Example: "eat" -> ['a','e','t'] -> "aet"
 *                "tea" -> "aet"  (same key => same group)
 *
 * Complexity:
 *   Let:
 *     n = number of strings
 *     k = average string length
 *
 *   - Time:  O(n * k log k)
 *       Each string is sorted, costing O(k log k).
 *   - Space: O(n * k)
 *       Keys and grouped strings are stored in memory.
 *
 * Notes:
 *   - Output order is not guaranteed:
 *       Dictionary iteration order should not be relied upon for stable output
 *       across all runtimes/versions. The test output correctly notes that
 *       group ordering may vary.
 *   - For high performance, you could build a frequency-based key (O(k))
 *       instead of sorting (O(k log k)).
 */
static IList<IList<string>> GroupAnagrams(string[] strs) {
    // Map: sorted-word key -> list of original words that match that key
    var map = new Dictionary<string, List<string>>();

    // Process each string and insert it into its anagram bucket
    foreach (var s in strs) {
        // Convert to char[] so we can sort in place
        var chars = s.ToCharArray();

        // Sorting creates the canonical key for anagrams
        Array.Sort(chars);

        // Convert sorted chars back into a string key (e.g., "aet")
        var key = new string(chars);

        // Get the existing group list or create one if this is the first time
        // we've seen this key.
        if (!map.TryGetValue(key, out var list)) {
            list = new List<string>();
            map[key] = list;
        }

        // Add the original (unsorted) string to the group
        list.Add(s);
    }

    // Return the groups as IList<IList<string>>.
    // - map.Values is a collection of List<string>
    // - Cast<IList<string>>() widens each List<string> to IList<string>
    // - ToList() materializes it as a List<IList<string>>
    //
    // Note: This requires `using System.Linq;`
    return map.Values.Cast<IList<string>>().ToList();
}

// ==============================
// Top-level tests
// ==============================
//
// This uses top-level statements (C# 9+), so these lines execute immediately
// without requiring an explicit Main() method.
//

// Input data (collection expression syntax; C# 12+)
string[] input = ["eat", "tea", "tan", "ate", "nat", "bat"];

Console.WriteLine("=== Test: GroupAnagrams ===\n");
Console.WriteLine("Input: [" + string.Join(", ", input) + "]\n");

// Run the function under test
var groups = GroupAnagrams(input);

// Print the groups (order may vary)
Console.WriteLine("Anagram Groups:");
foreach (var group in groups) {
    Console.WriteLine("  [" + string.Join(", ", group) + "]");
}

// Expected output guidance
Console.WriteLine("\nExpected groups (order may vary):");
Console.WriteLine("  [eat, tea, ate]");
Console.WriteLine("  [tan, nat]");
Console.WriteLine("  [bat]");
