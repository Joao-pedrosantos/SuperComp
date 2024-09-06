#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <numeric>
#include <unordered_map>
#include <climits>

using namespace std;
using namespace chrono;

typedef pair<int, int> Result;
unordered_map<int, Result> memoTable;

Result computeMaxValue(int remainingCapacity, const vector<int>& itemWeights, const vector<int>& itemValues, int currentItemIndex, int& numCombinations) {
    numCombinations++;
    if (currentItemIndex == 0 || remainingCapacity == 0) return {0, 0};
    int memoKey = remainingCapacity * 1000 + currentItemIndex;

    if (memoTable.find(memoKey) != memoTable.end()) return memoTable[memoKey];

    Result result;
    if (itemWeights[currentItemIndex - 1] > remainingCapacity) {
        result = computeMaxValue(remainingCapacity, itemWeights, itemValues, currentItemIndex - 1, numCombinations);
    } else {
        Result includeItem = computeMaxValue(remainingCapacity - itemWeights[currentItemIndex - 1], itemWeights, itemValues, currentItemIndex - 1, numCombinations);
        includeItem.first += itemValues[currentItemIndex - 1];
        includeItem.second += itemWeights[currentItemIndex - 1];
        Result excludeItem = computeMaxValue(remainingCapacity, itemWeights, itemValues, currentItemIndex - 1, numCombinations);
        result = (includeItem.first > excludeItem.first) ? includeItem : excludeItem;
    }
    memoTable[memoKey] = result;
    return result;
}

void displayResults(const string& method, int totalValue, int totalWeight, long long execTime, int iteration = 0) {
    if (iteration == 0) {
        cout << method << " Results:" << endl;
    } else {
        cout << method << " - Iteration " << iteration << ":" << endl;
    }
    cout << "Total Value: " << totalValue << endl;
    cout << "Total Weight: " << totalWeight << endl;
    cout << "Execution Time: " << execTime << " ns" << endl << endl;
}

void shuffleAndFill(int knapsackCapacity, vector<int>& weights, vector<int>& values, mt19937& generator) {
    for (int i = 0; i < 5; ++i) {
        shuffle(weights.begin(), weights.end(), generator);
        shuffle(values.begin(), values.end(), generator);
        int currentWeight = 0, totalValue = 0;

        auto startTime = high_resolution_clock::now();

        for (int j = 0; j < weights.size(); ++j) {
            if (currentWeight + weights[j] <= knapsackCapacity) {
                currentWeight += weights[j];
                totalValue += values[j];
            }
        }

        auto endTime = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(endTime - startTime).count();

        displayResults("Shuffle and Fill Method", totalValue, currentWeight, duration, i + 1);
    }
}

void randomSelection(int knapsackCapacity, vector<int>& weights, vector<int>& values, mt19937& generator) {
    uniform_int_distribution<int> distribution(0, weights.size() - 1);
    for (int i = 0; i < 5; ++i) {
        int currentWeight = 0, totalValue = 0;

        auto startTime = high_resolution_clock::now();

        for (int j = 0; j < weights.size(); ++j) {
            if (distribution(generator) % 2 == 0 && currentWeight + weights[j] <= knapsackCapacity) {
                currentWeight += weights[j];
                totalValue += values[j];
            }
        }

        auto endTime = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(endTime - startTime).count();

        displayResults("Random Selection Method", totalValue, currentWeight, duration, i + 1);
    }
}

