#include <vector>
#include <iostream>

/* Module to determine bit sequence for KlikAanKlikUit 

*/
namespace constants {
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