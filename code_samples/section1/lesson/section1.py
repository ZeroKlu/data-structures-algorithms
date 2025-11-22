"""Section 1 - Foundations"""
#pylint: disable=consider-using-enumerate

def ex1(arr):
    """Prints all elements of an array"""
    for x in arr:
        print(x)

def ex2(arr):
    """Prints all pairs of elements of an array"""
    for i in range(len(arr)):
        for j in range(i, len(arr)):
            print(arr[i], arr[j])

def ex3(arr):
    """Prints the first and last elements of the list"""
    if arr:
        print(arr[0], arr[-1])

def ex4(arr):
    """Prints the list twice"""
    for x in arr:
        print(x)
    for x in arr:
        print(x)

def ex5(arr):
    """Returns a list of x*2 for each x in arr"""
    return [x * 2 for x in arr]

lst = [1, 2, 3, 4, 5]

print("Ex 1:")
ex1(lst)

print("\nEx 2:")
ex2(lst)

print("\nEx 3:")
ex3(lst)

print("\nEx 4:")
ex4(lst)

print("\nEx 5:")
res = ex5(lst)
for n in res:
    print(n)
