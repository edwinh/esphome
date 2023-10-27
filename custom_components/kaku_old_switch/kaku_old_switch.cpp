#include "esphome/core/log.h"
#include "kaku_old_switch.h"
#include "kaku_old.h"

namespace esphome {
namespace kaku_old_switch {

void KakuOldSwitch::setup() {
  // TODO: Fix pin
  this->pin_->setup();
}

void KakuOldSwitch::write_state(bool state) {
  u_int16_t pw = get_pulsewidth();
  u_int16_t adr = get_address();
  u_int16_t unit = get_unit();
  ESP_LOGD(TAG, "Connection to address=%d, unit=%d, with pulsewidth=%d", adr, unit, pw);
  KakuOld kaku = KakuOld(adr, pw);

  ESP_LOGD(TAG, "is_on=%d", state);
  std::vector<int> sequence;
  if (state) {
    sequence = kaku.on(unit);
    ESP_LOGD(TAG, "Light turned on");
  } else {
    sequence = kaku.off(unit);
    ESP_LOGD(TAG, "Light turned off ");
  }
  // Write sequence 4x
  for (int j=0; j < 4; j++) {
    for (int i=0; i < sequence.size(); i++) {
      bool isHighPulse = sequence[i] >= 0;
      // ESP_LOGCONFIG(TAG, "Loop: %d, %d", j, sequence[i]);
      this->pin_->digital_write(isHighPulse);
      delayMicroseconds(isHighPulse ? sequence[i] : -1*sequence[i]);
    }
  }
  publish_state(state);
}

void KakuOldSwitch::dump_config(){
    ESP_LOGCONFIG(TAG, "KlikAanKlikUit Old switch");
    LOG_PIN("  Pin: ", this->pin_);
    ESP_LOGCONFIG(TAG, "  Pulsewidth: %d", this->pulsewidth_);
    ESP_LOGCONFIG(TAG, "  Address: %d", this->address_);
    ESP_LOGCONFIG(TAG, "  Unit: %d", this->unit_);
}

} //namespace kaku_old_switch
} //namespace esphome