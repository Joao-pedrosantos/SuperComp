#include <iostream>
#include <vector>
#include <chrono>  // For measuring time

// Function to calculate Fibonacci with memoization
int fib(int n, std::vector<int>& memo) {
    if (n <= 1) {
        return n;
    }

    // If the value has already been computed, return it
    if (memo[n] != -1) {
        return memo[n];
    }

    // Otherwise, calculate and store it in the memoization table
    memo[n] = fib(n - 1, memo) + fib(n - 2, memo);
    return memo[n];
}

int main() {
    int n;

    // Accept input from the user
    std::cout << "Enter the value of N: ";
    std::cin >> n;

    // Create a memoization table initialized to -1 (meaning uncomputed)
    std::vector<int> memo(n + 1, -1);

    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    // Calculate Fibonacci with memoization
    int result = fib(n, memo);

    // Stop measuring time
    auto stop = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // Display the result and the execution time
    std::cout << "Fibonacci of " << n << " is " << result << std::endl;
    std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;

    return 0;
}
