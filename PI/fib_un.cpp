#include <iostream>
#include <chrono>

int fib(int n) {
    if (n <= 1) {
        return n;
    } else {
        return fib(n - 1) + fib(n - 2);
    }
}

int main() {
    int n;

    // Accept input from the user
    std::cout << "Enter the value of N: ";
    std::cin >> n;

    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    // Calculate Fibonacci using unoptimized recursion
    int result = fib(n);

    // Stop measuring time
    auto stop = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // Display the result and the execution time
    std::cout << "Fibonacci of " << n << " is " << result << std::endl;
    std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;

    return 0;
}
