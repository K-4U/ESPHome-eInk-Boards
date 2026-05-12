#pragma once

#include "esphome/core/component.h"
#include "esphome/components/display/display_buffer.h"
#include "esphome/components/font/font.h"
#include "esphome/core/color.h"
#include "esphome/core/time.h"

namespace esphome {
namespace calendar_ns {

class Calendar : public Component {
 public:
  void set_font_header(font::Font *f) { font_header_ = f; }
  void set_font_numbers(font::Font *f) { font_numbers_ = f; }
  void set_color_blk(Color c) { color_blk_ = c; }
  void set_color_red(Color c) { color_red_ = c; }

  void render(esphome::display::Display &it, ESPTime time, int x, int y) {
    int days_padding_bottom = 0;
    int circle_size = 17;
    int cell_size_width = 30;
    int cell_size_height = 30;

    int calendar_y_pos = y;
    int calendar_x_pos = x;

    char cal[7][7][3];
    get_calendar_matrix_(time.year, time.month, cal);

    int days_line_left_position = 0;
    int days_line_right_position = 0;

    for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 7; j++) {
        int x_pos = calendar_x_pos + cell_size_width * j;
        int y_pos = calendar_y_pos + cell_size_height * i;

        if (i == 0) {
          // Header (day names)
          it.printf(x_pos, y_pos, font_header_, color_red_, display::TextAlign::CENTER, "%s", cal[i][j]);
          if (j == 0) {
            days_line_left_position = x_pos - (get_text_width_(&it, font_numbers_, cal[i][j]) / 2);
          } else if (j == 6) {
            days_line_right_position = x_pos + (get_text_width_(&it, font_numbers_, cal[i][j]) / 2);
          }
        } else {
          // Day numbers
          y_pos += days_padding_bottom;
          if (cal[i][j][0] != '\0') {
            int day_num = atoi(cal[i][j]);
            if (day_num == time.day_of_month) {
              // Highlight current day
              if (j > 4) { // Weekend
                it.filled_circle(x_pos, y_pos - 1, circle_size, color_red_);
              } else {
                it.filled_circle(x_pos, y_pos - 1, circle_size, color_blk_);
              }
              it.printf(x_pos, y_pos, font_numbers_, display::COLOR_OFF, display::TextAlign::CENTER, "%s", cal[i][j]);
            } else {
                if (j > 4) { // Weekend
                  it.printf(x_pos, y_pos, font_numbers_, color_red_, display::TextAlign::CENTER, "%s", cal[i][j]);
                } else {
                  it.printf(x_pos, y_pos, font_numbers_, color_blk_, display::TextAlign::CENTER, "%s", cal[i][j]);
                }
            }
          }
        }
      }
      if (i == 1) {
        it.line(days_line_left_position, calendar_y_pos + (cell_size_height / 2), days_line_right_position, calendar_y_pos + (cell_size_height / 2), color_blk_);
      }
    }
  }

  int get_height() {
    int cell_size_height = 30;
    int days_padding_bottom = 0;
    return (6 * cell_size_height) + days_padding_bottom;
  }

 protected:
  font::Font *font_header_{nullptr};
  font::Font *font_numbers_{nullptr};
  Color color_blk_;
  Color color_red_;

  bool is_leap_year_(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
  }

  void get_calendar_matrix_(int year, int month, char cal[7][7][3]) {
    int days_in_month[] = {31, 28 + is_leap_year_(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int num_days = days_in_month[month - 1];
    
    int m = month;
    int y = year;
    if (m < 3) {
      m = m + 12;
      y = y - 1;
    }
    int day_of_week = (1 + (13 * (m + 1)) / 5 + y + y / 4 - y / 100 + y / 400) % 7;
    day_of_week = (day_of_week + 5) % 7;
    
    const char *weekdays[7] = {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"};
    for (int i = 0; i < 7; i++) {
      strcpy(cal[0][i], weekdays[i]);
    }
    
    int current_day = 1 - day_of_week;
    for (int week_num = 1; week_num < 7; week_num++) {
      for (int day_num = 0; day_num < 7; day_num++) {
        if (current_day > 0 && current_day <= num_days) {
          sprintf(cal[week_num][day_num], "%d", current_day);
        } else {
          strcpy(cal[week_num][day_num], "");
        }
        current_day++;
      }
    }
  }

  int get_text_width_(esphome::display::Display *it, font::Font *f, const char *text) {
    if (!f) return 0;
    int x1, y1, width, height;
    it->get_text_bounds(0, 0, text, f, display::TextAlign::TOP_LEFT, &x1, &y1, &width, &height);
    return width;
  }
};

}  // namespace calendar_ns
}  // namespace esphome
