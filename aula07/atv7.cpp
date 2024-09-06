#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <numeric>
#include <unordered_map>

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

void shuffleAndFill(int knapsackCapacity, vector<int>& weights, vector<int>& values, mt19937& gen) {
    int numItems = weights.size();
    vector<int> indices(numItems);
    iota(indices.begin(), indices.end(), 0);

    for (int i = 0; i < 5; ++i) {
        shuffle(indices.begin(), indices.end(), gen);

        int currentWeight = 0;
        int totalValue = 0;
        auto startTime = high_resolution_clock::now();

        for (int idx : indices) {
            if (currentWeight + weights[idx] <= knapsackCapacity) {
                currentWeight += weights[idx];
                totalValue += values[idx];
            }
        }

        auto endTime = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(endTime - startTime).count();

        displayResults("Shuffle and Fill Method", totalValue, currentWeight, duration, i + 1);
    }
}

void randomSelection(int knapsackCapacity, vector<int>& weights, vector<int>& values, mt19937& gen) {
    int numItems = weights.size();
    uniform_real_distribution<double> dist(0.0, 1.0);
    double probThreshold = 0.5;

    for (int i = 0; i < 5; ++i) {
        int currentWeight = 0;
        int totalValue = 0;
        auto startTime = high_resolution_clock::now();

        for (int j = 0; j < numItems; ++j) {
            if (dist(gen) > probThreshold && currentWeight + weights[j] <= knapsackCapacity) {
                currentWeight += weights[j];
                totalValue += values[j];
            }
        }

        auto endTime = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(endTime - startTime).count();

        displayResults("Random Selection Method", totalValue, currentWeight, duration, i + 1);
    }
}

int main() {
    vector<string> inputFiles = {"entrada1.txt", "entrada2.txt", "entrada3.txt", "entrada4.txt"};

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

        cout << "--------------------------------------------------" << endl;
    }

    return 0;
}
