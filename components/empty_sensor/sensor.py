import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import UNIT_CELSIUS, ICON_THERMOMETER, DEVICE_CLASS_TEMPERATURE

# Create a namespace for our component
my_sensor_ns = cg.esphome_ns.namespace("empty_sensor")
MySensor = my_sensor_ns.class_("MySensor", sensor.Sensor, cg.PollingComponent)

# Configuration schema
CONFIG_SCHEMA = (
    sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_TEMPERATURE
    )
    .extend(
        {
            cv.Optional("update_interval", default="60s"): cv.update_interval,
        }
    )
    .extend(cv.polling_component_schema("60s"))
)

# Code generation
async def to_code(config):
    var = cg.new_Pvariable(config[cv.CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
