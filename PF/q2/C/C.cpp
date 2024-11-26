#include <vector>
#include <iostream>
#include <omp.h>

void processImage(std::vector<std::vector<int>>& image) {
    #pragma omp parallel for collapse(2)
    for (size_t i = 0; i < image.size(); ++i) {
        for (size_t j = 0; j < image[i].size(); ++j) {
            image[i][j] = (image[i][j] * 2) % 256;
        }
    }
}

int main() {
    std::vector<std::vector<int>> img{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    processImage(img);
    #pragma omp critical
    for (auto& row : img) {
        for (auto& col : row) {
            std::cout << col << " ";
        }
    std::cout << std::endl;
    }

    return 0;
}