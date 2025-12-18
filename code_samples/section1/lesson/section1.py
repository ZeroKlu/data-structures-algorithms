"""Section 1 - Foundations"""

#pylint: disable=consider-using-enumerate

def print_array_elements(arr):
    """Prints all elements of an array"""
    for x in arr:
        print(x)

def print_element_pairs(arr):
    """Prints all pairs of elements of an array"""
    for i in range(len(arr)):
        for j in range(i, len(arr)):
            print(arr[i], arr[j])

def print_first_and_last(arr):
    """Prints the first and last elements of the list"""
    if arr:
        print(arr[0], arr[-1])

def print_array_twice(arr):
    """Prints the list twice"""
    for x in arr:
        print(x)
    for x in arr:
        print(x)

def double_array_elements(arr):
    """Returns a list of x*2 for each x in arr"""
    return [x * 2 for x in arr]

lst = [1, 2, 3, 4, 5]

print("Ex 1:")
print_array_elements(lst)

print("\nEx 2:")
print_element_pairs(lst)

print("\nEx 3:")
print_first_and_last(lst)

print("\nEx 4:")
print_array_twice(lst)

print("\nEx 5:")
res = double_array_elements(lst)
for n in res:
    print(n)
