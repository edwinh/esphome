
#include "esphome.h"

const Color textColor = esphome::Color(66, 66, 66);
int numPages = 0;

void setup(esphome::display::Display& it) {
  const Color bg = esphome::Color(250, 250, 250);
  it.fill(bg);
}

void header(esphome::display::Display& it) {
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
}

void bedtimeCounter(int &remain_m, int &remain_s) {
  
  if (esphome::id(bedtime_timer_status).state == "active") {
    struct tm tm_finish{}, tm_ha_time{};
    time_t t_helper;
    double remain;
    int remain_h;//, remain_m, remain_s;
    
    strptime(id(bedtime_timer_finish).state.c_str(), "%Y-%m-%dT%T", &tm_finish); // Convert string to time struct
    t_helper = mktime(&tm_finish) + 3600;  // Convert to time_t and add 1 hour (correct for your timezone)

    tm_ha_time = esphome::id(time_sntp).now().to_c_tm(); // Get current time in a tm struct
    
    remain = difftime(t_helper, mktime(&tm_ha_time)); // Calculate difference in seconds
    
    remain_h = (int) remain/3600;
    remain_m = (int) (remain - 3600*remain_h)/60;
    remain_s = (int) remain - 3600*remain_h - 60*remain_m; 

    //it.printf(100, 100, &esphome::id(font_small), textColor, "%02d:%02d", remain_m, remain_s);
    // it.printf(100, 130, &esphome::id(font_small), textColor, "%03d", (int)remain);

  } else {
    remain_m = remain_s = 0;
  }
};

void firstPage(esphome::display::Display& it) {
  // Living room
  it.image(10, 170, &esphome::id(sofa_icon), textColor);

  // Bedtime
  int remain_m{0}, remain_s{0};
  bedtimeCounter(remain_m, remain_s);
  if (!(remain_m == 0 && remain_s == 0)) {
    it.printf(135, 155, &esphome::id(font_small), textColor, "%02d:%02d", remain_m, remain_s);
  }
  it.image(130, 170, &esphome::id(sleep_icon), textColor);
  
  // Spare
  it.filled_rectangle(250, 170, 60, 60, Color(0,0,255));
  auto vertical_spacing {25};
  auto horizontal_spacing {35};
  auto text_vertical_padding {3};
  auto margin_left {5};
  auto margin_top {5};
  auto line {0};
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
};

void secondPage(esphome::display::Display& it) {
  it.print(100, 80, &esphome::id(font_small), textColor, "Connect to my WiFi!");
  it.qr_code(100, 100, &esphome::id(qr_wifi), textColor, 4);
};

void fourthPage(esphome::display::Display& it) {
  it.printf(100, 80, &esphome::id(font_small), textColor, "Timer: %s", esphome::id(bedtime_timer_status).state.c_str());

  if (esphome::id(bedtime_timer_status).state == "active") {
    struct tm tm_finish{}, tm_ha_time{};
    time_t t_helper;
    double remain;
    int remain_h, remain_m, remain_s;
    
    strptime(id(bedtime_timer_finish).state.c_str(), "%Y-%m-%dT%T", &tm_finish); // Convert string to time struct
    t_helper = mktime(&tm_finish) + 3600;  // Convert to time_t and add 1 hour (correct for your timezone)

    tm_ha_time = esphome::id(time_sntp).now().to_c_tm(); // Get current time in a tm struct
    
    remain = difftime(t_helper, mktime(&tm_ha_time)); // Calculate difference in seconds
    
    remain_h = (int) remain/3600;
    remain_m = (int) (remain - 3600*remain_h)/60;
    remain_s = (int) remain - 3600*remain_h - 60*remain_m; 

    it.printf(100, 100, &esphome::id(font_small), textColor, "%02d:%02d", remain_m, remain_s);
  } else {
    it.printf(0, 30, &esphome::id(font_small), " "); // clear second line
  }
};
void thirdPage(esphome::display::Display& it) {
  it.print(60, 80, &esphome::id(font_medium), textColor, "Kusje voor Yoni");
  it.image(100, 120, &esphome::id(heart_icon), esphome::Color(255, 0, 0));
};

void displayPage(int pageNumber, esphome::display::Display& it) {
  setup(it);
  header(it);
  switch(pageNumber) {
  case 1:
    firstPage(it);
    break;
  case 2:
    secondPage(it);
    break;
  case 3:
    thirdPage(it);
    break;
  case 4:
    fourthPage(it);
    break;
  default:
    setup(it);
    header(it);
    it.printf(100, 80, &esphome::id(font_small), textColor, "Page %d not defined yet!", pageNumber);
  }
};