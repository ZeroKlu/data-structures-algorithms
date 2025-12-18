#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Linked list node
typedef struct ListNode {
    // Node value
    int val;

    // Pointer to next node
    struct ListNode* next;
} ListNode;

// Prints a linked list
void printList(ListNode* head) {
    ListNode* current = head;
    printf("\n");
    while (current != NULL) {
        printf("%d", current->val);
        current = current->next;
    }
}

// Insert at head: head pointer is updated via pointer-to-pointer
void insertHead(ListNode** head, int x) {
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->val = x;
    node->next = *head;
    *head = node;
}

// Insert after node p (if p is non-NULL)
void insertAfter(ListNode* p, int x) {
    if (!p) return;
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->val = x;
    node->next = p->next;
    p->next = node;
}

// Delete node immediately after p (if it exists)
void deleteAfter(ListNode* p) {
    if (!p || !p->next) return;
    ListNode* doomed = p->next;
    p->next = doomed->next;
    free(doomed);
}

// Reverse a linked list
ListNode* reverseList(ListNode* head) {
    ListNode* prev = NULL;
    ListNode* curr = head;

    while (curr != NULL) {
        ListNode* nxt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nxt;
    }
    return prev;
}

// Check if a linked list has a cycle
bool hasCycle(ListNode* head) {
    if (head == NULL) return false;
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

// Find the middle of a linked list
ListNode* middleNode(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// Merge two sorted linked lists
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy;
    dummy.next = NULL;
    ListNode* tail = &dummy;

    ListNode* a = l1;
    ListNode* b = l2;
    while (a != NULL && b != NULL) {
        if (a->val <= b->val) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    if (a != NULL) tail->next = a;
    if (b != NULL) tail->next = b;

    return dummy.next;
}

// Remove the nth node from the end of a linked list
ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy;
    dummy.val = 0;
    dummy.next = head;

    ListNode* fast = &dummy;
    ListNode* slow = &dummy;

    for (int i = 0; i <= n; i++) {
        if (fast == NULL) return head; // n too large
        fast = fast->next;
    }

    while (fast != NULL) {
        fast = fast->next;
        slow = slow->next;
    }

    ListNode* doomed = slow->next;
    if (doomed != NULL) {
        slow->next = doomed->next;
        free(doomed);
    }
    return dummy.next;
}

int main() {
    ListNode* head = NULL;
    insertHead(&head, 5);
    insertHead(&head, 4);
    insertHead(&head, 3);
    insertHead(&head, 2);
    insertHead(&head, 1);

    // Insert 7 at the end
    ListNode* tail = head;
    while (tail->next != NULL) {
        tail = tail->next;
    }

    insertAfter(tail, 6);
    
    printList(head);

    deleteAfter(tail);

    printList(head);

    head = reverseList(head);

    printList(head);

    printf("\nHas cycle: %s", hasCycle(head) ? "true" : "false");

    ListNode* cycle = NULL;
    insertHead(&cycle, 4);
    insertHead(&cycle, 3);
    insertHead(&cycle, 2);
    insertHead(&cycle, 1);

    printList(cycle);

    // Get pointers to the nodes we want
    ListNode* p = cycle->next->next;          // node 3
    ListNode* q = cycle->next->next->next;    // node 4

    // Create the cycle
    q->next = p; // 4→3

    printf("\nHas cycle: %s", hasCycle(cycle) ? "true" : "false");

    head = reverseList(head);

    printList(head);
    ListNode* mid = middleNode(head);

    if (mid != NULL) {
        printf("\nMiddle node value: %d", mid->val);
    }

    ListNode* more = NULL;
    insertHead(&more, 8);
    insertHead(&more, 7);
    insertHead(&more, 6);

    printList(more);

    ListNode* merged = mergeTwoLists(head, more);
    printList(merged);

    // Remove 7
    merged = removeNthFromEnd(merged, 2);
    printList(merged);

    return 0;
}
