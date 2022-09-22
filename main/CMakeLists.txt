set(COMPONENT_SRCS "main.c wifi.c mqtt.c dht11.c led.c nvs.c button.c buzzer.c")
set(COMPONENT_ADD_INCLUDEDIRS ".")

register_component()

target_compile_options(${COMPONENT_LIB} PRIVATE  -Wno-format)