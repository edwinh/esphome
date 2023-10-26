import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID, CONF_ADDRESS, CONF_PULSE_LENGTH, CONF_PIN
from esphome.core import TimePeriod
from esphome import pins

CONF_UNIT = "unit"

kaku_old_switch_ns = cg.esphome_ns.namespace('kaku_old_switch')
KakuOldSwitch = kaku_old_switch_ns.class_('KakuOldSwitch', switch.Switch, cg.Component)

CONFIG_SCHEMA = switch.SWITCH_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(KakuOldSwitch),
    cv.Optional(CONF_PULSE_LENGTH, default="420us"): cv.All(cv.positive_time_period, cv.Range(max=TimePeriod(microseconds=1000))),
    cv.Required(CONF_ADDRESS): cv.All(cv.Length(max=1)),
    cv.Required(CONF_UNIT): cv.int_,
    # TODO: Fix pin
    #cv.Required(CONF_PIN): pins.gpio_output_pin_schema,
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await switch.register_switch(var, config)
    # TODO: Fix pin
    #pin = await cg.gpio_pin_expression(config[CONF_PIN])
    #cg.add(var.set_pin(pin))

    if CONF_PULSE_LENGTH in config:
        duration = int(
          round(config[CONF_PULSE_LENGTH].total_microseconds))
        cg.add(var.set_pulsewidth(duration))
    
    # Convert letter to int A = 0, D = 3
    address = ord(config[CONF_ADDRESS][0]) - ord('A')
    cg.add(var.set_address(address))

    unit = int(config[CONF_UNIT])
    cg.add(var.set_unit(unit))
