package code_samples.section6.problems.problem6_3;

import java.util.*;

public class problem6_3 {

    List<Integer> kSmallest(int[] arr, int k) {
        // Handle edge cases up front
        if (k <= 0 || arr == null || arr.length == 0) {
            return new ArrayList<>();
        }

        PriorityQueue<Integer> pq =
            new PriorityQueue<>((a, b) -> b - a); // max-heap

        for (int x : arr) {
            if (pq.size() < k) {
                pq.add(x);
            } else if (x < pq.peek()) {
                pq.poll();
                pq.add(x);
            }
        }
        return new ArrayList<>(pq);
    }

    // ===== Test Harness =====
    private static void printArray(String label, int[] arr) {
        System.out.print(label);
        for (int x : arr) System.out.print(x + " ");
        System.out.println();
    }

    private static void printList(String label, List<Integer> list) {
        System.out.print(label);
        for (int x : list) System.out.print(x + " ");
        System.out.println();
    }

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
