#include "esphome/core/component.h"
#include "esphome/components/json/json_util.h"
#include <vector>
#include <string>
#include <map>

namespace esphome {
namespace task_manager {

struct Task {
    std::string category;
    std::string event;
    std::string date;
    std::string time;
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


 protected:
    Task parse_task_(const std::string& full_text, const std::string& date, const std::string& time) {
        if (full_text.length() >= 3 && full_text[1] == ':') {
            return {full_text.substr(0, 2), full_text.substr(3), date, time};
        }
        return {"", full_text, date, time};
    }
};

}  // namespace task_manager
}  // namespace esphome
