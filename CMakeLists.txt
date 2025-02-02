# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# Generated CMake Pico project file

cmake_minimum_required(VERSION 3.13)

# Definir padrões de compilação
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Importar o Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Definir o nome do projeto
project(main C CXX ASM)

# Inicializar o Raspberry Pi Pico SDK
pico_sdk_init()

# Definir o modelo da placa (Pico padrão)
set(PICO_BOARD pico CACHE STRING "Board type")

# Adicionar o executável principal
add_executable(main
    main.c  # Certifique-se de que o arquivo exista neste diretório
)

# Definir o nome do programa e a versão
pico_set_program_name(main "main")
pico_set_program_version(main "0.1")

# Ativar saída via USB/UART
pico_enable_stdio_uart(main 1)
pico_enable_stdio_usb(main 1)

# Gerar cabeçalhos PIO (se necessário)
# pico_generate_pio_header(main ${CMAKE_CURRENT_LIST_DIR}/pio_matrix.pio)

# Vincular bibliotecas necessárias
target_link_libraries(main PRIVATE
    pico_stdlib
    hardware_pio
    hardware_adc
    hardware_pwm
    hardware_timer
)

# Incluir diretórios necessários
target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    $ENV{PICO_SDK_PATH}/sdk/include
    $ENV{PICO_SDK_PATH}/sdk/src/rp2_common/hardware_timer/include
    $ENV{PICO_SDK_PATH}/sdk/src/rp2_common/hardware_pwm/include
    $ENV{PICO_SDK_PATH}/sdk/src/rp2_common/hardware_pio/include
    $ENV{PICO_SDK_PATH}/sdk/src/common/include
)

# Gerar arquivos de saída (UF2, ELF, etc.)
pico_add_extra_outputs(main)