// Hill Climbing Method
void hillClimbing(int knapsackCapacity, vector<int>& weights, vector<int>& values, mt19937& generator) {
    // Hill Climbing starts with a random solution and iteratively improves
    vector<int> currentSolution(weights.size(), 0);  // Start with an empty knapsack
    int currentWeight = 0;
    int currentValue = 0;

    // Random initialization
    for (int i = 0; i < weights.size(); ++i) {
        if (weights[i] + currentWeight <= knapsackCapacity) {
            currentSolution[i] = 1;  // Include item in the knapsack
            currentWeight += weights[i];
            currentValue += values[i];
        }
    }

    // Perform hill climbing for a fixed number of iterations
    uniform_int_distribution<int> distribution(0, weights.size() - 1);
    auto startTime = high_resolution_clock::now();

    for (int iteration = 0; iteration < 1000; ++iteration) {
        // Randomly select an item to try and flip (include/exclude)
        int randomIndex = distribution(generator);

        if (currentSolution[randomIndex] == 1) {
            // Try removing the item
            currentWeight -= weights[randomIndex];
            currentValue -= values[randomIndex];
            currentSolution[randomIndex] = 0;
        } else if (currentWeight + weights[randomIndex] <= knapsackCapacity) {
            // Try adding the item
            currentWeight += weights[randomIndex];
            currentValue += values[randomIndex];
            currentSolution[randomIndex] = 1;
        }

        // Terminate early if no improvement can be made
        if (currentWeight > knapsackCapacity) break;
    }

    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(endTime - startTime).count();

    displayResults("Hill Climbing", currentValue, currentWeight, duration);
}

// Object Substitution Method
void objectSubstitution(int knapsackCapacity, vector<int>& weights, vector<int>& values) {
    // Object Substitution swaps out items if needed to optimize
    vector<int> currentSolution(weights.size(), 0);
    int currentWeight = 0;
    int currentValue = 0;

    for (int i = 0; i < weights.size(); ++i) {
        if (weights[i] + currentWeight <= knapsackCapacity) {
            currentSolution[i] = 1;  // Include the item
            currentWeight += weights[i];
            currentValue += values[i];
        } else {
            // Find the least valuable item in the knapsack
            int minValue = INT_MAX;
            int minIndex = -1;
            for (int j = 0; j < weights.size(); ++j) {
                if (currentSolution[j] == 1 && values[j] < minValue) {
                    minValue = values[j];
                    minIndex = j;
                }
            }

            // Attempt to substitute if it improves value
            if (minIndex != -1 && values[i] > minValue && 
                currentWeight - weights[minIndex] + weights[i] <= knapsackCapacity) {
                currentWeight = currentWeight - weights[minIndex] + weights[i];
                currentValue = currentValue - values[minIndex] + values[i];
                currentSolution[minIndex] = 0;
                currentSolution[i] = 1;
            }
        }
    }

    auto startTime = high_resolution_clock::now();
    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(endTime - startTime).count();

    displayResults("Object Substitution", currentValue, currentWeight, duration);
}

int main() {
    vector<string> inputFiles = {"entrada1.txt", "entrada2.txt", "entrada3.txt"};

    for (const string& inputFileName : inputFiles) {
        ifstream inputFile(inputFileName);
        if (!inputFile) {
            cerr << "Failed to open input file: " << inputFileName << endl;
            continue;
        }

        int itemCount, knapsackCapacity;
        inputFile >> itemCount >> knapsackCapacity;

        vector<int> itemWeights(itemCount), itemValues(itemCount);
        for (int i = 0; i < itemCount; ++i) {
            inputFile >> itemWeights[i] >> itemValues[i];
        }

        // Clear the memoTable for each new input file
        memoTable.clear();

        mt19937 generator(random_device{}());
        int numCombinations = 0;
        auto start = high_resolution_clock::now();

        auto result = computeMaxValue(knapsackCapacity, itemWeights, itemValues, itemCount, numCombinations);

        auto end = high_resolution_clock::now();
        long long totalExecTime = duration_cast<nanoseconds>(end - start).count();
        
        cout << "Input File: " << inputFileName << endl;
        cout << "Number of Combinations: " << numCombinations << endl;

        displayResults("Exhaustive Method", result.first, result.second, totalExecTime);

        // Execute heuristics
        shuffleAndFill(knapsackCapacity, itemWeights, itemValues, generator);
        randomSelection(knapsackCapacity, itemWeights, itemValues, generator);
        hillClimbing(knapsackCapacity, itemWeights, itemValues, generator);
        objectSubstitution(knapsackCapacity, itemWeights, itemValues);

        cout << "--------------------------------------------------" << endl;
    }

    return 0;
}
