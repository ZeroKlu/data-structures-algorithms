# Section 4 — Stacks & Queues: Problems, Patterns, and Solutions

This lecture covers the Section 4 problem set end-to-end.
The focus is not just on implementing solutions, but on recognizing
**when** to use stacks or queues, **what invariant they maintain**, and
**why each solution runs in linear time**.

---

## Big Picture: What stacks and queues really do

Stacks and queues are not about storage — they are about **order of processing**.

- **Stack (LIFO)** → “most recent still matters”
- **Queue (FIFO)** → “first discovered gets processed first”

Nearly every problem in this section can be solved once you correctly identify
which processing order the problem demands.

---

# Problem 1 — Valid Parentheses

## Problem recap
Given a string of brackets, determine whether:
- Every opening bracket is closed
- It is closed by the **correct type**
- It is closed in the **correct order**

---

## Why this is a stack problem

The key phrase is:
> “Correct order”

The **most recent unmatched opening bracket** must be the first one to close.
That is exactly what a stack represents.

---

## Algorithm intuition

The stack tracks the set of opening brackets that are currently “waiting to be closed”.

### Invariant
After processing the first `i` characters:
- The stack contains **exactly** the unmatched opening brackets
- The top of the stack is the **most recent** unmatched opener

---

## Step-by-step logic

1. Initialize an empty stack.
2. Scan the string from left to right:
   - If the character is an opening bracket, push it.
   - If it is a closing bracket:
     - If the stack is empty → invalid.
     - Pop the top and check that it matches the closing bracket.
3. After the scan:
   - If the stack is empty → valid.
   - Otherwise → invalid (some openings never closed).

---

## Why this is O(n)

Each character is:
- Pushed at most once
- Popped at most once

No nested loops, no rescanning → **O(n) time**, **O(n) space**.

---

# Problem 2 — Implement a Queue Using Two Stacks

## Problem recap
Implement a FIFO queue using only stack operations, with **amortized O(1)** time.

---

## Key idea

Use **two stacks**, each with a single responsibility:

- **`in` stack** → handles enqueue
- **`out` stack** → handles dequeue/front

This separation makes the queue behavior emerge naturally.

---

## Mental model

- `in` stack stores the back of the queue
- `out` stack stores the front of the queue (in reversed order)

---

## Core invariant

At all times:
- If `out` is non-empty, its top is the front of the queue
- Elements move from `in` to `out` **only when needed**

---

## Step-by-step logic

1. **Enqueue(x)**:
   - Push `x` onto `in`
2. **Dequeue / Front**:
   - If `out` is empty:
     - Pop everything from `in` and push it onto `out`
   - Pop or peek from `out`

---

## Why amortized O(1) works

Each element:
- Is pushed onto `in` once
- Is moved from `in` to `out` once
- Is popped once

So over `m` operations, total work is O(m).
Average (amortized) cost per operation is **O(1)**.

---

# Problem 3 — Next Greater Element (Monotonic Stack)

## Problem recap
For each element in an array, find the **next greater element to the right**.

---

## Why brute force fails

Checking each element against all elements to its right leads to **O(n²)** time.

We need a way to:
- Reuse work
- Avoid rescanning

---

## Key idea: Monotonic stack

We maintain a stack that is:
- **Monotonically decreasing**
- Represents “useful future candidates”

---

## Why scan from right to left

When we process index `i`, all elements to the right have already been processed
and summarized in the stack.

---

## Invariant

At index `i`:
- The stack contains values to the right of `i`
- The stack is strictly decreasing from bottom to top
- The top of the stack (if any) is the next greater value

---

## Step-by-step logic

1. Scan from right to left.
2. For current value `x`:
   - Pop from the stack while `stack.top <= x`
   - If stack is empty → answer is `-1`
   - Else → answer is `stack.top`
3. Push `x` onto the stack.

---

## Why this is O(n)

Each element is:
- Pushed once
- Popped once

Total stack operations ≤ 2n → **O(n) time**, **O(n) space**.

---

# Problem 4 — Daily Temperatures (Monotonic Stack of Indices)

## Problem recap
For each day, find how many days you must wait until a warmer temperature.

This is a **next-greater-element variant**, but we need distances, not values.

---

## Key twist

Instead of storing values in the stack, we store **indices**.

That lets us compute: `days waited = current_index - previous_index`


---

## Invariant

The stack holds indices such that:
- Temperatures are strictly decreasing
- Each index has not yet found a warmer day

---

## Step-by-step logic

1. Scan from left to right.
2. For day `i`:
   - While stack is not empty and `temps[i] > temps[stack.top]`:
     - Pop index `idx`
     - Set `ans[idx] = i - idx`
   - Push `i` onto the stack
3. Remaining indices never find a warmer day → answer stays `0`.

---

## Why this is O(n)

Each index:
- Is pushed once
- Is popped once

No index ever returns to the stack → **O(n) time**, **O(n) space**.

---

# Problem 5 — Level Order Traversal (BFS Using a Queue)

## Problem recap
Traverse a binary tree level by level from top to bottom.

---

## Why this is a queue problem

Level order traversal is **breadth-first search (BFS)**.

BFS requires:
- Processing nodes in the order they are discovered
- FIFO behavior → queue

---

## Core BFS invariant

- Nodes are dequeued in increasing depth order
- All nodes at depth `d` are processed before nodes at depth `d+1`

---

## Step-by-step logic

1. If root is null → return empty result.
2. Enqueue the root.
3. While queue is not empty:
   - Record `levelSize = queue.size()`
   - Process exactly `levelSize` nodes:
     - Dequeue node
     - Add its value to current level
     - Enqueue its children
   - Append current level to result

---

## Why queue size matters

The queue may contain nodes from multiple levels.
Capturing the size at the start of each iteration ensures:
- Exactly one level is processed at a time.

---

## Complexity

Each node is:
- Enqueued once
- Dequeued once

→ **O(n) time**, **O(n) space**.

---

# Problem 6 — Sliding Window Maximum (Monotonic Deque)

## Problem recap
Given a window of size `k`, find the maximum in each sliding window.

Brute force scanning each window costs **O(nk)**.

---

## Key idea: Monotonic deque

We maintain a deque of **indices**, not values.

The deque is kept in **decreasing order of values**.

---

## Invariant

At index `i`:
- All indices in deque are within the current window
- Values decrease from front to back
- The front of the deque is always the window maximum

---

## Step-by-step logic

For each index `i`:
1. Remove indices from the front that are out of the window.
2. Remove indices from the back whose values are ≤ `nums[i]`.
3. Push `i` to the back.
4. Once `i >= k - 1`, record `nums[deque.front]` as the maximum.

---

## Why this works

Any index with a smaller value than a newer element:
- Will never become a maximum again
- Can be safely discarded

---

## Complexity

Each index:
- Is pushed once
- Is popped at most once

→ **O(n) time**, **O(n) space**.

---

# Final Pattern Summary

Across all Section 4 problems, the same ideas repeat:

- **Stack** → most recent unmatched / most recent decision
- **Queue** → first discovered / level order
- **Monotonic stack or deque** → discard dominated elements early
- **Invariants** explain correctness and guarantee linear time

If you can clearly explain:
- What the data structure stores
- What invariant it maintains
- Why elements are pushed and popped only once

You are already giving a strong interview-level solution.

