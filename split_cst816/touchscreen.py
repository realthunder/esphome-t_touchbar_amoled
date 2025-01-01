from esphome import pins
import esphome.codegen as cg
from esphome.components import i2c, touchscreen
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_INTERRUPT_PIN, CONF_RESET_PIN


CODEOWNERS = ["@realthunder", "@clydebarrow"]
DEPENDENCIES = ["i2c"]

cst816_ns = cg.esphome_ns.namespace("split_cst816")

CST816Touchscreen = cst816_ns.class_(
    "CST816Touchscreen",
    touchscreen.Touchscreen,
    i2c.I2CDevice,
)

CST816ButtonListener = cst816_ns.class_("CST816ButtonListener")

CONF_MIDDLE_SPLIT = "middle_split"
CONF_SKIP_PROBE = "skip_probe"
CONFIG_SCHEMA = touchscreen.TOUCHSCREEN_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(CST816Touchscreen),
        cv.Optional(CONF_INTERRUPT_PIN): pins.internal_gpio_input_pin_schema,
        cv.Optional(CONF_RESET_PIN): pins.gpio_output_pin_schema,
        cv.Optional(CONF_SKIP_PROBE, default=False): cv.boolean,
        cv.Optional(CONF_MIDDLE_SPLIT, default=32): cv.uint8_t,
    }
).extend(i2c.i2c_device_schema(0x15))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await touchscreen.register_touchscreen(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_skip_probe(config[CONF_SKIP_PROBE]))
    cg.add(var.set_middle_split(config[CONF_MIDDLE_SPLIT]))
    if interrupt_pin := config.get(CONF_INTERRUPT_PIN):
        cg.add(var.set_interrupt_pin(await cg.gpio_pin_expression(interrupt_pin)))
    if reset_pin := config.get(CONF_RESET_PIN):
        cg.add(var.set_reset_pin(await cg.gpio_pin_expression(reset_pin)))
