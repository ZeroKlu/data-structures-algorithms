#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

// Reverse an array O(n)
void reverseArray(int *arr, size_t n) {
    size_t i = 0;
    size_t j = (n == 0) ? 0 : n - 1;

    while (i < j) {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
        i++;
        j--;
    }
}

// Check for pair of elements that sum to target - Brute force O(n^2)
bool twoSumBruteForce(const int *nums, int n, int target) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (nums[i] + nums[j] == target) {
                return true;
            }
        }
    }
    return false;
}

// Move zeroes to the end - O(n)
// Hash-based solution would require a hash table implementation.
// In interviews, you can describe using an unordered_set-like API:
// scan once, check if (target - x) is in the set, then insert x.
void moveZeroes(int *nums, size_t n) {
    size_t write = 0;

    for (size_t read = 0; read < n; read++) {
        if (nums[read] != 0) {
            nums[write] = nums[read];
            write++;
        }
    }
    while (write < n) {
        nums[write] = 0;
        write++;
    }
}

// Check if two strings are anagrams - O(n)
bool isAnagram(const char *s, const char *t) {
    if (strlen(s) != strlen(t)) return false;

    int freq[26] = {0};
    for (size_t i = 0; s[i] != '\0'; i++) {
        freq[s[i] - 'a']++;
        freq[t[i] - 'a']--;
    }
    for (int i = 0; i < 26; i++) {
        if (freq[i] != 0) return false;
    }
    return true;
}

// Find the length of the longest substring without repeating characters - O(n)
// Assume ASCII (0-127). Initialize last_pos to -1.
int lengthOfLongestSubstring(const char *s) {
    int last_pos[128];
    for (int i = 0; i < 128; i++) last_pos[i] = -1;

    int best = 0;
    int left = 0;
    for (int right = 0; s[right] != '\0'; right++) {
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

// Reverse a range of an array - O(n)
static void reverseRange(int *nums, size_t l, size_t r) {
    while (l < r) {
        int tmp = nums[l];
        nums[l] = nums[r];
        nums[r] = tmp;
        l++;
        r--;
    }
}

// Rotate an array - O(n)
void rotateArray(int *nums, size_t n, size_t k) {
    if (n == 0) return;
    k %= n;
    if (k == 0) return;

    reverseRange(nums, 0, n - 1);
    reverseRange(nums, 0, k - 1);
    reverseRange(nums, k, n - 1);
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int n = sizeof(arr) / sizeof(arr[0]);

    reverseArray(arr, n);
    printf("Reversed array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    reverseArray(arr, n);

    printf("Has pair with sum 9: %s\n", twoSumBruteForce(arr, n, 9) ? "true" : "false");

    int nums[] = {0, 1, 0, 3, 12};
    printf("Original array with zeroes: ");
    for (int i = 0; i < sizeof(nums) / sizeof(nums[0]); i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");
    moveZeroes(nums, sizeof(nums) / sizeof(nums[0]));
    printf("Moved zeroes: ");
    for (int i = 0; i < sizeof(nums) / sizeof(nums[0]); i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");

    char s1[] = "listen";
    char s2[] = "silent";
    printf("'%s' is an anagram of '%s': %s\n", s1, s2, isAnagram(s1, s2) ? "true" : "false");

    char s3[] = "abcabcabc";
    printf("Length of longest substring in '%s': %d\n", s3, lengthOfLongestSubstring(s3));

    int nums2[] = {1, 2, 3, 4, 5};
    rotateArray(nums2, sizeof(nums2) / sizeof(nums2[0]), 2);
    printf("Rotated array: ");
    for (int i = 0; i < sizeof(nums2) / sizeof(nums2[0]); i++) {
        printf("%d ", nums2[i]);
    }
    printf("\n");

    return 0;
}
