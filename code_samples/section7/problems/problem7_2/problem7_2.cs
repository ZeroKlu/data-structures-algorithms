static IList<IList<string> > GroupAnagrams(string[] strs) {
    var map = new Dictionary<string, List<string>>();
    foreach (var s in strs) {
        var chars = s.ToCharArray();
        Array.Sort(chars);
        var key = new string(chars);
        if (!map.TryGetValue(key, out var list)) {
            list = new List<string>();
            map[key] = list;
        }
        list.Add(s);
    }
    return map.Values.Cast<IList<string> >().ToList();
}

// ==============================
// Top-level tests
// ==============================

string[] input = ["eat", "tea", "tan", "ate", "nat", "bat"];

Console.WriteLine("=== Test: GroupAnagrams ===\n");
Console.WriteLine("Input: [" + string.Join(", ", input) + "]\n");

var groups = GroupAnagrams(input);

Console.WriteLine("Anagram Groups:");
foreach (var group in groups) {
    Console.WriteLine("  [" + string.Join(", ", group) + "]");
}

Console.WriteLine("\nExpected groups (order may vary):");
Console.WriteLine("  [eat, tea, ate]");
Console.WriteLine("  [tan, nat]");
Console.WriteLine("  [bat]");
