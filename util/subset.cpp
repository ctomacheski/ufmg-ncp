#include <vector>
#include <cmath>

#include "subset.h"

bool subsetSum(std::vector<int> input, int target) {
  int lb = 0;
  int ub = 0;
  for (int i = 0; i < input.size(); i++) {
    if (input[i] > 0) {
      ub += input[i];
    } else {
      lb += input[i];
    }
  }

  int offset = std::abs(lb);
  int maxSum = std::abs(ub) + offset;

  std::vector<std::vector<bool> > dp(input.size() + 1, std::vector<bool>(maxSum + 1, false));
  for (int i = 1; i < input.size() + 1; i++) {
    dp[i][input[i-1] + offset] = true;
  }

  for (int i = 1; i <= input.size(); i++) {
    for (int sum = 0; sum < maxSum + 1; sum++) {
      if (!dp[i][sum]) {
        int index = sum - input[i-1];
        bool validIndex = index >= 0 && index <= maxSum;

        dp[i][sum] = dp[i-1][sum] || (validIndex ? dp[i-1][index] : false);
      }
    }
  }

  int index = offset + target;
  bool validIndex = index >= 0 && index <= maxSum;
  return validIndex ? dp[input.size()][offset + target] : false;
}
