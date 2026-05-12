import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.components import font, color

task_manager_ns = cg.esphome_ns.namespace('task_manager_ns')
TaskManager = task_manager_ns.class_('TaskManager', cg.Component)

Color = cg.esphome_ns.class_('Color')

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(TaskManager),
    cv.Optional("font_title"): cv.use_id(font.Font),
    cv.Optional("font_mdi_22"): cv.use_id(font.Font),
    cv.Optional("font_today_task"): cv.use_id(font.Font),
    cv.Optional("font_today_time"): cv.use_id(font.Font),
    cv.Optional("font_upcoming_date"): cv.use_id(font.Font),
    cv.Optional("font_upcoming_task"): cv.use_id(font.Font),
    cv.Optional("color_blk"): cv.use_id(Color),
    cv.Optional("color_red"): cv.use_id(Color),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    if "font_title" in config:
        f = await cg.get_variable(config["font_title"])
        cg.add(var.set_font_title(f))
    if "font_mdi_22" in config:
        f = await cg.get_variable(config["font_mdi_22"])
        cg.add(var.set_font_mdi_22(f))
    if "font_today_task" in config:
        f = await cg.get_variable(config["font_today_task"])
        cg.add(var.set_font_today_task(f))
    if "font_today_time" in config:
        f = await cg.get_variable(config["font_today_time"])
        cg.add(var.set_font_today_time(f))
    if "font_upcoming_date" in config:
        f = await cg.get_variable(config["font_upcoming_date"])
        cg.add(var.set_font_upcoming_date(f))
    if "font_upcoming_task" in config:
        f = await cg.get_variable(config["font_upcoming_task"])
        cg.add(var.set_font_upcoming_task(f))
        
    if "color_blk" in config:
        c = await cg.get_variable(config["color_blk"])
        cg.add(var.set_color_blk(c))
    if "color_red" in config:
        c = await cg.get_variable(config["color_red"])
        cg.add(var.set_color_red(c))
