import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output
from esphome.core import TimePeriod
from esphome.const import CONF_OUTPUT_ID, CONF_OUTPUT, CONF_ADDRESS, CONF_ID

kaku_dimmable_light_ns = cg.esphome_ns.namespace('kaku_dimmable_light')
KakuDimmableLightOutput = kaku_dimmable_light_ns.class_('KakuDimmableLightOutput', light.LightOutput)

CONF_PULSEWIDTH = "pulsewidth"
CONF_ADDRESS = "address"
CONF_UNIT = "unit"

CONFIG_SCHEMA = light.BRIGHTNESS_ONLY_LIGHT_SCHEMA.extend({
    cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(KakuDimmableLightOutput),
    cv.Optional(CONF_PULSEWIDTH, default="420us"): cv.All(cv.positive_time_period, cv.Range(max=TimePeriod(microseconds=1000))),
    cv.Optional(CONF_ADDRESS, default=1013): cv.int_,
    cv.Optional(CONF_UNIT, default=2): cv.int_
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)
    if CONF_PULSEWIDTH in config:
        duration = int(
          round(config[CONF_PULSEWIDTH].total_microseconds))
        cg.add(var.set_pulsewidth(duration))
    
    if CONF_ADDRESS in config:
        address = int(config[CONF_ADDRESS])
        cg.add(var.set_address(address))
    
    if CONF_UNIT in config:
        unit = int(config[CONF_UNIT])
        cg.add(var.set_unit(unit))
    