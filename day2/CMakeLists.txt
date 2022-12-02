add_executable(day2
        day2.cpp
        )

# pull in common dependencies
target_link_libraries(day2 pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(day2)

# enable usb output, disable uart output
pico_enable_stdio_usb(day2 1)
pico_enable_stdio_uart(day2 0)

# add url via pico_set_program_url
# example_auto_set_url(blink)
