# Add the executable for read_all.c
add_executable(read_all read_all.c)
add_executable(test test.c)

# pull in common dependencies and additional i2c hardware support
target_link_libraries(read_all PRIVATE qwstpad pico_stdlib pico_printf hardware_i2c)
target_link_libraries(test PRIVATE qwstpad pico_stdlib pico_printf hardware_i2c)

# add include paths
target_include_directories(read_all PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(test PRIVATE ${PROJECT_SOURCE_DIR}/include)

# enable usb output, disable uart output
pico_enable_stdio_usb(read_all 1)
pico_enable_stdio_uart(read_all 0)
pico_enable_stdio_usb(test 1)
pico_enable_stdio_uart(test 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(read_all)
pico_add_extra_outputs(test)

# add url via pico_set_program_url
# example_auto_set_url(read_all)
# example_auto_set_url(test)