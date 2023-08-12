# example configuration:
# set default_transition_length to 0s to get the smoothest response.
# Parameters, all optional;
# pulsewidth (in us, default 260)
# address (default 1013)
# unit (default 2)

light:
  - platform: kaku_dimmable_light
    id: kaku_dinner
    name: dinnertable
    pulsewidth: 260us
    address: 1013
    unit: 2
    default_transition_length: 0s