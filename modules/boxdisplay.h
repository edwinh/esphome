#include "esphome.h"

const Color textColor = esphome::Color(66, 66, 66);
const Color offColor = esphome::Color(66, 66, 66);
const Color onColor = esphome::Color(255,100,0);

int numPages = 0;
esphome::display::DisplayPage *currentPage;

// Intial setup of screen
void setup(esphome::display::Display& it) {
  const Color bg = esphome::Color(250, 250, 250);
  it.fill(bg);
}

// Header shown in every page
void header(esphome::display::Display& it) {
  it.strftime(250, 10, &esphome::id(font_small), textColor, "%H:%M:%S", esphome::id(time_sntp).now());
  if (esphome::id(muted).state) {
    it.image(280, 0, &esphome::id(mic_mute_icon), ImageAlign::TOP_RIGHT, textColor);
  }
  if (esphome::id(ext_speaker).is_muted()) {
    it.image(320, 0, &esphome::id(mute_icon), ImageAlign::TOP_RIGHT, textColor);
  }
  // if (esphome::id(voice_asst).is_running()) {
  //   it.image(0, 0, &esphome::id(voice_icon), ImageAlign::TOP_LEFT, textColor);
  // }
}

bool handleTouch(bool x, esphome::display::DisplayPage *touchPage) {
  if (esphome::id(backlight_on).value() == false) {
    return false;
  }
  if (currentPage == touchPage) {   // Only respond when on designated page
    return x;
  } else {
    return false;
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
  if (esphome::id(living_room_lights).state == "on") {
    it.image(10, 170, &esphome::id(sofa_icon), onColor);
  } else {
    it.image(10, 170, &esphome::id(sofa_icon), offColor);
  }
  // Bedtime
  int remain_m{0}, remain_s{0};
  bedtimeCounter(remain_m, remain_s);
  if (!(remain_m == 0 && remain_s == 0)) {
    it.printf(135, 155, &esphome::id(font_small), textColor, "%02d:%02d", remain_m, remain_s);
    it.image(130, 170, &esphome::id(sleep_icon), onColor);
  } else {
    it.image(130, 170, &esphome::id(sleep_icon), offColor);
  }
  
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
    it.printf(margin_left + horizontal_spacing, text_vertical_padding+margin_top+(vertical_spacing*line), &esphome::id(font_small), textColor, "%.0f W", esphome::id(current_power).state * 1000);
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

void fifthPage(esphome::display::Display& it) {
  int num = 1;
  int pos_x, pos_y {0};
  for (int r = 0; r < 3; r++) {
    for (int c = 0; c < 3; c++) {
      pos_x = 60+(60*c);
      pos_y = 10+(60*r);
      it.printf(pos_x, pos_y, &esphome::id(font_large_key_pad), textColor, "%d", num++);
    }
  }
  pos_y += 60;
  pos_x = 60;
  it.print(pos_x, pos_y, &esphome::id(font_large_key_pad), textColor, "X");
  it.print(pos_x+60, pos_y, &esphome::id(font_large_key_pad), textColor, "0");
  it.print(pos_x+120, pos_y, &esphome::id(font_large_key_pad), textColor, "V");
};

void displayPage(int pageNumber, esphome::display::Display& it) {
  setup(it);
  header(it);
  if (numPages < pageNumber) {
    numPages = pageNumber;
  }
  switch(pageNumber) {
  case 1:
    currentPage = &esphome::id(page1);
    firstPage(it);
    break;
  case 2:
    currentPage = &esphome::id(page2);
    secondPage(it);
    break;
  case 3:
    currentPage = &esphome::id(page3);
    thirdPage(it);
    break;
  case 4:
    currentPage = &esphome::id(page4);
    fourthPage(it);
    break;
  case 5:
    currentPage = &esphome::id(page5);
    fifthPage(it);
    break;
  default:
    setup(it);
    header(it);
    it.printf(100, 80, &esphome::id(font_small), textColor, "Page %d not defined yet!", pageNumber);
  }
};