#include <iostream>
#include <vector>
#include <queue>
using namespace std;

/*
    This program merges K sorted singly linked lists into one sorted list using
    a priority queue (min-heap) of ListNode*.

    High-level algorithm (classic "k-way merge"):
      1) Put the head of each non-empty list into a min-heap ordered by node->val.
      2) Repeatedly:
           - Extract the smallest node from the heap
           - Append it to the output list
           - If that node has a next node, push node->next into the heap
      3) Continue until the heap is empty.

    Why a min-heap?
      - At any time, the next smallest value must be one of the current heads
        of the remaining lists.
      - A min-heap gives O(log k) extraction/insertion for those candidates.

    Complexity:
      - Let N be the total number of nodes across all lists.
      - Heap size is at most k.
      - Each node is pushed and popped at most once: O(N log k) time, O(k) space.

    Memory behavior:
      - mergeKLists() REUSES the original nodes (no new nodes allocated there).
      - The test harness allocates nodes with new and frees the merged list with delete.
*/

// Basic singly-linked node definition used throughout tests and merge code.
struct ListNode {
    int val;
    ListNode *next;

    // Convenience constructor to initialize val and next pointer.
    ListNode(int v) : val(v), next(nullptr) {}
};

/*
    Comparator for the priority_queue.

    Important:
      - std::priority_queue is a MAX-HEAP by default.
      - We invert comparison so the "smallest" value ends up on top,
        producing MIN-HEAP behavior.

    In other words:
      - If a->val > b->val returns true, then 'a' has lower priority than 'b',
        making smaller values rise to the top.
*/
struct Cmp {
    bool operator()(ListNode *a, ListNode *b) const {
        return a->val > b->val;  // min-heap: smallest val is highest priority
    }
};

/*
    mergeKLists
    -----------
    Merges all lists in 'lists' into a single sorted list.

    Parameters:
      - lists: vector of head pointers, each list individually sorted ascending.
               The vector itself may be empty and may contain nullptr entries.

    Returns:
      - Head pointer to merged sorted list (or nullptr if all lists are empty).

    Notes:
      - This function DOES NOT allocate or free nodes. It relinks existing nodes.
      - The caller owns the nodes and should free the returned list.
*/
ListNode *mergeKLists(vector<ListNode *> &lists) {
    // Min-heap of ListNode* ordered by node->val.
    priority_queue<ListNode *, vector<ListNode *>, Cmp> pq;

    // Seed heap with the first node of each non-empty list.
    for (auto node : lists) {
        if (node) pq.push(node);
    }

    // Dummy node simplifies output list construction (no special-case for first append).
    ListNode dummy(0);
    ListNode *tail = &dummy; // tail always points to last node in merged list

    // While there are still candidate nodes across lists...
    while (!pq.empty()) {
        // Extract the smallest available node.
        ListNode *node = pq.top();
        pq.pop();

        // Append it to the merged list.
        tail->next = node;
        tail = tail->next;

        // If there is a next node in the extracted node's original list,
        // push that next node into the heap as a new candidate.
        if (node->next) {
            pq.push(node->next);
        }
    }

    // Optional safety: ensure the merged list terminates cleanly.
    // In well-formed acyclic input lists this is already true, but it doesn't hurt.
    tail->next = nullptr;

    return dummy.next; // head of merged list (nullptr if no nodes were appended)
}

// ===== utilities for testing =====

