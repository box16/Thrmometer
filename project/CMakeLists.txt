add_executable(thermometer)

target_sources(thermometer PRIVATE main.cpp)
include_directories(${CMAKE_CURRENT_LIST_DIR})


target_link_libraries(thermometer PRIVATE pico_stdlib hardware_pio)
pico_add_extra_outputs(thermometer)