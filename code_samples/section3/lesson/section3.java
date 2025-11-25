package code_samples.section3.lesson;

public class section3 {
    public static ListNode pushFront(ListNode head, int value) {
        return new ListNode(value, head);
    }

    public static void printList(ListNode head) {
        ListNode current = head;
        System.out.println();
        while (current != null) {
            System.out.print(current.value);
            current = current.next;
        }
    }

    public static ListNode reverseList(ListNode head) {
        ListNode prev = null;
        ListNode current = head;
        while (current != null) {
            ListNode nextNode = current.next;
            current.next = prev;
            prev = current;
            current = nextNode;
        }
        return prev;
    }

    public static void printListRecursive(ListNode head) {
        if (head == null) {
            return;
        }
        System.out.print(head.value);
        printListRecursive(head.next);
    }

    public static void main(String[] args) {
        ListNode head = null;
        head = pushFront(head, 3);
        head = pushFront(head, 2);
        head = pushFront(head, 1);
        printList(head);
        head = reverseList(head);
        printList(head);
        System.out.println();
        printListRecursive(head);
    }
}
