import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import UNIT_CELSIUS, ICON_THERMOMETER, DEVICE_CLASS_TEMPERATURE

DEPENDENCIES = []
AUTO_LOAD = ["sensor"]

my_custom_ns = cg.esphome_ns.namespace("my_custom_component")
MyCustomSensor = my_custom_ns.class_("MyCustomSensor", sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = sensor.sensor_schema(
    unit_of_measurement=UNIT_CELSIUS,
    icon=ICON_THERMOMETER,
    accuracy_decimals=1,
    device_class=DEVICE_CLASS_TEMPERATURE
).extend({
    cv.GenerateID(): cv.declare_id(MyCustomSensor),
    cv.Optional("update_interval", default="5s"): cv.update_interval,
})

async def to_code(config):
    var = cg.new_Pvariable(config[cv.GenerateID()])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
