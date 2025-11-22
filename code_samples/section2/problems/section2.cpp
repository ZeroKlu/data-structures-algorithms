#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

void reverseArray(vector<int>& arr) {
    int i = 0;
    int j = (int)arr.size() - 1;
    while (i < j) {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
        i++;
        j--;
    }
}

bool twoSumBruteforce(const vector<int>& nums, int target) {
    int n = (int)nums.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (nums[i] + nums[j] == target) return true;
        }
    }
    return false;
}

// Hash-based O(n) expected
bool twoSumHash(const vector<int>& nums, int target) {
    unordered_set<int> seen;
    for (int x : nums) {
        int need = target - x;
        if (seen.count(need)) return true;
        seen.insert(x);
    }
    return false;
}

void moveZeroes(vector<int>& nums) {
    int write = 0;
    for (int read = 0; read < (int)nums.size(); read++) {
        if (nums[read] != 0) {
            nums[write] = nums[read];
            write++;
        }
    }
    while (write < (int)nums.size()) {
        nums[write] = 0;
        write++;
    }
}

bool isAnagram(const string& s, const string& t) {
    if (s.size() != t.size()) return false;
    int freq[26] = {0};
    for (size_t i = 0; i < s.size(); i++) {
        freq[s[i] - 'a']++;
        freq[t[i] - 'a']--;
    }
    for (int i = 0; i < 26; i++) {
        if (freq[i] != 0) return false;
    }
    return true;
}

int lengthOfLongestSubstring(const string& s) {
    vector<int> last_pos(128, -1);
    int best = 0;
    int left = 0;
    for (int right = 0; right < (int)s.size(); right++) {
        unsigned char ch = (unsigned char)s[right];
        if (last_pos[ch] >= left) {
            left = last_pos[ch] + 1;
        }
        last_pos[ch] = right;
        int window_len = right - left + 1;
        if (window_len > best) best = window_len;
    }
    return best;
}

static void reverseRange(vector<int>& nums, int l, int r) {
    while (l < r) {
        int tmp = nums[l];
        nums[l] = nums[r];
        nums[r] = tmp;
        l++;
        r--;
    }
}

void rotate(vector<int>& nums, int k) {
    int n = (int)nums.size();
    if (n == 0) return;
    k %= n;
    if (k == 0) return;

    reverseRange(nums, 0, n - 1);
    reverseRange(nums, 0, k - 1);
    reverseRange(nums, k, n - 1);
}

int main() {
    vector<int> arr = {1, 2, 3, 4, 5};

    reverseArray(arr);
    for (int x : arr) {
        cout << x << " ";
    }
    cout << endl;
    reverseArray(arr);

    cout << "Has pair with sum 9 (brute force): " << (twoSumBruteforce(arr, 9) ? "true" : "false") << endl;
    cout << "Has pair with sum 9 (hash-based): " << (twoSumHash(arr, 9) ? "true" : "false") << endl;

    vector<int> nums = {0, 1, 0, 3, 12};
    moveZeroes(nums);
    for (int i = 0; i < 5; i++) {
        cout << nums[i] << " ";
    }
    cout << endl;

    string s1 = "listen";
    string s2 = "silent";
    cout << "Is \"" << s1 << "\" an anagram of \"" << s2 << "\": " << (isAnagram(s1.c_str(), s2.c_str()) ? "true" : "false") << endl;

    string s3 = "abcabcbb";
    cout << "Length of longest substring in \"" << s3 << "\": " << lengthOfLongestSubstring(s3.c_str()) << endl;

    vector<int> nums2 = {1, 2, 3, 4, 5};
    rotate(nums2, 2);
    cout << "Array rotated 2 positions: ";
    for (int i = 0; i < 5; i++) {
        cout << nums2[i] << " ";
    }
    cout << endl;
    
    return 0;
}
