#include <cstdlib>
#include <iostream>

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v) : val(v), next(nullptr) {}
};

// Insert at head: modify head by reference
void insertHead(ListNode*& head, int x) {
    ListNode* node = new ListNode(x);
    node->next = head;
    head = node;
}

// Insert after p
void insertAfter(ListNode* p, int x) {
    if (!p) return;
    ListNode* node = new ListNode(x);
    node->next = p->next;
    p->next = node;
}

// Delete after p
void deleteAfter(ListNode* p) {
    if (!p || !p->next) return;
    ListNode* doomed = p->next;
    p->next = doomed->next;
    delete doomed;
}

ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;

    while (curr != nullptr) {
        ListNode* nxt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nxt;
    }
    return prev;
}

bool hasCycle(ListNode* head) {
    if (!head) return false;
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

ListNode* middleNode(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;

    ListNode* a = l1;
    ListNode* b = l2;
    while (a && b) {
        if (a->val <= b->val) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    if (a) tail->next = a;
    if (b) tail->next = b;

    return dummy.next;
}

ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(0);
    dummy.next = head;
    ListNode* fast = &dummy;
    ListNode* slow = &dummy;

    for (int i = 0; i <= n; i++) {
        if (!fast) return head; // n too large
        fast = fast->next;
    }

    while (fast) {
        fast = fast->next;
        slow = slow->next;
    }

    ListNode* doomed = slow->next;
    slow->next = doomed->next;
    delete doomed;

    return dummy.next;
}

// Print list
void printList(ListNode* head) {
    std::cout << "\n";
    for (ListNode* curr = head; curr != nullptr; curr = curr->next) {
        std::cout << curr->val << " ";
    }
}

int main() {
    // ----- Build initial list: 1 2 3 4 5 -----
    ListNode* head = nullptr;
    insertHead(head, 5);
    insertHead(head, 4);
    insertHead(head, 3);
    insertHead(head, 2);
    insertHead(head, 1);

    // ----- Insert 6 at the end -----
    ListNode* tail = head;
    while (tail->next != nullptr) {
        tail = tail->next;
    }
    insertAfter(tail, 6);

    printList(head);   // 1 2 3 4 5 6

    // ----- Delete the 6 we just added -----
    deleteAfter(tail);

    printList(head);   // 1 2 3 4 5

    // ----- Reverse the list -----
    head = reverseList(head);

    printList(head);   // 5 4 3 2 1

    // ----- Check for cycle in head -----
    std::cout << "\nHas cycle: " << (hasCycle(head) ? "true" : "false");

    // ----- Build a separate list and then create a cycle -----
    ListNode* cycle = nullptr;
    insertHead(cycle, 4);
    insertHead(cycle, 3);
    insertHead(cycle, 2);
    insertHead(cycle, 1);   // cycle list: 1 2 3 4

    printList(cycle);       // 1 2 3 4

    // Get pointers to the nodes we want
    ListNode* p = cycle->next->next;          // node with value 3
    ListNode* q = cycle->next->next->next;    // node with value 4

    // Create the cycle: 4 -> 3
    q->next = p;

    std::cout << "\nHas cycle: " << (hasCycle(cycle) ? "true" : "false");

    // ----- Reverse head again -----
    head = reverseList(head);   // from 5 4 3 2 1 back to 1 2 3 4 5

    printList(head);            // 1 2 3 4 5

    // ----- Find middle node of head -----
    ListNode* mid = middleNode(head);
    if (mid != nullptr) {
        std::cout << "\nMiddle node value: " << mid->val;
    }

    // ----- Build second list: 6 7 8 -----
    ListNode* more = nullptr;
    insertHead(more, 8);
    insertHead(more, 7);
    insertHead(more, 6);    // more: 6 7 8

    printList(more);        // 6 7 8

    // ----- Merge the two sorted lists -----
    ListNode* merged = mergeTwoLists(head, more);
    printList(merged);      // 1 2 3 4 5 6 7 8

    // ----- Remove 7 (2nd from end) -----
    merged = removeNthFromEnd(merged, 2);
    printList(merged);      // 1 2 3 4 5 6 8

    return 0;
}
