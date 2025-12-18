package code_samples.section3.problems;

public class section3 {
    // Insert at head: return new head
    public static ListNode insertHead(ListNode head, int x) {
        ListNode node = new ListNode(x);
        node.next = head;
        return node;
    }

    // Insert after node p (if non-null)
    public static void insertAfter(ListNode p, int x) {
        if (p == null) return;
        ListNode node = new ListNode(x);
        node.next = p.next;
        p.next = node;
    }

    // Delete node immediately after p
    public static void deleteAfter(ListNode p) {
        if (p == null || p.next == null) return;
        ListNode doomed = p.next;
        p.next = doomed.next;
        // In Java, garbage collector reclaims doomed
    }

    // Reverse a linked list
    public static ListNode reverseList(ListNode head) {
        ListNode prev = null;
        ListNode curr = head;

        while (curr != null) {
            ListNode nxt = curr.next;
            curr.next = prev;
            prev = curr;
            curr = nxt;
        }
        return prev;
    }

    // Check if a linked list has a cycle
    public static boolean hasCycle(ListNode head) {
        if (head == null) return false;
        ListNode slow = head;
        ListNode fast = head;
        while (fast != null && fast.next != null) {
            slow = slow.next;
            fast = fast.next.next;
            if (slow == fast) return true;
        }
        return false;
    }

    // Find the middle node
    public static ListNode middleNode(ListNode head) {
        ListNode slow = head;
        ListNode fast = head;

        while (fast != null && fast.next != null) {
            slow = slow.next;
            fast = fast.next.next;
        }
        return slow;
    }

    // Merge two sorted linked lists
    public static ListNode mergeTwoLists(ListNode l1, ListNode l2) {
        ListNode dummy = new ListNode(0);
        ListNode tail = dummy;

        ListNode a = l1;
        ListNode b = l2;
        while (a != null && b != null) {
            if (a.val <= b.val) {
                tail.next = a;
                a = a.next;
            } else {
                tail.next = b;
                b = b.next;
            }
            tail = tail.next;
        }
        if (a != null) tail.next = a;
        if (b != null) tail.next = b;

        return dummy.next;
    }

    // Remove nth node from end
    public static ListNode removeNthFromEnd(ListNode head, int n) {
        ListNode dummy = new ListNode(0);
        dummy.next = head;
        ListNode fast = dummy;
        ListNode slow = dummy;

        for (int i = 0; i <= n; i++) {
            if (fast == null) return head; // n too large
            fast = fast.next;
        }

        while (fast != null) {
            fast = fast.next;
            slow = slow.next;
        }

        ListNode doomed = slow.next;
        slow.next = doomed.next;
        // doomed node is eventually GCed

        return dummy.next;
    }

    // Print a linked list
    public static void printList(ListNode head) {
        ListNode current = head;
        System.out.println();
        while (current != null) {
            System.out.print(current.val);
            current = current.next;
        }
    }

    public static void main(String[] args) {

        // ----- Build list: 1 2 3 4 5 -----
        ListNode head = null;
        head = insertHead(head, 5);
        head = insertHead(head, 4);
        head = insertHead(head, 3);
        head = insertHead(head, 2);
        head = insertHead(head, 1);

        // ----- Insert 6 at the end -----
        ListNode tail = head;
        while (tail.next != null) {
            tail = tail.next;
        }
        insertAfter(tail, 6);

        printList(head);  // 123456

        // ----- Delete 6 -----
        deleteAfter(tail);

        printList(head);  // 12345

        // ----- Reverse the list -----
        head = reverseList(head);

        printList(head);  // 54321

        // ----- Check for cycle -----
        System.out.println("\nHas cycle: " + (hasCycle(head) ? "true" : "false"));

        // ----- Build separate cycle list: 1 2 3 4 -----
        ListNode cycle = null;
        cycle = insertHead(cycle, 4);
        cycle = insertHead(cycle, 3);
        cycle = insertHead(cycle, 2);
        cycle = insertHead(cycle, 1);

        printList(cycle);  // 1234

        // Create cycle: 4 -> 3
        ListNode p = cycle.next.next;        // node 3
        ListNode q = cycle.next.next.next;   // node 4
        q.next = p;

        System.out.println("\nHas cycle: " + (hasCycle(cycle) ? "true" : "false"));

        // ----- Reverse head again -----
        head = reverseList(head);  // 54321 -> 12345

        printList(head);           // 12345

        // ----- Middle node -----
        ListNode mid = middleNode(head);
        if (mid != null) {
            System.out.println("\nMiddle node value: " + mid.val);
        }

        // ----- Build second list: 6 7 8 -----
        ListNode more = null;
        more = insertHead(more, 8);
        more = insertHead(more, 7);
        more = insertHead(more, 6);

        printList(more);           // 678

        // ----- Merge lists -----
        ListNode merged = mergeTwoLists(head, more);

        printList(merged);         // 12345678

        // ----- Remove 7 (2nd from end) -----
        merged = removeNthFromEnd(merged, 2);

        printList(merged);         // 1234568
    }
}
