package code_samples.section6.problems.problem6_4;

import java.util.*;

/*
    Problem 6.4 - Merge K Sorted Linked Lists (Java)

    This file demonstrates merging multiple sorted singly linked lists into a single
    sorted list using a PriorityQueue (min-heap).

    High-level algorithm (k-way merge):
      1) Put the head of each non-empty list into a MIN-HEAP ordered by node.val.
      2) While heap not empty:
           - poll() the smallest node
           - append it to the merged list
           - if that node has a next node, add node.next to the heap
      3) The merged list is built by relinking the original nodes.

    Why a min-heap?
      - At any moment, the next smallest element must be one of the current heads
        from the remaining lists.
      - A min-heap lets us pick that smallest head in O(log k).

    Complexity:
      - Let N be total number of nodes across all lists.
      - Heap size is at most k.
      - Each node is inserted and removed from the heap once:
            Time:  O(N log k)
            Space: O(k)  (heap only; output list reuses nodes)

    Important memory note:
      - mergeKLists does NOT allocate new nodes for the merged result.
        It reuses and relinks the existing nodes created by buildList().
*/

class ListNode {
    int val;
    ListNode next;

    // Constructor initializes the node's value; next defaults to null.
    ListNode(int v) { val = v; }
}

public class problem6_4 {

    /*
        mergeKLists
        -----------
        Merges an array of sorted linked lists into one sorted linked list.

        Parameters:
          - lists: array of head pointers (some may be null)
                   Each list is assumed sorted in ascending order.

        Returns:
          - head of the merged sorted list
          - null if all lists are empty OR if lists.length == 0

        Implementation details:
          - Uses a PriorityQueue as a MIN-HEAP.
          - Comparator orders nodes by node.val (smallest value first).
          - Uses a dummy node to simplify tail-append logic.
    */
    ListNode mergeKLists(ListNode[] lists) {

        /*
            PriorityQueue in Java is a MIN-HEAP by default.
            Comparator.comparingInt(a -> a.val) orders nodes by their value,
            so pq.poll() returns the node with the smallest val.
        */
        PriorityQueue<ListNode> pq =
            new PriorityQueue<>(Comparator.comparingInt(a -> a.val));

        // Seed the heap with the head of each non-empty list.
        for (ListNode node : lists) {
            if (node != null) pq.add(node);
        }

        // Dummy head to avoid special-casing the first appended node.
        ListNode dummy = new ListNode(0);
        ListNode cur = dummy; // tail pointer for building the merged list

        /*
            Main merge loop:
              - poll smallest node
              - append to merged list
              - push its successor (if present) into the heap
        */
        while (!pq.isEmpty()) {
            ListNode node = pq.poll();

            // Append the smallest node to the output list.
            cur.next = node;
            cur = cur.next;

            // If the extracted node had a next node, it becomes a new candidate.
            if (node.next != null) {
                pq.add(node.next);
            }
        }

        /*
            Optional safety:
              Ensure the merged list ends at null.
              In normal acyclic input lists this is already true, but it's harmless.
        */
        cur.next = null;

        return dummy.next;
    }

    // ===== Helpers for testing =====

    /*
        buildList
        ---------
        Creates a linked list from the provided ints in the given order.

        Parameters:
          - vals: varargs of ints (e.g., buildList(1, 4, 7))

        Returns:
          - head of the new list (null if vals is empty)

        Note:
          - This allocates nodes with 'new'.
          - This sample does not include an explicit free step (Java GC handles it).
    */
    private static ListNode buildList(int... vals) {
        ListNode head = null, tail = null;

        for (int v : vals) {
            ListNode node = new ListNode(v);

            if (head == null) {
                // First node sets both head and tail.
                head = tail = node;
            } else {
                // Append and advance tail.
                tail.next = node;
                tail = node;
            }
        }

        return head;
    }

    /*
        printList
        ---------
        Prints a linked list as space-separated values with a label.

        If head is null, prints only the label and a newline.
    */
    private static void printList(String label, ListNode head) {
        System.out.print(label);

        // Safe: for-loop won't execute if head is null.
        for (ListNode p = head; p != null; p = p.next) {
            System.out.print(p.val + " ");
        }

        System.out.println();
    }

    // ===== Test Runner =====

    public static void main(String[] args) {
        problem6_4 solver = new problem6_4();

        // ---- Test 1: Three sorted lists ----
        System.out.println("=== Test 1: multiple sorted lists ===");
        ListNode l1 = buildList(1, 4, 7);
        ListNode l2 = buildList(2, 5, 8);
        ListNode l3 = buildList(3, 6, 9);

        printList("List 1: ", l1);
        printList("List 2: ", l2);
        printList("List 3: ", l3);

        ListNode merged1 = solver.mergeKLists(new ListNode[]{l1, l2, l3});
        printList("Merged: ", merged1);
        System.out.println("(Expected: 1 2 3 4 5 6 7 8 9)\n");

        // ---- Test 2: mix of empty + non-empty lists ----
        System.out.println("=== Test 2: some lists empty ===");
        ListNode a = buildList(1, 3, 5);
        ListNode b = null;
        ListNode c = buildList(2, 4, 6, 8);
        ListNode d = null;

        printList("List A: ", a);
        printList("List B: ", b);
        printList("List C: ", c);
        printList("List D: ", d);

        ListNode merged2 = solver.mergeKLists(new ListNode[]{a, b, c, d});
        printList("Merged: ", merged2);
        System.out.println("(Expected: 1 2 3 4 5 6 8)\n");

        // ---- Test 3: all lists empty ----
        System.out.println("=== Test 3: all empty ===");
        ListNode merged3 = solver.mergeKLists(new ListNode[]{null, null, null});
        printList("Merged: ", merged3);
        System.out.println("(Expected: <empty>)\n");

        // ---- Test 4: single list ----
        System.out.println("=== Test 4: single list ===");
        ListNode single = buildList(10, 20, 30);
        printList("Input:  ", single);

        ListNode merged4 = solver.mergeKLists(new ListNode[]{single});
        printList("Merged: ", merged4);
        System.out.println("(Expected: 10 20 30)\n");

        // ---- Test 5: zero lists ----
        System.out.println("=== Test 5: zero lists ===");
        ListNode[] emptyArr = {};

        /*
            With an empty input array:
              - The for-loop that seeds the PQ does nothing.
              - The PQ is empty, so we skip the merge loop.
              - dummy.next is null, so mergeKLists returns null.
        */
        ListNode merged5 = solver.mergeKLists(emptyArr);
        printList("Merged: ", merged5);
        System.out.println("(Expected: <empty>)");
    }
}
