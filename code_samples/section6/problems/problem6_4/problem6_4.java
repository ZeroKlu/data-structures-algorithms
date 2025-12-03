package code_samples.section6.problems.problem6_4;

import java.util.*;

class ListNode {
    int val;
    ListNode next;
    ListNode(int v) { val = v; }
}

public class problem6_4 {

    // Your mergeKLists implementation
    ListNode mergeKLists(ListNode[] lists) {
        PriorityQueue<ListNode> pq =
            new PriorityQueue<>(Comparator.comparingInt(a -> a.val));

        for (ListNode node : lists) {
            if (node != null) pq.add(node);
        }

        ListNode dummy = new ListNode(0);
        ListNode cur = dummy;

        while (!pq.isEmpty()) {
            ListNode node = pq.poll();
            cur.next = node;
            cur = cur.next;
            if (node.next != null) {
                pq.add(node.next);
            }
        }
        return dummy.next;
    }

    // ===== Helpers for testing =====

    private static ListNode buildList(int... vals) {
        ListNode head = null, tail = null;
        for (int v : vals) {
            ListNode node = new ListNode(v);
            if (head == null) {
                head = tail = node;
            } else {
                tail.next = node;
                tail = node;
            }
        }
        return head;
    }

    private static void printList(String label, ListNode head) {
        System.out.print(label);
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
        ListNode merged5 = solver.mergeKLists(emptyArr);
        printList("Merged: ", merged5);
        System.out.println("(Expected: <empty>)");
    }
}
