#pragma once

#include "esphome/core/component.h"
// #include "esphome/components/output/float_output.h"
#include "esphome/components/light/light_output.h"

namespace esphome {
namespace kaku_dimmable_light {

class KakuDimmableLightOutput : public light::LightOutput, public Component {
 public:
  void setup() override;
  light::LightTraits get_traits() override;
  
  void set_pin(InternalGPIOPin *pin) { pin_ = pin; }
  void set_pulsewidth(uint16_t pulsewidth) { this->pulsewidth_ = pulsewidth; }
  void set_address(uint16_t address) { this->address_ = address; }
  void set_unit(uint16_t unit) { this->unit_ = unit; }

  uint16_t get_pulsewidth() {return this->pulsewidth_; }
  uint16_t get_address() {return this->address_; }
  uint16_t get_unit() {return this->unit_; }

  void write_state(light::LightState *state) override;
  void dump_config() override;
 
 protected:
  // output::FloatOutput *output_;
  uint16_t pulsewidth_{260};
  uint16_t address_{1013};
  uint16_t unit_{2};
  InternalGPIOPin *pin_;

};

} //namespace kaku_dimmable_light
} //namespace esphome