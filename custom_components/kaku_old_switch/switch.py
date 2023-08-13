import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID, CONF_ADDRESS
from esphome.core import TimePeriod

CONF_PULSEWIDTH = "pulsewidth"
# CONF_ADDRESS = "address"
CONF_UNIT = "unit"

kaku_old_switch_ns = cg.esphome_ns.namespace('kaku_old_switch')
KakuOldSwitch = kaku_old_switch_ns.class_('KakuOldSwitch', switch.Switch, cg.Component)

CONFIG_SCHEMA = switch.SWITCH_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(KakuOldSwitch),
    cv.Optional(CONF_PULSEWIDTH, default="420us"): cv.All(cv.positive_time_period, cv.Range(max=TimePeriod(microseconds=1000))),
    cv.Required(CONF_ADDRESS): cv.All(cv.Length(max=1)),
    cv.Required(CONF_UNIT): cv.int_,
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await switch.register_switch(var, config)
    if CONF_PULSEWIDTH in config:
        duration = int(
          round(config[CONF_PULSEWIDTH].total_microseconds))
        cg.add(var.set_pulsewidth(duration))
    
    address = ord(config[CONF_ADDRESS][0]) - ord('A')
    cg.add(var.set_address(address))

    unit = int(config[CONF_UNIT])
    cg.add(var.set_unit(unit))
