#include "esphome/core/log.h"
#include "kaku_dimmable_light.h"
#include "kaku_new.h"

namespace esphome {
namespace kaku_dimmable_light {

static const char *TAG = "kaku_dimmable_light.light";

void KakuDimmableLightOutput::setup() {
  this->pin_->setup();
  ESP_LOGD(TAG, "Connection to address=%d, unit=%d with pulsewidth=%d", address_, unit_, pulsewidth_);
}

light::LightTraits KakuDimmableLightOutput::get_traits() {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::BRIGHTNESS});
    return traits;
}

void KakuDimmableLightOutput::write_state(light::LightState *state) {
  float brightness = state->current_values.get_brightness();
  ESP_LOGD(TAG, "brightness=%.2f", brightness);
  ESP_LOGD(TAG, "is_on=%d", state->current_values.is_on());
  std::vector<int> sequence;
  KakuNew kaku(this->address_, this->pulsewidth_);
  if (state->current_values.is_on()) {
    int dimlevel = brightness * 15;

    // if dimlevel results in 0, set to 4 - lowest visible
    dimlevel = dimlevel >= 4 ? dimlevel : 4;
    ESP_LOGD(TAG, "dimlevel=%d", dimlevel);
    sequence = kaku.dim(unit_,dimlevel);
    ESP_LOGD(TAG, "Light turned on");
    
  } else {
    sequence = kaku.off(unit_);
    ESP_LOGD(TAG, "Light turned off");
  }
  // Write sequence 
  for (int j=0; j < 3; j++) {
    for (int i=0; i < sequence.size(); i++) {
      bool isHighPulse = sequence[i] >= 0;
      this->pin_->digital_write(isHighPulse);
      delayMicroseconds(isHighPulse ? sequence[i] : -1*sequence[i]);
    }
  }
  state->publish_state();
}

void KakuDimmableLightOutput::dump_config(){
    ESP_LOGCONFIG(TAG, "Kaku Dimmable Light:");
    LOG_PIN("  Pin: ", this->pin_);
    ESP_LOGCONFIG(TAG, "  Pulsewidth: %d", this->pulsewidth_);
    ESP_LOGCONFIG(TAG, "  Address: %d", this->address_);
    ESP_LOGCONFIG(TAG, "  Unit: %d", this->unit_);
}

} //namespace kaku_dimmable_light
} //namespace esphome