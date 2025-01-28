# Add library
add_library(qwstpad INTERFACE)

# Add source and include directories
target_sources(qwstpad INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/qwstpad.c)

target_include_directories(qwstpad INTERFACE ${PROJECT_SOURCE_DIR}/include)

# link libraries to the build
target_link_libraries(qwstpad INTERFACE pico_stdlib hardware_i2c)
