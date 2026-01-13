# Section 1 — Problem Set (Big-O & Complexity) — Lecture Script

## How to run this page (teacher flow)
- Tell students: “This is about **reading code and descriptions** and turning them into **time + space**.”
- Set expectations: they should answer **before** expanding “Show answer.”
- Use a consistent routine for every problem:
  1) Identify what `n` represents.
  2) Count loops / iterations (time).
  3) Identify allocations / recursion depth (extra space).
  4) Simplify: drop constants + keep dominant term.

**Suggested pacing:** 25–40 minutes depending on discussion depth.

---

## Warm-up (1 minute)
Quick reminder:
- **Time** = how many “steps” scale with `n`
- **Extra space** = additional memory allocated during execution (beyond input)

---

## Problem 1 — Dominant Term

### What to emphasize while teaching
This is a “simplify the expression” problem:
- Convert each bullet into a term
- Add them up
- Drop constants + lower-order terms

### Live walkthrough (talk track)
- Setup is constant → `5`
- Loop runs `3n` times → `3n`
- “Nested loop” here is misleadingly named: inner is constant `2`
  - Outer `n` × inner `2` → `2n`
- Total: `5 + 3n + 2n = 5 + 5n`
- Big-O ignores:
  - additive constants → `5n`
  - constant factors → `n`
- Final: **O(n)**

### Quick checkpoint questions
- “If the inner loop were `n` instead of `2`, what would change?” (would become O(n²))
- “Which term dominates as `n` grows?” (the linear terms)

---

## Problem 2 — Time Complexity from Code

### What to emphasize
This is **triangular number** counting:
- nested loops, but inner starts at `i+1`
- total inner runs is a sum

### Live walkthrough
- Outer loop: `i = 0..n-1` → `n` iterations
- For each `i`, `j` runs from `i+1` to `n-1`:
  - about `n-i-1` iterations
- Total work:
  - `(n-1) + (n-2) + ... + 1 + 0 = n(n-1)/2` → **O(n²)**

Space:
- Only a few variables (`count`, `i`, `j`) → **O(1)** extra space

### Quick checkpoint questions
- “Roughly how many pairs are checked?” (~ n²/2)
- “Does the `if` change Big-O?” (no—still constant work per iteration)

---

## Problem 3 — Comparing Growth Rates

### What to emphasize
You’re teaching the “growth-rate ladder”:
- log < linear < n log n < polynomial < exponential

### Live walkthrough (talk track)
- Log grows slowest: `log n`
- Then linear: `n`
- Then `n log n` (a bit above linear)
- Polynomials: compare exponents → `n²`, then `n³`
- Exponentials dominate polynomials: `2ⁿ` is fastest

Final order:
1. `log n`
2. `n`
3. `n log n`
4. `n²`
5. `n³`
6. `2ⁿ`

### Quick checkpoint questions
- “Why is `n log n` bigger than `n`?” (log n grows, even if slowly)
- “Why does `2ⁿ` beat `n³` eventually?” (exponential vs polynomial)

---

## Problem 4 — Space Complexity with an Extra Array

### What to emphasize
Two separate questions:
- loop iterations → time
- size of new array → space

### Live walkthrough
Time:
- Loop from `1` to `n-1` → `n-1` iterations → **O(n)**
Space:
- `prefix = new Array(n)` allocates n items → **O(n)** extra space

### Optional extension
- “Can we compute prefix sums in-place?” (yes, if allowed to modify input; still O(1) extra space)

---

## Problem 5 — Nested Loops with Different Bounds (doubling loops)

### What to emphasize
This is a classic “**log loop inside log loop**” pattern:
- Doubling until `n` → **log n**
- Doubling until `i` → **log i**
- Sum of logs across powers of two → **(log n)²**

### Live walkthrough (clean version)
Outer loop:
- `i = 1, 2, 4, 8, ...` until `n`
- iterations ≈ `log₂ n`

Inner loop (for a fixed `i`):
- `j = 1, 2, 4, ...` until `i`
- iterations ≈ `log₂ i`

Total work:
- i takes values `2^0, 2^1, ..., 2^k` where `k ≈ log₂ n`
- inner cost at `i = 2^t` is `log₂(2^t) = t`
- sum is `0 + 1 + 2 + ... + k = k(k+1)/2 = O(k²) = O((log n)²)`

Space:
- constant variables only → **O(1)** extra space

### Quick checkpoint questions
- “How many times does `i` double before passing `n`?” (log n)
- “If inner loop used `j += 1` instead of `j *= 2`, what happens?” (inner becomes O(i); total becomes O(n))

---

## Wrap-up (1 minute)
- Most complexity analysis is pattern recognition:
  - single loop → O(n)
  - nested loops → O(n²) (unless bounds shrink or jump)
  - doubling/halving loops → O(log n)
- Next: apply these instincts to real structures (arrays/strings).
