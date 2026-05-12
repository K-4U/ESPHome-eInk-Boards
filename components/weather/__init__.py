import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.components import font, color, sensor, text_sensor

weather_ns = cg.esphome_ns.namespace('weather_ns')
Weather = weather_ns.class_('Weather', cg.Component)

Color = cg.esphome_ns.class_('Color')

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Weather),
    cv.Optional("font_icon"): cv.use_id(font.Font),
    cv.Optional("font_temp"): cv.use_id(font.Font),
    cv.Optional("font_mdi_small"): cv.use_id(font.Font),
    cv.Optional("color_blk"): cv.use_id(Color),
    cv.Optional("color_red"): cv.use_id(Color),
    cv.Optional("temperature_sensor"): cv.use_id(sensor.Sensor),
    cv.Optional("temperature_min_sensor"): cv.use_id(sensor.Sensor),
    cv.Optional("temperature_max_sensor"): cv.use_id(sensor.Sensor),
    cv.Optional("condition_sensor"): cv.use_id(text_sensor.TextSensor),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    if "font_icon" in config:
        f = await cg.get_variable(config["font_icon"])
        cg.add(var.set_font_icon(f))
    if "font_temp" in config:
        f = await cg.get_variable(config["font_temp"])
        cg.add(var.set_font_temp(f))
    if "font_mdi_small" in config:
        f = await cg.get_variable(config["font_mdi_small"])
        cg.add(var.set_font_mdi_small(f))
        
    if "color_blk" in config:
        c = await cg.get_variable(config["color_blk"])
        cg.add(var.set_color_blk(c))
    if "color_red" in config:
        c = await cg.get_variable(config["color_red"])
        cg.add(var.set_color_red(c))
        
    if "temperature_sensor" in config:
        s = await cg.get_variable(config["temperature_sensor"])
        cg.add(var.set_sensor_temp(s))
    if "temperature_min_sensor" in config:
        s = await cg.get_variable(config["temperature_min_sensor"])
        cg.add(var.set_sensor_temp_min(s))
    if "temperature_max_sensor" in config:
        s = await cg.get_variable(config["temperature_max_sensor"])
        cg.add(var.set_sensor_temp_max(s))
    if "condition_sensor" in config:
        s = await cg.get_variable(config["condition_sensor"])
        cg.add(var.set_sensor_condition(s))
