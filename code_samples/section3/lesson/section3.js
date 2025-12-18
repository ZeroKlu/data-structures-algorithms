// Definition for singly-linked list node.
class ListNode {
    constructor(value, next = null) {
        // Node value
        this.value = value;

        // Reference to next node
        this.next = next;
    }
}

// Add a node to the front of the list
function pushFront(head, value) {
    return new ListNode(value, head);
}

// Print the list
function printList(head) {
    let current = head;
    while (current !== null) {
        console.log(current.value);
        current = current.next;
    }
}

// Reverse the list
function reverseList(head) {
    let prev = null;
    let current = head;
    while (current !== null) {
        const nextNode = current.next;
        current.next = prev;
        prev = current;
        current = nextNode;
    }
    return prev;
}

// Print the list recursively
function printListRecursive(head) {
    if (head === null) {
        return;
    }
    console.log(head.value);
    printListRecursive(head.next);
}

let head = new ListNode(1, new ListNode(2, new ListNode(3)));
printList(head);
console.log();
head = reverseList(head);
printList(head);
console.log();
printListRecursive(head);
