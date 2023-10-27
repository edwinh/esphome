
#include "esphome.h"

const Color textColor = esphome::Color(66, 66, 66);

void setup(esphome::display::Display& it) {
  const Color bg = esphome::Color(250, 250, 250);
  it.fill(bg);
}

void header(esphome::display::Display& it) {
  it.strftime(250, 10, &esphome::id(font_small), textColor, "%H:%M:%S", esphome::id(time_sntp).now());
}


void firstPage(esphome::display::Display& it) {
  setup(it);
  header(it);
  it.image(10, 170, &esphome::id(sofa_icon), textColor);
  it.image(130, 170, &esphome::id(sleep_icon), textColor);
  it.filled_rectangle(250, 170, 60, 60, Color(0,0,255));
  auto vertical_spacing = 25;
  auto horizontal_spacing = 35;
  auto text_vertical_padding = 3;
  auto margin_left = 5;
  auto margin_top = 5;
  auto line = 0;
  if (!isnan(esphome::id(weather_forecast_temperature).state)) {
    it.image(margin_left, margin_top+(vertical_spacing*line), &esphome::id(tree_icon_small), textColor);
    it.printf(margin_left + horizontal_spacing, text_vertical_padding+margin_top+(vertical_spacing*line), &esphome::id(font_small), textColor, "%.1f °C", esphome::id(weather_forecast_temperature).state);
  }
  line = 1;
  if (!isnan(esphome::id(living_room_temperature).state)) {
    it.image(margin_left, margin_top+(vertical_spacing*line), &esphome::id(home_icon_small), textColor);
    it.printf(margin_left + horizontal_spacing, text_vertical_padding+margin_top+(vertical_spacing*line), &esphome::id(font_small), textColor, "%.1f°C", esphome::id(living_room_temperature).state);
  }
  line = 2;
  if (!isnan(esphome::id(current_power).state)) {
    it.image(margin_left, margin_top+(vertical_spacing*line), &esphome::id(flash_icon_small), textColor);
    it.printf(margin_left + horizontal_spacing, text_vertical_padding+margin_top+(vertical_spacing*line), &esphome::id(font_small), textColor, "%.3f kW", esphome::id(current_power).state);
  }
  it.strftime(250, 10, &esphome::id(font_small), textColor, "%H:%M:%S", esphome::id(time_sntp).now());
  
  if (esphome::id(muted).state) {
    it.image(280, 0, &esphome::id(mic_mute_icon), ImageAlign::TOP_RIGHT, textColor);
  }
  if (esphome::id(ext_speaker).is_muted()) {
    it.image(320, 0, &esphome::id(mute_icon), ImageAlign::TOP_RIGHT, textColor);
  }
  if (esphome::id(voice_asst).is_running()) {
    it.image(0, 0, &esphome::id(voice_icon), ImageAlign::TOP_LEFT, textColor);
  }
};

void thirdPage(esphome::display::Display& it) {
  setup(it);
  header(it);
  auto icon = esphome::id(mute_icon);
  it.image(100, 80, &icon, textColor);
};