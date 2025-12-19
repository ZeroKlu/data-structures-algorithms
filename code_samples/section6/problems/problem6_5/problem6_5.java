package code_samples.section6.problems.problem6_5;

import java.util.*;

/*
    Problem 6.5 - Kth Largest Element in a Stream (Java)

    This program implements a KthLargest data structure that continuously
    tracks the k-th largest element in a stream of integers.

    Core idea:
      - Maintain a MIN-HEAP of size at most k.
      - The heap stores the k largest values seen so far.
      - The smallest value in that heap (the root) is the k-th largest overall.

    Why a min-heap?
      - We want fast access to the "cutoff" value (the smallest among the top k).
      - If a new value is larger than that cutoff, it belongs in the top k.
      - Otherwise, it can be ignored.

    Complexity:
      - Each add(val) operation runs in O(log k) time.
      - Space usage is O(k) for the heap.
*/

/*
    KthLargest
    ----------
    Maintains the k-th largest element in a dynamic stream of integers.
*/
class KthLargest {

    // Desired rank (k-th largest)
    private final int k;

    /*
        PriorityQueue<Integer> pq

        Java's PriorityQueue is a MIN-HEAP by default:
          - pq.peek() returns the smallest element.

        We store the k largest elements in this heap.
        Therefore, pq.peek() is the k-th largest element overall.
    */
    private final PriorityQueue<Integer> pq;

    /*
        Constructor
        -----------
        Initializes the KthLargest object with:
          - k: which largest element to track
          - nums: initial values in the stream

        Strategy:
          - Reuse the add(x) method so that initialization and streaming updates
            follow exactly the same logic.
    */
    public KthLargest(int k, int[] nums) {
        this.k = k;
        this.pq = new PriorityQueue<>(); // min-heap

        // Insert initial numbers using the same logic as future additions
        for (int x : nums) {
            add(x);
        }
    }

    /*
        add
        ---
        Adds a new value to the stream and returns the current k-th largest value.

        Algorithm:
          1) If the heap has fewer than k elements:
                - Insert the value unconditionally.
          2) Otherwise:
                - Compare val with the smallest element in the heap (pq.peek()).
                - If val is larger:
                      * Remove the smallest element
                      * Insert val
                - Else:
                      * Ignore val (it is not among the top k).
          3) Return pq.peek(), which represents the k-th largest element.

        Time complexity:
          - O(log k) per call

        Assumption:
          - k > 0. (If k == 0, the concept of "k-th largest" is undefined.)
    */
    public int add(int val) {
        if (pq.size() < k) {
            // Haven't collected k elements yet; just add the value
            pq.add(val);
        } else if (val > pq.peek()) {
            // val belongs among the top k
            pq.poll();      // remove the smallest of the top k
            pq.add(val);    // insert the new value
        }

        // The smallest value in the top-k heap is the k-th largest overall
        return pq.peek();
    }
}

/*
    problem6_5
    ----------
    Test harness for the KthLargest class.
*/
public class problem6_5 {

    /*
        printArray
        ----------
        Helper method to print an integer array with a label.
    */
    private static void printArray(String label, int[] arr) {
        System.out.print(label);
        for (int x : arr) System.out.print(x + " ");
        System.out.println();
    }

    public static void main(String[] args) {
        System.out.println("=== Test KthLargest ===");

        // Initial stream values
        int[] nums = {4, 5, 8, 2};
        int k = 3;

        printArray("Initial nums: ", nums);
        System.out.println("k = " + k);
        System.out.println();

        // Create the KthLargest object
        KthLargest kth = new KthLargest(k, nums);

        // Values to add incrementally
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
            The smallest of those elements is the k-th largest overall.
        */
    }
}
