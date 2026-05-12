#pragma once

#include "esphome/core/component.h"
#include "esphome/components/display/display_buffer.h"
#include "esphome/components/font/font.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/color.h"
#include <string>
#include <map>
#include <cmath>

namespace esphome {
namespace weather_ns {

class Weather : public Component {
 public:
  void set_font_icon(font::Font *f) { font_icon_ = f; }
  void set_font_temp(font::Font *f) { font_temp_ = f; }
  void set_font_mdi_small(font::Font *f) { font_mdi_small_ = f; }
  void set_color_blk(Color c) { color_blk_ = c; }
  void set_color_red(Color c) { color_red_ = c; }

  void set_sensor_temp(sensor::Sensor *s) { sensor_temp_ = s; }
  void set_sensor_temp_min(sensor::Sensor *s) { sensor_temp_min_ = s; }
  void set_sensor_temp_max(sensor::Sensor *s) { sensor_temp_max_ = s; }
  void set_sensor_condition(text_sensor::TextSensor *s) { sensor_condition_ = s; }

  void render(esphome::display::Display &it, int x, int y, int width, int height) {
    int weather_x_pos = x;
    int weather_y_pos = y;

    std::string condition = "";
    if (sensor_condition_ && sensor_condition_->has_state()) {
      condition = sensor_condition_->state;
    }

    float temp = NAN;
    if (sensor_temp_ && sensor_temp_->has_state()) {
      temp = sensor_temp_->state;
    }

    float temp_min = NAN;
    if (sensor_temp_min_ && sensor_temp_min_->has_state()) {
      temp_min = sensor_temp_min_->state;
    }

    float temp_max = NAN;
    if (sensor_temp_max_ && sensor_temp_max_->has_state()) {
      temp_max = sensor_temp_max_->state;
    }

    // Vertical line before weather
    it.vertical_line(weather_x_pos, weather_y_pos - 10, height + 20, color_red_);
    
    int middle_of_box_x = weather_x_pos + (width / 2);
    int weather_icon_height = 130;

    // CURRENT WEATHER ICON
    if (font_icon_) {
      it.printf(middle_of_box_x, weather_y_pos + 20, font_icon_, color_blk_, display::TextAlign::TOP_CENTER, "%s", get_weather_icon_(condition));
    }
    
    // TEMPERATURES
    if (font_mdi_small_ && font_temp_) {
      // MIN TEMP
      if (!std::isnan(temp_min)) {
        Color min_color = get_temp_color_(temp_min);
        it.printf(weather_x_pos + 10, weather_y_pos + 20 + weather_icon_height, font_mdi_small_, min_color, display::TextAlign::TOP_LEFT, "\U0000f37a");
        it.printf(weather_x_pos + 10 + 22, weather_y_pos + 20 + weather_icon_height, font_temp_, min_color, display::TextAlign::TOP_LEFT, "%2.0f°", temp_min);
      }
      
      // CURRENT TEMP
      if (!std::isnan(temp)) {
        Color curr_color = get_temp_color_(temp);
        it.printf(middle_of_box_x, weather_y_pos + 20 + weather_icon_height, font_temp_, curr_color, display::TextAlign::TOP_CENTER, "%2.0f°", temp);
      }
      
      // MAX TEMP
      if (!std::isnan(temp_max)) {
        Color max_color = get_temp_color_(temp_max);
        it.printf(weather_x_pos + width - 10, weather_y_pos + 20 + weather_icon_height, font_mdi_small_, max_color, display::TextAlign::TOP_RIGHT, "\U0000f379");
        it.printf(weather_x_pos + width - 10 - 22, weather_y_pos + 20 + weather_icon_height, font_temp_, max_color, display::TextAlign::TOP_RIGHT, "%2.0f°", temp_max);
      }
    }
  }

 protected:
  font::Font *font_icon_{nullptr};
  font::Font *font_temp_{nullptr};
  font::Font *font_mdi_small_{nullptr};
  Color color_blk_;
  Color color_red_;

  sensor::Sensor *sensor_temp_{nullptr};
  sensor::Sensor *sensor_temp_min_{nullptr};
  sensor::Sensor *sensor_temp_max_{nullptr};
  text_sensor::TextSensor *sensor_condition_{nullptr};

  const char* get_weather_icon_(const std::string& condition) {
    static const std::map<std::string, const char*> weather_icon_map = {
        {"cloudy", "\U000F0590"},
        {"cloudy-alert", "\U000F0F2F"},
        {"cloudy-arrow-right", "\U000F0E6E"},
        {"clear-night", "\U000F0594"},
        {"fog", "\U000F0591"},
        {"hail", "\U000F0592"},
        {"hazy", "\U000F0F30"},
        {"hurricane", "\U000F0898"},
        {"lightning", "\U000F0593"},
        {"lightning-rainy", "\U000F067E"},
        {"night", "\U000F0594"},
        {"night-partly-cloudy", "\U000F0F31"},
        {"partlycloudy", "\U000F0595"},
        {"partly-lightning", "\U000F0F32"},
        {"partly-rainy", "\U000F0F33"},
        {"partly-snowy", "\U000F0F34"},
        {"partly-snowy-rainy", "\U000F0F35"},
        {"pouring", "\U000F0596"},
        {"rainy", "\U000F0597"},
        {"snowy", "\U000F0598"},
        {"snowy-heavy", "\U000F0F36"},
        {"snowy-rainy", "\U000F067F"},
        {"sunny", "\U000F0599"},
        {"sunny-alert", "\U000F0F37"},
        {"sunny-off", "\U000F14E4"},
        {"sunset", "\U000F059A"},
        {"sunset-down", "\U000F059B"},
        {"sunset-up", "\U000F059C"},
        {"tornado", "\U000F0F38"},
        {"windy", "\U000F059D"},
        {"windy-variant", "\U000F059E"},
    };
    auto it = weather_icon_map.find(condition);
    return (it != weather_icon_map.end()) ? it->second : "";
  }

  Color get_temp_color_(float temp) {
    return (temp >= 25.0f) ? color_red_ : color_blk_;
  }
};

}  // namespace weather_ns
}  // namespace esphome
