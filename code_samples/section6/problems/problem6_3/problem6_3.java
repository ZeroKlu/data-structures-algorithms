package code_samples.section6.problems.problem6_3;

import java.util.*;

/*
    Problem 6.3 - Find the k smallest elements using a max-heap (Java).

    Core idea:
      - Maintain a MAX-HEAP of size at most k.
      - The heap always contains the k smallest elements seen so far.
      - The root of the max-heap is the *largest* among those k smallest.
      - When a new element x arrives:
          * If the heap has fewer than k elements → insert x.
          * Else if x < heap.peek() → remove the largest and insert x.
          * Else → ignore x.

    Why a max-heap?
      - We want fast access to the largest of the kept k elements.
      - That element defines the cutoff for whether a new value belongs
        among the k smallest.

    Complexity:
      - Time:  O(n log k), where n = arr.length
      - Space: O(k)

    Edge cases handled:
      - k <= 0
      - null or empty input array
*/

public class problem6_3 {

    /*
        kSmallest
        ---------
        Returns a list containing the k smallest elements of the input array.

        Parameters:
            arr : input array of integers
            k   : number of smallest elements to return

        Returns:
            - A list containing up to k smallest elements (unordered).
            - Returns an empty list if k <= 0, arr is null, or arr is empty.

        Notes:
            - The returned list is NOT sorted.
            - Because a max-heap is used, iteration order typically reflects
              heap order (not ascending).
    */
    List<Integer> kSmallest(int[] arr, int k) {

        // Handle edge cases up front to keep main logic simple
        if (k <= 0 || arr == null || arr.length == 0) {
            return new ArrayList<>();
        }

        /*
            Java's PriorityQueue is a MIN-HEAP by default.
            To turn it into a MAX-HEAP, we reverse the comparator.

            Comparator explanation:
                (a, b) -> b - a

            This causes larger values to be considered "smaller" in priority,
            making peek() return the maximum element.
        */
        PriorityQueue<Integer> pq =
            new PriorityQueue<>((a, b) -> b - a); // max-heap

        /*
            Iterate through all elements in the array and maintain a heap
            of at most k elements.
        */
        for (int x : arr) {

            if (pq.size() < k) {
                // Heap not full yet: just add the element.
                pq.add(x);

            } else if (x < pq.peek()) {
                /*
                    Heap already contains k elements.

                    pq.peek() is the LARGEST of the k smallest elements so far.
                    If x is smaller than that, x belongs in the result set.
                */
                pq.poll();   // remove current largest
                pq.add(x);   // insert the smaller element
            }

            // Else:
            //   x >= pq.peek(), so x cannot be among the k smallest elements.
            //   Ignore it.
        }

        /*
            Convert heap contents to a list.

            Important:
              - Order is arbitrary (heap order), NOT sorted.
              - Sorting can be done by the caller if required.
        */
        return new ArrayList<>(pq);
    }

    // ===== Test Harness =====

    /*
        printArray
        ----------
        Utility function to print an int[] with a label.
    */
    private static void printArray(String label, int[] arr) {
        System.out.print(label);
        for (int x : arr) System.out.print(x + " ");
        System.out.println();
    }

    /*
        printList
        ---------
        Utility function to print a List<Integer> with a label.
    */
    private static void printList(String label, List<Integer> list) {
        System.out.print(label);
        for (int x : list) System.out.print(x + " ");
        System.out.println();
    }

    /*
        main
        ----
        Test driver that exercises kSmallest with:
          1) k <= n
          2) k > n
          3) k = 0

        For verification:
          - Results are also sorted before printing expected output,
            since kSmallest does not guarantee order.
    */
    public static void main(String[] args) {
        problem6_3 solver = new problem6_3();

        int[] arr = {7, 2, 9, 4, 1, 5, 8, 3, 6};

        // ---- Test 1: k <= n ----
        System.out.println("=== Test 1: k <= n ===");
        printArray("Input array: ", arr);

        int k = 4;
        System.out.println("k = " + k);

        List<Integer> result1 = solver.kSmallest(arr, k);
        printList("kSmallest result (heap order): ", result1);

        // Sort result for easy correctness checking
        List<Integer> sorted1 = new ArrayList<>(result1);
        Collections.sort(sorted1);
        printList("kSmallest result (sorted): ", sorted1);
        System.out.println("(Expected: 1 2 3 4)\n");

        // ---- Test 2: k > n ----
        System.out.println("=== Test 2: k > n ===");
        k = 20;

        printArray("Input array: ", arr);
        System.out.println("k = " + k);

        List<Integer> result2 = solver.kSmallest(arr, k);
        printList("kSmallest result (heap order): ", result2);

        List<Integer> sorted2 = new ArrayList<>(result2);
        Collections.sort(sorted2);
        printList("kSmallest result (sorted): ", sorted2);
        System.out.println("(Expected: all elements sorted)\n");

        // ---- Test 3: k = 0 ----
        System.out.println("=== Test 3: k = 0 ===");
        k = 0;

        System.out.println("k = " + k);
        List<Integer> result3 = solver.kSmallest(arr, k);
        printList("kSmallest result: ", result3);
        System.out.println("(Expected: empty list)\n");
    }
}
