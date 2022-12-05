add_executable(day5
        day5.cpp
        )

# pull in common dependencies
target_link_libraries(day5 pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(day5)

# enable usb output, disable uart output
pico_enable_stdio_usb(day5 1)
pico_enable_stdio_uart(day5 0)
