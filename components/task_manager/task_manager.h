#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include "esphome/core/component.h"
#include "esphome/components/json/json_util.h"
#include "esphome/components/display/display.h"
#include "esphome/components/display/display_buffer.h"
#include "esphome/components/font/font.h"
#include "esphome/core/color.h"
#include <vector>
#include <string>
#include <map>

// We'll include text_utilities.h later in the YAML or here if possible.
// To be safe, we'll assume it's available or we'll define what we need.
#include "text_utilities.h"

namespace esphome {
namespace task_manager_ns {

struct Task {
    std::string category;
    std::string event;
    std::string date;
    std::string time;
    std::string category_icon;
};

class TaskManager : public Component {
 public:
    std::vector<Task> today;
    std::vector<Task> upcoming;

    void clear_today() { this->today.clear(); }
    void clear_upcoming() { this->upcoming.clear(); }

    void add_today(const std::string& event, const std::string& time) {
        this->today.push_back(this->parse_task_(event, "", time));
    }

    void add_upcoming(const std::string& event, const std::string& date, const std::string& time) {
        this->upcoming.push_back(this->parse_task_(event, date, time));
    }

    void parse_today_json(const std::string& json_data) {
        this->today.clear();
        auto doc = json::parse_json(json_data);
        if (doc.is<JsonArray>()) {
            for (JsonVariant elem : doc.as<JsonArray>()) {
                std::string event = elem["event"] | "";
                if (!event.empty()) {
                    this->add_today(event, elem["start_time"] | "");
                }
            }
        }
    }

    void parse_upcoming_json(const std::string& json_data) {
        this->upcoming.clear();
        auto doc = json::parse_json(json_data);
        if (doc.is<JsonArray>()) {
            for (JsonVariant elem : doc.as<JsonArray>()) {
                std::string event = elem["event"] | "";
                if (!event.empty()) {
                    this->add_upcoming(event, elem["date"] | "", elem["start_time"] | "");
                }
            }
        }
    }

    font::Font *font_title{nullptr};
    font::Font *font_mdi_22{nullptr};
    font::Font *font_today_task{nullptr};
    font::Font *font_today_time{nullptr};
    font::Font *font_upcoming_date{nullptr};
    font::Font *font_upcoming_task{nullptr};

    Color color_blk;
    Color color_red;

    void set_font_title(font::Font *f) { font_title = f; }
    void set_font_mdi_22(font::Font *f) { font_mdi_22 = f; }
    void set_font_today_task(font::Font *f) { font_today_task = f; }
    void set_font_today_time(font::Font *f) { font_today_time = f; }
    void set_font_upcoming_date(font::Font *f) { font_upcoming_date = f; }
    void set_font_upcoming_task(font::Font *f) { font_upcoming_task = f; }

    void set_color_blk(Color c) { color_blk = c; }
    void set_color_red(Color c) { color_red = c; }

    bool layout_done_{false};
    int spacing_title_{0};
    int spacing_today_{0};
    int spacing_upcoming_date_{0};
    int spacing_upcoming_item_{0};

    void setup_layout(esphome::display::Display &it) {
        if (this->layout_done_) return;
        if (this->font_title) this->spacing_title_ = get_text_height(&it, this->font_title, "%s", "TODAY");
        if (this->font_today_task) this->spacing_today_ = get_text_height(&it, this->font_today_task, "%s", "Sample Task Text") + 2;
        if (this->font_upcoming_date) this->spacing_upcoming_date_ = get_text_height(&it, this->font_upcoming_date, "%s", "Sunday August 25:") + 2;
        if (this->font_upcoming_task) this->spacing_upcoming_item_ = get_text_height(&it, this->font_upcoming_task, "%s", "Sample Upcoming Task") + 2;
        this->layout_done_ = true;
    }

    int get_spacing_title() { return this->spacing_title_; }
    int get_spacing_today() { return this->spacing_today_; }
    int get_spacing_upcoming_date() { return this->spacing_upcoming_date_; }
    int get_spacing_upcoming_item() { return this->spacing_upcoming_item_; }

    void render_today(esphome::display::Display &it, int x, int y, int maxX, int maxY) {
        this->setup_layout(it);
        if (!font_title || !font_today_task || !font_today_time || !font_mdi_22) return;

        it.printf(x + (maxX - x) / 2, y, font_title, color_red, display::TextAlign::TOP_CENTER, "TODAY");
        
        int row = y + spacing_title_ + 3;

        if (this->today.empty()) {
            it.printf(x, row, font_today_task, color_blk, display::TextAlign::TOP_LEFT, "Nothing today!");
        } else {
            for (const auto& task : this->today) {
                if (row + spacing_today_ > maxY) break;
                it.printf(x, row + 2, font_mdi_22, color_blk, display::TextAlign::TOP_LEFT, "%s", task.category_icon.c_str());
                it.printf(x + 25, row, font_today_task, color_blk, display::TextAlign::TOP_LEFT, "%s", task.event.c_str());
                it.printf(maxX, row + 4, font_today_time, color_red, display::TextAlign::TOP_RIGHT, "%s", task.time.c_str());
                row += spacing_today_;
            }
        }
    }

    void render_upcoming(esphome::display::Display &it, int x, int y, int maxX, int maxY) {
        this->setup_layout(it);
        if (!font_title || !font_upcoming_task || !font_upcoming_date || !font_mdi_22 || !font_today_time) return;

        it.printf(x + (maxX - x) / 2, y, font_title, color_red, display::TextAlign::TOP_CENTER, "UPCOMING");
        
        int row = y + spacing_title_ + 3;

        for (size_t i = 0; i < this->upcoming.size(); i++) {
            const auto& task = this->upcoming[i];
            
            bool is_new_date = (i == 0 || task.date != this->upcoming[i-1].date);
            int needed_height = is_new_date ? (spacing_upcoming_date_ + spacing_upcoming_item_) : spacing_upcoming_item_;

            if (row + needed_height > maxY) break;

            if (is_new_date) {
                it.printf(x, row, font_upcoming_date, color_red, display::TextAlign::TOP_LEFT, "%s", (task.date + ":").c_str());
                row += spacing_upcoming_date_;
            }

            it.printf(x + 5, row - 1, font_mdi_22, color_blk, display::TextAlign::TOP_LEFT, "%s", task.category_icon.c_str());
            it.printf(x + 30, row, font_upcoming_task, color_blk, display::TextAlign::TOP_LEFT, "%s", task.event.c_str());
            it.printf(maxX, row, font_today_time, color_red, display::TextAlign::TOP_RIGHT, "%s", task.time.c_str());
            row += spacing_upcoming_item_;
        }
    }


 protected:
    Task parse_task_(const std::string& full_text, const std::string& date, const std::string& time) {
        std::string category = "";
        std::string event = full_text;
        if (full_text.length() >= 3 && full_text[1] == ':') {
            category = full_text.substr(0, 2);
            event = full_text.substr(3);
        }
        return {category, event, date, time, this->get_category_icon_(category)};
    }

    std::string get_category_icon_(const std::string& category) {
        static const std::map<std::string, const char*> category_map = {
            {"F:", "\U0000E87D"},
            {"W:", "\U0000E8F9"},
            {"P:", "\U0000E7FD"},
            {"J:", "\U0000E531"},
            {"M:", "\U0000f036"},
            {"B:", "\U0000e7e9"}
        };
        auto it = category_map.find(category);
        return (it != category_map.end()) ? it->second : "";
    }
};

}  // namespace task_manager_ns
}  // namespace esphome

#endif // TASK_MANAGER_H
