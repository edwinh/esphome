#include "esphome/core/log.h"
#include "kaku_dimmable_light.h"
#include "esphome/components/light/color_mode.h"
#include "kaku_new.h"


namespace esphome {
namespace kaku_dimmable_light {

static const char *TAG = "kaku_dimmable_light.light";

void KakuDimmableLightOutput::setup() {
   
}

light::LightTraits KakuDimmableLightOutput::get_traits() {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::BRIGHTNESS});
    return traits;
}

void KakuDimmableLightOutput::write_state(light::LightState *state) {
  // Pin D1 = GPIO5 on the Wemos D1 mini
  int outPin = 5;
  u_int16_t pw = get_pulsewidth();
  u_int16_t adr = get_address();
  u_int16_t unit = get_unit();
  ESP_LOGD(TAG, "Connection to address=%d, unit=%d with pulsewidth=%d", adr, unit, pw);
  KakuNew kaku(adr, pw);

  float brightness = state->current_values.get_brightness();
  ESP_LOGD(TAG, "get_brightness=%.2f", brightness);
  ESP_LOGD(TAG, "is_on=%d", state->current_values.is_on());
  std::vector<int> sequence;
  if (state->current_values.is_on()) {
    int dimlevel = brightness * 15;

    // if dimlevel results in 0, set to 4 - lowest visible
    dimlevel = dimlevel >= 4 ? dimlevel : 4;
    ESP_LOGD(TAG, "dimlevel=%d", dimlevel);
    sequence = kaku.dim(unit,dimlevel);
    ESP_LOGD(TAG, "Light turned on");
  } else {
    sequence = kaku.off(unit);
    ESP_LOGD(TAG, "Light turned off ");
  }

  // Write sequence 
  for (int j=0; j < 4; j++) {
    for (int i=0; i < sequence.size(); i++) {
      bool isHighPulse = sequence[i] >= 0;
      //ESP_LOGD(TAG, "%d", sequence[i]);
      digitalWrite(outPin, isHighPulse);
      delayMicroseconds(isHighPulse ? sequence[i] : -1*sequence[i]);
    }
  }
  state->publish_state();
}

void KakuDimmableLightOutput::dump_config(){
    ESP_LOGCONFIG(TAG, "Kaku Dimmable Light");
}

} //namespace kaku_dimmable_light
} //namespace esphome