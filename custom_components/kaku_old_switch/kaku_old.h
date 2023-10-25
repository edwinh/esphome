#include <vector>

const u_int16_t PULSE_WIDTH {375};

class KakuOld {
  private:
    u_int16_t periodusec;
    const int KAKU_LOW {-1};
    const int KAKU_HIGH {1};

  protected:
    u_int16_t address_;
    
  public:
    KakuOld(u_int16_t address, u_int16_t periodusec = PULSE_WIDTH) {
        this->periodusec = periodusec;
        this->address_ = address;
    }
        
    std::vector<int> on(u_int16_t device) {
        return buildSequence(device, true);
    }

    std::vector<int> off(u_int16_t device) {
        return buildSequence(device, false);
    }

    std::vector<int> buildSequence(u_int16_t device, bool on) {
      int trits[12];
      std::vector<int> result = {};
      getTelegram(this->address_, device, on, trits);
      int periodusec = this->periodusec;

      for (int i {0}; i < 12; i++) {
        int code = trits[i];
        switch (code) {
          case 0:
            result.push_back( KAKU_HIGH * periodusec * 1);
            result.push_back( KAKU_LOW  * periodusec * 3);
            result.push_back( KAKU_HIGH * periodusec * 1);
            result.push_back( KAKU_LOW  * periodusec * 3);
            break;
          case 1:
            result.push_back( KAKU_HIGH * periodusec * 3);
            result.push_back( KAKU_LOW  * periodusec * 1);
            result.push_back( KAKU_HIGH * periodusec * 3);
            result.push_back( KAKU_LOW  * periodusec * 1);
            break;
          case 2:
            result.push_back( KAKU_HIGH * periodusec * 1);
            result.push_back( KAKU_LOW  * periodusec * 3);
            result.push_back( KAKU_HIGH * periodusec * 3);
            result.push_back( KAKU_LOW  * periodusec * 1);
            break;
          }
        }

      result.push_back( 1 * periodusec * 1);
      result.push_back(-1 * periodusec * 31);
      return result;
    }

    void getTelegram(u_int16_t _address, u_int16_t _device, bool on, int trits[]) {
      int address = _address;
      u_int16_t device = _device - 1;

      for (int i {0}; i < 4; i++) {
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
