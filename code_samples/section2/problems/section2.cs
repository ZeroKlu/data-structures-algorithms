static void ReverseArray(int[] arr) {
    int i = 0;
    int j = arr.Length - 1;
    while (i < j) {
        // Old way requires a temp variable
        // int tmp = arr[i];
        // arr[i] = arr[j];
        // arr[j] = tmp;

        // In C# 10 and later, we can swap using a tuple
        (arr[j], arr[i]) = (arr[i], arr[j]);

        i++;
        j--;
    }
}

// Brute force O(n^2)
static bool TwoSumBruteforce(int[] nums, int target) {
    int n = nums.Length;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (nums[i] + nums[j] == target) {
                return true;
            }
        }
    }
    return false;
}

// Hash-based O(n) expected
static bool TwoSumHash(int[] nums, int target) {
    var seen = new HashSet<int>();
    foreach (int x in nums) {
        int need = target - x;
        if (seen.Contains(need)) {
            return true;
        }
        seen.Add(x);
    }
    return false;
}

static void MoveZeroes(int[] nums) {
    int write = 0;
    for (int read = 0; read < nums.Length; read++) {
        if (nums[read] != 0) {
            nums[write] = nums[read];
            write++;
        }
    }
    while (write < nums.Length) {
        nums[write] = 0;
        write++;
    }
}

static bool IsAnagram(string s, string t) {
    if (s.Length != t.Length) return false;
    var freq = new int[26];
    for (int i = 0; i < s.Length; i++) {
        freq[s[i] - 'a']++;
        freq[t[i] - 'a']--;
    }
    for (int i = 0; i < 26; i++) {
        if (freq[i] != 0) return false;
    }
    return true;
}

static int LengthOfLongestSubstring(string s) {
    var lastPos = new Dictionary<char, int>();
    int best = 0;
    int left = 0;

    for (int right = 0; right < s.Length; right++) {
        char ch = s[right];
        if (lastPos.TryGetValue(ch, out int prev) && prev >= left) {
            left = prev + 1;
        }
        lastPos[ch] = right;
        int windowLen = right - left + 1;
        if (windowLen > best) best = windowLen;
    }
    return best;
}

static void Rotate(int[] nums, int k) {
    int n = nums.Length;
    if (n == 0) return;
    k %= n;
    if (k == 0) return;

    ReverseRange(nums, 0, n - 1);
    ReverseRange(nums, 0, k - 1);
    ReverseRange(nums, k, n - 1);
}

static void ReverseRange(int[] nums, int l, int r) {
    while (l < r) {
        // Old way requires a temp variable
        // int tmp = nums[l];
        // nums[l] = nums[r];
        // nums[r] = tmp;

        // In C# 10 and later, we can swap using a tuple
        (nums[r], nums[l]) = (nums[l], nums[r]);

        l++;
        r--;
    }
}

int[] arr = [1, 2, 3, 4, 5];

ReverseArray(arr);
Console.WriteLine($"Reversed array: {string.Join(",", arr)}");
ReverseArray(arr);

Console.WriteLine($"TwoSumBruteforce: {TwoSumBruteforce(arr, 9)}");
Console.WriteLine($"TwoSumHash: {TwoSumHash(arr, 9)}");

int[] nums = [0, 1, 0, 3, 12];
MoveZeroes(nums);
Console.WriteLine(string.Join(",", nums));

string s = "listen";
string t = "silent";
Console.WriteLine($"Is \"{s}\" an anagram of \"{t}\": {IsAnagram(s, t)}");

string s2 = "abcabcbb";
Console.WriteLine($"Length of longest substring: {LengthOfLongestSubstring(s2)}");

Rotate(arr, 2);
Console.WriteLine($"Array rotated 2 positions: {string.Join(",", arr)}");
