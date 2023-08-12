#include <vector>

class KakuNew {
  private:
    const int KAKU_LOW = -1;
    const int KAKU_HIGH = 1;
    const int PULSE_WIDTH = 375;
    int address;
    int periodusec;

    std::vector<int> getBit(bool isBitOne) {
      std::vector<int> result = {};
      if (isBitOne) {
        // Send 1
        result.push_back( KAKU_HIGH * periodusec * 1);
        result.push_back( KAKU_LOW  * periodusec * 5);
        result.push_back( KAKU_HIGH * periodusec * 1);
        result.push_back( KAKU_LOW  * periodusec * 1);

      } else {
        // Send 0
        result.push_back( KAKU_HIGH * periodusec * 1);
        result.push_back( KAKU_LOW  * periodusec * 1);
        result.push_back( KAKU_HIGH * periodusec * 1);
        result.push_back( KAKU_LOW  * periodusec * 5);        

      }
      return result;
    }

    std::vector<int> getStartPulse() {
      std::vector<int> result = {};
      result.push_back( KAKU_HIGH * periodusec * 1);
      result.push_back( KAKU_LOW  * periodusec * 10 + (periodusec >> 1));
      return result;
    }
    
    std::vector<int> getStopPulse() {
      std::vector<int> result = {};
      result.push_back( KAKU_HIGH * periodusec * 1);
      result.push_back( KAKU_LOW  * periodusec * 40);
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

    std::vector<int> switchUnit(int unit, bool switchOn) {
      std::vector<int> result = {};
      std::vector<int> startPulse = getStartPulse();
      std::vector<int> addressSequence = getAddress();
      std::vector<int> falseBit = getBit(false);
      std::vector<int> switchBit = getBit(switchOn);
      std::vector<int> unitSequence = getUnit(unit);
      std::vector<int> stopPulse = getStopPulse();
      
      // Build result sequence
      result.insert(std::end(result), std::begin(startPulse),       std::end(startPulse));
      result.insert(std::end(result), std::begin(addressSequence),  std::end(addressSequence));
      result.insert(std::end(result), std::begin(falseBit),         std::end(falseBit));
      result.insert(std::end(result), std::begin(switchBit),        std::end(switchBit));
      result.insert(std::end(result), std::begin(unitSequence),     std::end(unitSequence));
      result.insert(std::end(result), std::begin(stopPulse),        std::end(stopPulse));
      return result;
    }

  public:
    KakuNew(int address, int periodusec = 260) {
        this->periodusec = periodusec;
        this->address = address;
    }
 
    std::vector<int> on(int unit) {
      return switchUnit(unit, true);
    }
    std::vector<int> off(int unit) {
      return switchUnit(unit, false);
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
      result.push_back( KAKU_HIGH * periodusec * 1);
      result.push_back( KAKU_LOW * periodusec * 1);
      result.push_back( KAKU_HIGH * periodusec * 1);
      result.push_back( KAKU_LOW * periodusec * 1);
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
