class ListNode {
    constructor(value, next = null) {
        this.value = value;
        this.next = next;
    }
}

function pushFront(head, value) {
    return new ListNode(value, head);
}

function printList(head) {
    let current = head;
    while (current !== null) {
        console.log(current.value);
        current = current.next;
    }
}

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
