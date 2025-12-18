// Print each element of an array
static void PrintArrayElements(int[] arr)
{
    foreach (var x in arr)
    {
        Console.WriteLine(x);
    }
}

// Print all pairs of elements of an array
static void PrintElementPairs(int[] arr)
{
    for (int i = 0; i < arr.Length; i++)
    {
        for (int j = i; j < arr.Length; j++)
        {
            Console.WriteLine($"{arr[i]}, {arr[j]}");
        }
    }
}

// Print the first and last elements of an array
static void PrintFirstAndLast(int[] arr)
{
    if (arr.Length > 0)
    {
        Console.WriteLine($"{arr[0]} {arr[^1]}");
    }
}

// Print all elements of an array twice
static void PrintArrayTwice(int[] arr)
{
    foreach (var x in arr)
    {
        Console.WriteLine(x);
    }
    foreach (var x in arr)
    {
        Console.WriteLine(x);
    }
}

// Double each element of an array
static int[] DoubleArrayElements(int[] arr)
{
    return arr.Select(x => x * 2).ToArray();
}

int[] arr = [1, 2, 3, 4, 5];

Console.WriteLine("Ex 1:");
PrintArrayElements(arr);

Console.WriteLine("\nEx 2:");
PrintElementPairs(arr);

Console.WriteLine("\nEx 3:");
PrintFirstAndLast(arr);

Console.WriteLine("\nEx 4:");
PrintArrayTwice(arr);

Console.WriteLine("\nEx 5:");
int[] res = DoubleArrayElements(arr);
foreach (var x in res)
{
    Console.WriteLine(x);
}
