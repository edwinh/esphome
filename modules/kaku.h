#include <vector>

namespace constants {
  const std::vector<std::vector<int>> TELEGRAMS_ARRAY   = {{375, -1125, 375, -1125}, {}, {375, -1125, 1125, -375}};
  const std::vector<int>              END_SEQUENCE      = {375, -11625};
  const std::vector<int>              D1_ON_SEQUENCE    = {2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2};
  const std::vector<int>              D1_OFF_SEQUENCE   = {2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0};
  const std::vector<int>              D2_ON_SEQUENCE    = {2, 2, 0, 0, 2, 0, 0, 0, 0, 2, 2, 2};
  const std::vector<int>              D2_OFF_SEQUENCE   = {2, 2, 0, 0, 2, 0, 0, 0, 0, 2, 2, 0};
  const std::vector<int>              D3_ON_SEQUENCE    = {2, 2, 0, 0, 0, 2, 0, 0, 0, 2, 2, 2};
  const std::vector<int>              D3_OFF_SEQUENCE   = {2, 2, 0, 0, 0, 2, 0, 0, 0, 2, 2, 0};
}

std::vector<int> kakuOldProtocol(const char* address, bool on) {
  std::vector<int> kaku_sequence = constants::D3_ON_SEQUENCE;
  //std::vector<std::vector<int>> telegramsArray = constants::TELEGRAMS_ARRAY;
  std::vector<int> result = {};
  for (const auto& element : kaku_sequence) {
    result.insert(result.end(), constants::TELEGRAMS_ARRAY[element].begin(), constants::TELEGRAMS_ARRAY[element].end());
  }
  result.insert(result.end(), constants::END_SEQUENCE.begin(), constants::END_SEQUENCE.end());
  return result;
}