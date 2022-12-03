add_executable(day3
        day3.cpp
        )

# pull in common dependencies
target_link_libraries(day3 pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(day3)

# enable usb output, disable uart output
pico_enable_stdio_usb(day3 1)
pico_enable_stdio_uart(day3 0)
