```yaml
# example configuration:

light:
  - platform: empty_light
    name: Empty light

output:
  - platform: gpio
    pin: D1
    frequency: 1000 Hz
    id: pwm_output
```