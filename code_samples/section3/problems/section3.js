class ListNode {
    constructor(val, next = null) {
        this.val = val;
        this.next = next;
    }
}

// Insert at head: return new head
function insertHead(head, x) {
    const node = new ListNode(x, head);
    return node;
}

function insertAfter(p, x) {
    if (!p) return;
    const node = new ListNode(x, p.next);
    p.next = node;
}

function deleteAfter(p) {
    if (!p || !p.next) return;
    const doomed = p.next;
    p.next = doomed.next;
    // doomed is now unreachable and will be GCed
}

function reverseList(head) {
    let prev = null;
    let curr = head;

    while (curr !== null) {
        const nxt = curr.next;
        curr.next = prev;
        prev = curr;
        curr = nxt;
    }
    return prev;
}

function hasCycle(head) {
    if (!head) return false;
    let slow = head;
    let fast = head;
    while (fast !== null && fast.next !== null) {
        slow = slow.next;
        fast = fast.next.next;
        if (slow === fast) return true;
    }
    return false;
}

function middleNode(head) {
    let slow = head;
    let fast = head;

    while (fast !== null && fast.next !== null) {
        slow = slow.next;
        fast = fast.next.next;
    }
    return slow;
}

function mergeTwoLists(l1, l2) {
    const dummy = new ListNode(0);
    let tail = dummy;

    let a = l1;
    let b = l2;
    while (a !== null && b !== null) {
        if (a.val <= b.val) {
            tail.next = a;
            a = a.next;
        } else {
            tail.next = b;
            b = b.next;
        }
        tail = tail.next;
    }
    if (a !== null) tail.next = a;
    if (b !== null) tail.next = b;

    return dummy.next;
}

function removeNthFromEnd(head, n) {
    const dummy = new ListNode(0, head);
    let fast = dummy;
    let slow = dummy;

    for (let i = 0; i <= n; i++) {
        if (fast === null) return head; // n too large
        fast = fast.next;
    }

    while (fast !== null) {
        fast = fast.next;
        slow = slow.next;
    }

    const doomed = slow.next;
    if (doomed !== null) {
        slow.next = doomed.next;
    }
    return dummy.next;
}

function printList(head) {
    let current = head;
    while (current !== null) {
        console.log(current.val);
        current = current.next;
    }
}

// ----- Build list: 1 2 3 4 5 -----
let head = null;
head = insertHead(head, 5);
head = insertHead(head, 4);
head = insertHead(head, 3);
head = insertHead(head, 2);
head = insertHead(head, 1);

// ----- Insert 6 at the end -----
let tail = head;
while (tail.next !== null) {
    tail = tail.next;
}
insertAfter(tail, 6);

printList(head);   // 1 2 3 4 5 6

console.log();

// ----- Delete 6 -----
deleteAfter(tail);

printList(head);   // 1 2 3 4 5

console.log();

// ----- Reverse the list -----
head = reverseList(head);

printList(head);   // 5 4 3 2 1

console.log();

// ----- Check for cycle -----
console.log("Has cycle:", hasCycle(head) ? "true" : "false");

console.log();

// ----- Build separate cycle list: 1 2 3 4 -----
let cycle = null;
cycle = insertHead(cycle, 4);
cycle = insertHead(cycle, 3);
cycle = insertHead(cycle, 2);
cycle = insertHead(cycle, 1);

printList(cycle);  // 1 2 3 4

console.log();

// Create cycle: 4 -> 3
let p = cycle.next.next;        // node 3
let q = cycle.next.next.next;   // node 4
q.next = p;

console.log("Has cycle:", hasCycle(cycle) ? "true" : "false");

console.log();

// ----- Reverse head again -----
head = reverseList(head);   // 5 4 3 2 1 -> 1 2 3 4 5

printList(head);            // 1 2 3 4 5

console.log();

// ----- Middle node -----
const mid = middleNode(head);
if (mid !== null) {
    console.log("Middle node value:", mid.val);
}

// ----- Build second list: 6 7 8 -----
let more = null;
more = insertHead(more, 8);
more = insertHead(more, 7);
more = insertHead(more, 6);

printList(more);   // 6 7 8

console.log();

// ----- Merge lists -----
let merged = mergeTwoLists(head, more);

printList(merged); // 1 2 3 4 5 6 7 8

console.log();

// ----- Remove 7 (2nd from end) -----
merged = removeNthFromEnd(merged, 2);

printList(merged); // 1 2 3 4 5 6 8
