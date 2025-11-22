package code_samples.section2.problems;

import java.util.HashSet;
import java.util.Set;

public class section2 {
    public static void reverseArray(int[] arr) {
        int i = 0;
        int j = arr.length - 1;
        while (i < j) {
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    }

    public static // Brute force O(n^2)
    boolean twoSumBruteforce(int[] nums, int target) {
        int n = nums.length;
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
    public static boolean twoSumHash(int[] nums, int target) {
        Set<Integer> seen = new HashSet<>();
        for (int x : nums) {
            int need = target - x;
            if (seen.contains(need)) {
                return true;
            }
            seen.add(x);
        }
        return false;
    }

    public static void moveZeroes(int[] nums) {
        int write = 0;
        for (int read = 0; read < nums.length; read++) {
            if (nums[read] != 0) {
                nums[write] = nums[read];
                write++;
            }
        }
        while (write < nums.length) {
            nums[write] = 0;
            write++;
        }
    }

    public static boolean isAnagram(String s, String t) {
        if (s.length() != t.length())
            return false;
        int[] freq = new int[26];
        for (int i = 0; i < s.length(); i++) {
            freq[s.charAt(i) - 'a']++;
            freq[t.charAt(i) - 'a']--;
        }
        for (int f : freq) {
            if (f != 0)
                return false;
        }
        return true;
    }

    public static int lengthOfLongestSubstring(String s) {
        int[] lastPos = new int[128];
        for (int i = 0; i < 128; i++)
            lastPos[i] = -1;

        int best = 0;
        int left = 0;
        for (int right = 0; right < s.length(); right++) {
            char ch = s.charAt(right);
            if (lastPos[ch] >= left) {
                left = lastPos[ch] + 1;
            }
            lastPos[ch] = right;
            int windowLen = right - left + 1;
            if (windowLen > best)
                best = windowLen;
        }
        return best;
    }

    public static void rotate(int[] nums, int k) {
        int n = nums.length;
        if (n == 0)
            return;
        k %= n;
        if (k == 0)
            return;

        reverseRange(nums, 0, n - 1);
        reverseRange(nums, 0, k - 1);
        reverseRange(nums, k, n - 1);
    }

    public static void reverseRange(int[] nums, int l, int r) {
        while (l < r) {
            int tmp = nums[l];
            nums[l] = nums[r];
            nums[r] = tmp;
            l++;
            r--;
        }
    }

    public static void main(String[] args) {
        int[] arr = { 1, 2, 3, 4, 5 };
        
        reverseArray(arr);
        System.out.print("Reversed array: ");
        for (int x : arr) {
            System.out.print(x + " ");
        }
        System.out.println();
        reverseArray(arr);
        
        System.out.println("Has pair with sum 9 (brute force): " + (twoSumBruteforce(arr, 9) ? "true" : "false"));
        System.out.println("Has pair with sum 9 (hash-based): " + (twoSumHash(arr, 9) ? "true" : "false"));
        
        int[] nums = {0, 1, 0, 3, 12};
        moveZeroes(nums);
        for (int i = 0; i < 5; i++) {
            System.out.print(nums[i] + " ");
        }
        System.out.println();
        
        String s1 = "listen";
        String s2 = "silent";
        System.out.println("Is \"" + s1 + "\" an anagram of \"" + s2 + "\": " + (isAnagram(s1, s2) ? "true" : "false"));
        
        String s3 = "abcabcbb";
        System.out.println("Length of longest substring in \"" + s3 + "\": " + lengthOfLongestSubstring(s3));
        
        int[] nums2 = {1, 2, 3, 4, 5};
        rotate(nums2, 2);
        System.out.print("Array rotated 2 positions: ");
        for (int x : nums2) {
            System.out.print(x + " ");
        }
    }
}
