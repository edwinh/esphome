#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace kaku_old_switch {
  
static const char *TAG = "kaku_old_switch.switch";

class KakuOldSwitch : public switch_::Switch, public Component {
 public:
  void setup() override;
  void set_address(uint16_t address) {
    this->address_ = address;
    ESP_LOGD(TAG, "Address: %d", address);

    }
  void set_unit(uint16_t unit) {this->unit_ = unit;}
  void set_pulsewidth(uint16_t pulsewidth) {this->pulsewidth_ = pulsewidth;}
  uint16_t get_pulsewidth() {return this->pulsewidth_; }
  char get_address() {return this->address_; }
  uint16_t get_unit() {return this->unit_; }
  void write_state(bool state) override;
  void dump_config() override;

protected:
  uint16_t pulsewidth_{375}; // 375 us
  uint16_t unit_{2};
  uint16_t address_{3}; //D

};

} //namespace kaku_old_switch
} //namespace esphome