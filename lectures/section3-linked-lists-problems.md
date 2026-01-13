# Section 3 — Linked Lists: Problem-Solving Patterns & Solutions

This lecture walks through the Section 3 problem set and solutions as a single narrative.
The goal is not just to “solve” each problem, but to recognize the recurring patterns
that appear again and again in linked list interview questions.

---

## Core idea of linked list problems

Linked list problems are rarely about clever math. They test whether you can:

- Track and preserve pointers correctly
- Maintain a clear invariant (“what is true at every step?”)
- Avoid losing access to part of the list
- Choose the right traversal pattern (single pointer, two pointers, dummy node)

A useful mental model is:

> Every linked list problem is about controlling who points to whom, and when.

---

## Problem 1 — Basic Singly Linked List Operations

### Operations covered
- Insert at head
- Insert after a given node
- Delete the node after a given node

### Key insight

Once you already have a pointer to the correct location in the list, you do **not**
need to traverse the list. Each operation modifies only a constant number of pointers.

That is why all three operations run in **O(1)** time.

---

### Insert at head

**Pattern:** Create a new first node.

**Steps:**
1. Allocate a new node.
2. Point the new node at the old head.
3. Update the head pointer to the new node.

**Invariant:**
The original list remains intact because the new node points to the old head.

**Interview takeaway:**
- Head insertion is always O(1).
- In C/C++, updating the head requires a pointer-to-pointer or returning the new head.

---

### Insert after a given node

**Pattern:** Splice a new node into the list.

**Steps:**
1. Check that the given node exists.
2. Point the new node at `p.next`.
3. Redirect `p.next` to the new node.

**Invariant:**
You must connect the new node to the remainder of the list **before**
redirecting `p.next`, or you risk losing the rest of the list.

---

### Delete the node after a given node

**Pattern:** Bypass a node.

**Steps:**
1. Ensure `p` and `p.next` exist.
2. Save `p.next` in a temporary variable.
3. Set `p.next = doomed.next`.
4. Free or drop the doomed node.

**Interview takeaway:**
Deletion in a singly linked list is O(1) **only if** you already have
a pointer to the previous node.

---

## Problem 2 — Reverse a Singly Linked List

**Pattern:** Three-pointer iterative reversal.

This is one of the most important linked list problems to master.

---

### Core invariant

At every step:
- `prev` points to the head of the reversed portion.
- `curr` points to the head of the remaining unreversed portion.

---

### Why three pointers are required

When you reverse a pointer (`curr.next = prev`), you destroy the original forward link.
You must save that link first, or the remainder of the list will be lost.

---

### Step-by-step logic

1. Save the remainder: `nxt = curr.next`.
2. Reverse the link: `curr.next = prev`.
3. Advance the reversed prefix: `prev = curr`.
4. Advance into the remainder: `curr = nxt`.

When `curr` becomes null, `prev` is the new head.

---

### Complexity

- Time: **O(n)** — each node is processed once.
- Extra space: **O(1)** — only a few pointers.

**Common bug:** Forgetting to save `curr.next` before overwriting it.

---

## Problem 3 — Detect a Cycle (Floyd’s Tortoise and Hare)

**Pattern:** Two pointers moving at different speeds.

---

### Pointer rules

- `slow` moves one step at a time.
- `fast` moves two steps at a time.

---

### Stopping conditions

1. **No cycle:**  
   If `fast` or `fast.next` becomes null, the list has an end.
2. **Cycle exists:**  
   If `slow == fast`, the pointers have met inside a loop.

---

### Why this works

Once both pointers are inside the cycle:
- Think of nodes as positions on a circle.
- Each iteration, `fast` gains one node on `slow`.
- On a finite cycle, this guarantees a meeting.

---

### Complexity

- Time: **O(n)**.
- Extra space: **O(1)**.

---

## Problem 4 — Find the Middle of a Linked List

**Pattern:** Slow/fast pointers again, used for positioning instead of detection.

---

### Logic

- `slow` moves one step per iteration.
- `fast` moves two steps per iteration.
- When `fast` reaches the end, `slow` is at the middle.

---

### Even-length lists

With the loop condition: `while (fast != null && fast.next != null)`,


`slow` ends at the **second** middle node for even-length lists.
This is the convention used by most interview platforms.

---

### Complexity

- Time: **O(n)**.
- Extra space: **O(1)**.

---

## Problem 5 — Merge Two Sorted Linked Lists

**Pattern:** Dummy head + tail pointer.

This is the linked list equivalent of the merge step in merge sort.

---

### Why a dummy node matters

The dummy node removes edge cases for the first insertion.
You always attach nodes to `tail.next`.

---

### Algorithm

1. Create `dummy` and set `tail = dummy`.
2. Use pointers `a` and `b` to walk through both lists.
3. Attach the smaller node to `tail`.
4. Advance the pointer you used.
5. Attach the remainder when one list ends.
6. Return `dummy.next`.

---

### Invariants

- The list starting at `dummy.next` is always sorted.
- `tail` always points to the last node of the merged list.

---

### Complexity

- Time: **O(n + m)**.
- Extra space: **O(1)** (nodes are reused).

---

## Problem 6 — Remove the N-th Node From the End

**Pattern:** Two pointers with a fixed gap + dummy head.

---

### Key idea

Maintain a gap of exactly `n` nodes between `fast` and `slow`.
When `fast` reaches the end, `slow` is just before the node to remove.

---

### Algorithm

1. Create `dummy → head`.
2. Set `fast = dummy` and `slow = dummy`.
3. Advance `fast` by `n + 1` steps.
4. Move both pointers until `fast` is null.
5. Remove `slow.next`.
6. Return `dummy.next`.

---

### Invariants

- The gap between `fast` and `slow` never changes.
- `slow` always trails one node behind the target.

---

### Complexity

- Time: **O(n)** — single pass.
- Extra space: **O(1)**.

---

## Final interview takeaways

Across all linked list problems, the same patterns repeat:

- Pointer rewiring instead of traversal
- Dummy nodes to simplify edge cases
- Slow/fast pointer techniques
- Clear loop invariants

If you can explain **why** the pointers move the way they do and
what remains true at each step, you are already giving a strong
interview-quality answer.


