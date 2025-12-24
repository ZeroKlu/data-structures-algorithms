# Section 2 — Problem Set (Lecture Script)

## Teaching goals for the problem set
Students should practice:
- Recognizing the **core pattern** (two pointers, fast check with hashing, stable compaction, sliding window, reverse trick)
- Explaining **why** it works (invariant / “what stays true”)
- Stating **time + extra space** clearly and correctly

## Suggested pacing (45–75 minutes)
- P1–P3 (arrays + pointers): 20–30 min
- P4 (string freq counting): 8–10 min
- P5 (sliding window): 12–18 min
- P6 (rotate via reversals): 10–15 min

---

## Problem 1 — Reverse an Array In-Place
### What to elicit
- Pattern: **two pointers from both ends**
- Invariant: elements outside `[i, j]` are already in correct final positions

### Instructor prompts
- “What index does element k map to after reversal?” → `n - 1 - k`
- “How many swaps occur?” → about `n/2`

### Expected complexity
- Time: **O(n)**
- Extra space: **O(1)**

---

## Problem 2 — Two-Sum (Brute Force vs Hash Map)
### What to elicit
- Brute force: check all pairs `(i, j)` with `i < j` → **O(n²)**
- Hash-based: single pass + “have we seen the complement?” → expected **O(n)**

### Instructor prompts
- “Why is it okay that we only look backward in the hash approach?”
- “What assumption are we making about hash operations?” (expected O(1))

### Expected complexity
- Brute force: time **O(n²)**, extra space **O(1)**
- Hash-based: time **O(n)** expected, extra space **O(n)**

---

## Problem 3 — Move Zeros to the End (stable, in-place)
### What to elicit
- Pattern: **write pointer** (stable compaction)
- Two phases:
  1) copy non-zeros forward
  2) fill remainder with zeros

### Instructor prompts
- “Why does this preserve order?”
- “Do we ever swap?” (No—copy forward in encounter order)

### Expected complexity
- Time: **O(n)**
- Extra space: **O(1)**

---

## Problem 4 — Check for Anagram (a–z only)
### What to elicit
- Use a fixed 26-length frequency array (constant alphabet)
- Increment for `s`, decrement for `t` in the same pass

### Instructor prompts
- “Why do we do a length check first?”
- “What makes space O(1) here?”

### Expected complexity
- Time: **O(n)**
- Extra space: **O(1)**

---

## Problem 5 — Longest Substring Without Repeating Characters
### What to elicit
- Pattern: **sliding window** with last-seen index map
- Invariant: window `[left, right]` contains **no duplicates**
- When repeat occurs, jump `left` forward: `left = last_pos[ch] + 1` (but never backward)

### Instructor prompts
- “Why doesn’t this become O(n²)?”
- “What exactly does `last_pos[ch] >= left` mean?” (repeat is inside current window)

### Expected complexity
- Time: **O(n)** (each pointer/index moves forward at most once)
- Extra space: **O(min(n, alphabet))**

---

## Problem 6 — Rotate Array Right by k (in-place)
### What to elicit
- Normalize: `k = k % n`
- Triple reversal trick:
  1) reverse entire array
  2) reverse first `k`
  3) reverse remaining `n-k`

### Instructor prompts
- “What does reversing the whole array accomplish?”
- “Why do we need the next two reversals?”
- Walk a concrete example: `[1,2,3,4,5,6,7], k=3`

### Expected complexity
- Time: **O(n)**
- Extra space: **O(1)**

---

## Wrap-up
- Most array/string interview problems are: pick the right **pattern + invariant**
- If a solution is “fast,” ask what it traded (often **space for time** with hashing/maps)
