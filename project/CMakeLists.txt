add_executable(thermometer)

target_sources(thermometer PRIVATE main.cpp)
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/LCD)
include_directories(${CMAKE_CURRENT_LIST_DIR}/SC1602BSLB)


target_link_libraries(thermometer PRIVATE pico_stdlib hardware_pio)
target_link_libraries(thermometer PRIVATE pico_stdlib hardware_adc)
pico_add_extra_outputs(thermometer)