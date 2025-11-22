static void Ex1(int[] arr)
{
    foreach (var x in arr)
    {
        Console.WriteLine(x);
    }
}

static void Ex2(int[] arr)
{
    for (int i = 0; i < arr.Length; i++)
    {
        for (int j = i; j < arr.Length; j++)
        {
            Console.WriteLine($"{arr[i]}, {arr[j]}");
        }
    }
}

static void Ex3(int[] arr)
{
    if (arr.Length > 0)
    {
        Console.WriteLine($"{arr[0]} {arr[^1]}");
    }
}

static void Ex4(int[] arr)
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

static int[] Ex5(int[] arr)
{
    return arr.Select(x => x * 2).ToArray();
}

int[] arr = [1, 2, 3, 4, 5];

Console.WriteLine("Ex 1:");
Ex1(arr);

Console.WriteLine("\nEx 2:");
Ex2(arr);

Console.WriteLine("\nEx 3:");
Ex3(arr);

Console.WriteLine("\nEx 4:");
Ex4(arr);

Console.WriteLine("\nEx 5:");
int[] res = Ex5(arr);
foreach (var x in res)
{
    Console.WriteLine(x);
}
