#include <vector>
#include <iostream>

/* Module to determine bit sequence for KlikAanKlikUit 

*/
namespace constants {
  const std::vector<std::vector<int>> TELEGRAMS         = {{375, -1125, 375, -1125}, {}, {375, -1125, 1125, -375}};
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
    result.insert(result.end(), constants::TELEGRAMS[element].begin(), constants::TELEGRAMS[element].end());
  }
  result.insert(result.end(), constants::END_SEQUENCE.begin(), constants::END_SEQUENCE.end());
  return result;
}

class KlikAanKlikUitOld {
  private:
    int periodusec;
      
  public:
    KlikAanKlikUitOld(int periodusec = 375) {
        this->periodusec = periodusec;
    }
        
    std::vector<int> on(char address, int device) {
        return buildSequence(address, device, true);
    }

    std::vector<int> off(char address, int device) {
        return buildSequence(address, device, false);
    }

    std::vector<int> buildSequence(char address, int device, bool on) {
      int trits[12];
      std::vector<int> result = {};
      getTelegram(address, device, on, trits);
      int periodusec = this->periodusec;

      for (int i = 0; i < 12; i++) {
        int code = trits[i];
        switch (code) {
          case 0:
            result.push_back( 1 * periodusec * 1);
            result.push_back(-1 * periodusec * 3);
            result.push_back( 1 * periodusec * 1);
            result.push_back(-1 * periodusec * 3);
            break;
          case 1:
            result.push_back( 1 * periodusec * 3);
            result.push_back(-1 * periodusec * 1);
            result.push_back( 1 * periodusec * 3);
            result.push_back(-1 * periodusec * 1);
            break;
          case 2:
            result.push_back( 1 * periodusec * 1);
            result.push_back(-1 * periodusec * 3);
            result.push_back( 1 * periodusec * 3);
            result.push_back(-1 * periodusec * 1);
            break;
          }
        }

      result.push_back( 1 * periodusec * 1);
      result.push_back(-1 * periodusec * 31);
      return result;
    }

    void getTelegram(char _address, int _device, bool on, int trits[]) {
      int address = _address - 'A';
      int device = _device - 1;

      for (int i = 0; i < 4; i++) {
        trits[i] = (address & 1) ? 2 : 0;
        address >>= 1;

        trits[i + 4] = (device & 1) ? 2 : 0;
        device >>= 1;
      }
      trits[8] = 0;
      trits[9] = 2;
      trits[10] = 2;
      trits[11] = on ? 2 : 0;
    }
};