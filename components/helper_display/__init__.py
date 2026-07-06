import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

helper_display_ns = cg.esphome_ns.namespace("helper_display")
HelperDisplay = helper_display_ns.class_("HelperDisplay", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(HelperDisplay),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
