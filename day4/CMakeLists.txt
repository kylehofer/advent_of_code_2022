add_executable(day4
        day4.cpp
        )

# pull in common dependencies
target_link_libraries(day4 pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(day4)

# enable usb output, disable uart output
pico_enable_stdio_usb(day4 1)
pico_enable_stdio_uart(day4 0)
