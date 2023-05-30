#include <vector>

/* Module to determine bit sequence for KlikAanKlikUit 

*/
namespace constants {
  const int KAKU_LOW = -1;
  const int KAKU_HIGH = 1;
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
            result.push_back( constants::KAKU_HIGH * periodusec * 1);
            result.push_back( constants::KAKU_LOW * periodusec * 3);
            result.push_back( constants::KAKU_HIGH * periodusec * 1);
            result.push_back( constants::KAKU_LOW * periodusec * 3);
            break;
          case 1:
            result.push_back( constants::KAKU_HIGH * periodusec * 3);
            result.push_back( constants::KAKU_LOW * periodusec * 1);
            result.push_back( constants::KAKU_HIGH * periodusec * 3);
            result.push_back( constants::KAKU_LOW * periodusec * 1);
            break;
          case 2:
            result.push_back( constants::KAKU_HIGH * periodusec * 1);
            result.push_back( constants::KAKU_LOW * periodusec * 3);
            result.push_back( constants::KAKU_HIGH * periodusec * 3);
            result.push_back( constants::KAKU_LOW * periodusec * 1);
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
class KlikAanKlikUitNew {
  private:
    int address;
    int periodusec;

  public:
    KlikAanKlikUitNew(int address, int periodusec = 260) {
        this->periodusec = periodusec;
        this->address = address;
    }
    
    std::vector<int> getBit(bool isBitOne) {
      std::vector<int> result = {};
      if (isBitOne) {
        // Send 1
        result.push_back( constants::KAKU_HIGH * periodusec * 1);
        result.push_back( constants::KAKU_LOW  * periodusec * 5);
        result.push_back( constants::KAKU_HIGH * periodusec * 1);
        result.push_back( constants::KAKU_LOW  * periodusec * 1);

      } else {
        // Send 0
        result.push_back( constants::KAKU_HIGH * periodusec * 1);
        result.push_back( constants::KAKU_LOW  * periodusec * 1);
        result.push_back( constants::KAKU_HIGH * periodusec * 1);
        result.push_back( constants::KAKU_LOW  * periodusec * 5);        

      }
      return result;
    }

    std::vector<int> getStartPulse() {
      std::vector<int> result = {};
      result.push_back( constants::KAKU_HIGH * periodusec * 1);
      result.push_back( constants::KAKU_LOW  * periodusec * 10 + (periodusec >> 1));
      return result;
    }
    
    std::vector<int> getStopPulse() {
      std::vector<int> result = {};
      result.push_back( constants::KAKU_HIGH * periodusec * 1);
      result.push_back( constants::KAKU_LOW  * periodusec * 40);
      return result;
    }

    std::vector<int> getAddress() {
      std::vector<int> result = {};
      for (int i = 25; i >= 0; i--) {
        std::vector<int> bit = getBit((address >> i) & 1);
        result.insert(std::end(result), std::begin(bit), std::end(bit));
      }
      return result;
    }
    
    std::vector<int> getUnit(int unit) {
      std::vector<int> result = {};
      for (int i = 3; i >= 0; i--) {
        std::vector<int> bit = getBit((unit & 1 << i));
        result.insert(std::end(result), std::begin(bit), std::end(bit));
      }
      return result;
    }
    std::vector<int> on(int unit) {
      return switchUnit(unit, true);
    }
    std::vector<int> off(int unit) {
      return switchUnit(unit, false);
    }



    std::vector<int> switchUnit(int unit, bool switchOn) {
      std::vector<int> result = {};
      std::vector<int> startPulse = getStartPulse();
      std::vector<int> addressSequence = getAddress();
      std::vector<int> falseBit = getBit(false);
      std::vector<int> switchBit = getBit(switchOn);
      std::vector<int> unitSequence = getUnit(unit);
      std::vector<int> stopPulse = getStopPulse();
      
      // Build result sequence
      result.insert(std::end(result), std::begin(startPulse), std::end(startPulse));
      result.insert(std::end(result), std::begin(addressSequence), std::end(addressSequence));
      result.insert(std::end(result), std::begin(falseBit), std::end(falseBit));
      result.insert(std::end(result), std::begin(switchBit), std::end(switchBit));
      result.insert(std::end(result), std::begin(unitSequence), std::end(unitSequence));
      result.insert(std::end(result), std::begin(stopPulse), std::end(stopPulse));
      return result;
    }
    std::vector<int> dim(int unit, int dimLevel) {
      // dimLevel can be 0 to 15
      std::vector<int> result = {};
      std::vector<int> startPulse = getStartPulse();
      std::vector<int> addressSequence = getAddress();
      std::vector<int> falseBit = getBit(false);
      std::vector<int> unitSequence = getUnit(unit);
      std::vector<int> stopPulse = getStopPulse();
      
      // Build result sequence
      result.insert(std::end(result), std::begin(startPulse), std::end(startPulse));
      result.insert(std::end(result), std::begin(addressSequence), std::end(addressSequence));
      result.insert(std::end(result), std::begin(falseBit), std::end(falseBit));
      result.push_back( constants::KAKU_HIGH * periodusec * 1);
      result.push_back( constants::KAKU_LOW * periodusec * 1);
      result.push_back( constants::KAKU_HIGH * periodusec * 1);
      result.push_back( constants::KAKU_LOW * periodusec * 1);
      result.insert(std::end(result), std::begin(unitSequence), std::end(unitSequence));

      // Set dimlevel
      std::vector<int> dimlevelSequence = {};
      std::vector<int> dimpart;
      for (int i = 3; i >=0; i--) {
        dimpart = getBit(dimLevel & 1 << i);
        dimlevelSequence.insert(std::end(dimlevelSequence), std::begin(dimpart), std::end(dimpart));
      }
      result.insert(std::end(result), std::begin(dimlevelSequence), std::end(dimlevelSequence));
      result.insert(std::end(result), std::begin(stopPulse), std::end(stopPulse));
      return result;
    }
};