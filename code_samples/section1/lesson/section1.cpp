#include <iostream>
#include <vector>
using namespace std;

// Print each element of an array
void ex1(const vector<int>& arr) {
    for (int x : arr) {
        cout << x << endl;
    }
}

// Print all pairs of elements of an array
void ex2(const vector<int>& arr) {
    for (int i = 0; i < arr.size(); i++) {
        for (int j = i; j < arr.size(); j++) {
            cout << arr[i] << ", " << arr[j] << endl;
        }
    }
}

// Print the first and last elements of an array
void ex3(const vector<int>& arr) {
    if (!arr.empty()) {
        cout << arr.front() << " " << arr.back() << endl;
    }
}

// Print each element of an array twice
void ex4(const vector<int>& arr) {
    for (int x : arr) {
        cout << x << endl;
    }
    for (int x : arr) {
        cout << x << endl;
    }
}

// Double each element of an array
vector<int> ex5(const vector<int>& arr) {
    vector<int> res;
    res.reserve(arr.size());
    for (int x : arr) {
        res.push_back(x * 2);
    }
    return res;
}

int main() {
    vector<int> arr = {1, 2, 3, 4, 5};

    cout << "Ex 1:\n" << endl;
    ex1(arr);

    cout << "\nEx 2:\n" << endl;
    ex2(arr);

    cout << "\nEx 3:\n" << endl;
    ex3(arr);

    cout << "\nEx 4:\n" << endl;
    ex4(arr);

    cout << "\nEx 5:\n" << endl;
    vector<int> res = ex5(arr);
    for (int x : res) {
        cout << x << endl;
    }
}
