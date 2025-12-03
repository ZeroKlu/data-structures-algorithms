package code_samples.section6.problems.problem6_5;

import java.util.*;

class KthLargest {
    private final int k;
    private final PriorityQueue<Integer> pq; // min-heap

    public KthLargest(int k, int[] nums) {
        this.k = k;
        this.pq = new PriorityQueue<>();
        for (int x : nums) {
            add(x);
        }
    }

    public int add(int val) {
        if (pq.size() < k) {
            pq.add(val);
        } else if (val > pq.peek()) {
            pq.poll();
            pq.add(val);
        }
        return pq.peek();
    }
}

public class problem6_5 {

    // Helper to print arrays
    private static void printArray(String label, int[] arr) {
        System.out.print(label);
        for (int x : arr) System.out.print(x + " ");
        System.out.println();
    }

    public static void main(String[] args) {
        System.out.println("=== Test KthLargest ===");

        int[] nums = {4, 5, 8, 2};
        int k = 3;

        printArray("Initial nums: ", nums);
        System.out.println("k = " + k);
        System.out.println();

        KthLargest kth = new KthLargest(k, nums);

        int[] toAdd = {3, 5, 10, 9, 4};

        System.out.println("Adding values:");
        for (int val : toAdd) {
            int kthVal = kth.add(val);
            System.out.println("add(" + val + ") -> kth largest = " + kthVal);
        }

        /*
          Expected sequence:
            add(3)  -> 4
            add(5)  -> 5
            add(10) -> 5
            add(9)  -> 8
            add(4)  -> 8

          Explanation:
            The min-heap always stores the current k largest elements.
        */
    }
}
