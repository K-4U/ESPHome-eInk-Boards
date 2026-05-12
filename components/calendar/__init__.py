import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.components import font, color

calendar_ns = cg.esphome_ns.namespace('calendar_ns')
Calendar = calendar_ns.class_('Calendar', cg.Component)

Color = cg.esphome_ns.class_('Color')

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Calendar),
    cv.Optional("font_header"): cv.use_id(font.Font),
    cv.Optional("font_numbers"): cv.use_id(font.Font),
    cv.Optional("color_blk"): cv.use_id(Color),
    cv.Optional("color_red"): cv.use_id(Color),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    if "font_header" in config:
        f = await cg.get_variable(config["font_header"])
        cg.add(var.set_font_header(f))
    if "font_numbers" in config:
        f = await cg.get_variable(config["font_numbers"])
        cg.add(var.set_font_numbers(f))
        
    if "color_blk" in config:
        c = await cg.get_variable(config["color_blk"])
        cg.add(var.set_color_blk(c))
    if "color_red" in config:
        c = await cg.get_variable(config["color_red"])
        cg.add(var.set_color_red(c))
