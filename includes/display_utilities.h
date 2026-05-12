#ifndef DISPLAY_UTILITIES_H
#define DISPLAY_UTILITIES_H

#include <string>
#include <map>

inline const char* get_battery_icon(float voltage) {
    if (voltage < 3.4) return "\U0000E19C"; 
    if (voltage < 3.6) return "\U0000F155"; 
    if (voltage < 3.8) return "\U0000EBDD"; 
    if (voltage < 4.3) return "\U0000E1A4"; 
    return "";
}

inline const char* get_weather_icon(const std::string& condition) {
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

template<typename T>
T get_temp_color(float temp, T hot_color, T normal_color) {
    return (temp >= 25.0f) ? hot_color : normal_color;
}

#endif // DISPLAY_UTILITIES_H
