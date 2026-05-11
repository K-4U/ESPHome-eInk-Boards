import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

task_manager_ns = cg.esphome_ns.namespace('task_manager')
TaskManager = task_manager_ns.class_('TaskManager', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(TaskManager),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
