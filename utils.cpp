#include "utils.h"

int find_max_pips(const std::vector<std::vector<int>> &board) {
    int maxPips = 0;
    for (const auto &row: board) {
        int rowMax = *std::max_element(row.begin(), row.end());
        maxPips = std::max(maxPips, rowMax);
    }
    return maxPips;
}
