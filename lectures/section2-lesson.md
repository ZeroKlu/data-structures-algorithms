# Section 2 — Arrays & Strings (Lecture Script)

## Learning goals
By the end of this section, students should be able to:
- Explain what an **array** is (contiguous memory, indexed access) and how a **string** is often “array-like.”
- Recognize when problems reduce to common patterns:
  - **simple traversal**
  - **two pointers (ends)**
  - **two pointers (fast/slow runner)**
- Reason about **in-place vs copying** and how that affects **extra space**.
- Apply two pointers to a **sorted two-sum** problem.

---

## Suggested pacing (45–60 minutes)
- 2.1 Arrays vs strings: 8–10 min
- 2.2 Patterns overview: 8–10 min
- Examples 2.4–2.7: 20–30 min
- 2.8 Quick practice: 8–10 min

---

## 2.1 Arrays vs. Strings

### Talk track
- **Array:** contiguous block of same-type slots; **O(1)** indexed access (`arr[i]`).
- **String:** typically a sequence of characters; many string problems are solved like arrays.

### Emphasize (what students often miss)
- In higher-level languages “arrays/lists” may resize, but resizing can trigger a **copy** (still usually amortized).
- Many string operations *look* simple but allocate behind the scenes because strings are often **immutable**.

### “Dig deeper” highlights to mention
- Language differences:
  - C strings are `char*` with a terminating `'\0'`.
  - Java/C#/JS/Python strings are typically immutable.
- Complexity facts:
  - random access often O(1)
  - insert in middle typically O(n)
  - resizing dynamic arrays sometimes allocates + copies

### Checkpoint question
> If inserting in the middle of an array requires shifting, what’s the time complexity? (O(n))

---

## 2.2 Common Array Patterns

### Introduce the three patterns (plus one bonus)
1) **Simple traversal** — one index from 0..n-1  
2) **Two pointers (ends)** — left and right move inward  
3) **Two pointers (runner)** — one moves faster  
**Bonus:** sliding window — maintain a [left, right) window

### Teach the “pattern selection” checklist
- “Do I need every element at least once?” → traversal  
- “Is it sorted or symmetric from ends?” → two pointers ends  
- “Need cycles / spacing / partition behavior?” → runner  
- “Need a contiguous subarray/substring?” → sliding window  

---

## 2.4 Example 1 — Sum of an Array (O(n))

### Walkthrough
- One loop touches each element once.
- Constant work per element (add + assign).
- Time: **O(n)**
- Extra space: **O(1)**

### Instructor notes
- Mention overflow briefly (real-world detail)
- Empty array should return 0

### Checkpoint question
> Can any correct sum algorithm do better than O(n)? (No—you must at least read every element.)

---

## 2.5 Example 2 — Reverse an Array In Place (Two Pointers)

### Teach the invariant (use this line explicitly)
“As we move inward, everything **outside [left, right]** is already in final position.”

### Walkthrough
- Start `left=0`, `right=n-1`
- Swap, then `left++`, `right--`
- Each element gets swapped at most once

Time: **O(n)**  
Extra space: **O(1)**

### Follow-up discussion
- In-place is space-efficient but **destroys original order**
- Copying reversal keeps input intact but costs **O(n)** space

### Quick demo prompt
Ask learners to reverse `[1,2,3,4,5]` and narrate pointer moves.

---

## 2.6 Example 3 — Two-Sum in a Sorted Array (Two Pointers)

### Key idea
Sorting gives you **monotonic control** over the sum:
- if sum too small → move left rightward to increase
- if sum too big → move right leftward to decrease

### Walkthrough (talk track)
- Initialize `left=0`, `right=n-1`
- Compute sum
- Adjust the pointer that *can* fix the direction (because sorted)

Time: **O(n)** (each pointer moves at most n steps)  
Extra space: **O(1)**

### Important precondition
This logic requires the array to be **sorted**. Without sorting:
- common approach: hash set / map → **O(n)** time, **O(n)** space

### Checkpoint question
> If the array is NOT sorted, can we still do two pointers like this? (Not reliably—need sorting or hashing.)

---

## 2.7 Example 4 — Reverse a String

### Main message
Strings are often **immutable**, so reversal usually builds a **new** string.

### Walkthrough by language (high-level)
- Java/C#: use builder or char array then convert back
- JS: split → reverse → join (still O(n) work + extra memory)
- Python: slicing `[::-1]` returns new string

Time: **O(n)**  
Extra space: typically **O(n)**

### Subtlety worth mentioning
For Unicode / multibyte encodings, “character” can mean different units. In interviews: clarify.

---

## 2.8 Quick Practice (run interactively)
Have students answer each, then reveal.

Coach them to say:
- “Time is O(n) because …”
- “Extra space is O(1)/O(n) because …”

---

## Wrap-up (1 minute)
- Arrays/strings problems are mostly **pattern recognition**.
- Next: linked lists + recursion, where pointer manipulation and stack space become central.