/*
    buildList
    ---------
    Allocates a new linked list from the given values using 'new'.

    Parameters:
      - vals: values to put into the list in the given order

    Returns:
      - head pointer (or nullptr if vals is empty)

    Ownership:
      - Caller must delete nodes (freeList) when done.
*/
ListNode *buildList(const vector<int> &vals) {
    ListNode *head = nullptr;
    ListNode *tail = nullptr;

    for (int v : vals) {
        auto *node = new ListNode(v);

        if (!head) {
            // First node sets both head and tail.
            head = tail = node;
        } else {
            // Append at the tail.
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

/*
    printList
    ---------
    Prints a list as space-separated values preceded by a label.

    If head is nullptr, prints just the label and newline.
*/
void printList(const string &label, ListNode *head) {
    cout << label;
    for (ListNode *p = head; p != nullptr; p = p->next) {
        cout << p->val << " ";
    }
    cout << "\n";
}

/*
    freeList
    --------
    Deletes every node in the list (assumes nodes allocated with 'new').

    Important:
      - After mergeKLists, the merged list contains the original nodes.
      - Freeing the merged list frees all original lists' nodes as well.
*/
void freeList(ListNode *head) {
    while (head) {
        ListNode *next = head->next;
        delete head;
        head = next;
    }
}

// ===== test function =====

/*
    test_mergeKLists
    ----------------
    Runs multiple test cases to validate mergeKLists:

      Test 1: three sorted lists
      Test 2: mixture of empty and non-empty lists
      Test 3: all lists empty
      Test 4: single list
      Test 5: no lists at all (empty vector)

    Each test prints inputs and merged result, plus an expected output line.
*/
void test_mergeKLists() {
    // ---------- Test 1: three sorted lists ----------
    cout << "=== Test 1: three sorted lists ===\n";
    ListNode *l1 = buildList({1, 4, 7});
    ListNode *l2 = buildList({2, 5, 8});
    ListNode *l3 = buildList({3, 6, 9});
    vector<ListNode *> lists1 = {l1, l2, l3};

    printList("List 1: ", l1);
    printList("List 2: ", l2);
    printList("List 3: ", l3);

    ListNode *merged1 = mergeKLists(lists1);
    printList("Merged: ", merged1);
    cout << "(Expected: 1 2 3 4 5 6 7 8 9)\n\n";

    // merged list reuses original nodes, so freeing merged1 frees l1/l2/l3 too
    freeList(merged1);

    // ---------- Test 2: mixture of empty and non-empty ----------
    cout << "=== Test 2: empty + non-empty ===\n";
    ListNode *a = buildList({1, 3, 5});
    ListNode *b = nullptr;
    ListNode *c = buildList({2, 4, 6, 8});
    ListNode *d = nullptr;

    vector<ListNode *> lists2 = {a, b, c, d};

    printList("List A: ", a);
    printList("List B: ", b);
    printList("List C: ", c);
    printList("List D: ", d);

    ListNode *merged2 = mergeKLists(lists2);
    printList("Merged: ", merged2);
    cout << "(Expected: 1 2 3 4 5 6 8)\n\n";

    freeList(merged2);

    // ---------- Test 3: all lists empty ----------
    cout << "=== Test 3: all empty ===\n";
    vector<ListNode *> lists3 = {nullptr, nullptr, nullptr};

    ListNode *merged3 = mergeKLists(lists3);
    printList("Merged: ", merged3);
    cout << "(Expected: <empty>)\n\n";

    // nothing to free since merged3 is null

    // ---------- Test 4: single list ----------
    cout << "=== Test 4: single list ===\n";
    ListNode *single = buildList({10, 20, 30});
    vector<ListNode *> lists4 = {single};

    printList("Input:  ", single);

    // For a single list, mergeKLists should effectively return the same sequence.
    ListNode *merged4 = mergeKLists(lists4);
    printList("Merged: ", merged4);
    cout << "(Expected: 10 20 30)\n\n";

    freeList(merged4);

    // ---------- Test 5: no lists at all ----------
    cout << "=== Test 5: zero lists ===\n";
    vector<ListNode *> lists5; // empty vector

    // mergeKLists should safely return nullptr when there are no lists.
    ListNode *merged5 = mergeKLists(lists5);
    printList("Merged: ", merged5);
    cout << "(Expected: <empty>)\n\n";

    // merged5 is null, nothing to free
}

// ===== main =====

int main() {
    test_mergeKLists();
    return 0;
}
