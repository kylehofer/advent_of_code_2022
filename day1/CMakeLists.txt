add_executable(day1
        day1.cpp
        )

# pull in common dependencies
target_link_libraries(day1 pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(day1)

# enable usb output, disable uart output
pico_enable_stdio_usb(day1 1)
pico_enable_stdio_uart(day1 0)

# add url via pico_set_program_url
# example_auto_set_url(blink)
