cmake_minimum_required(VERSION 3.22)

project(server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Find necessary Qt components, including SerialPort
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia SerialPort)

# INSTALL FONTS
if(NOT EXISTS "$ENV{HOME}/.local/share/fonts/MaterialIcons.ttf")
    file(COPY "./desktop/client/res/MaterialIcons.ttf" DESTINATION "$ENV{HOME}/.local/share/fonts")
endif()

# Initial set of source files for client and server
set(SOURCES_CLIENT
    ${CMAKE_CURRENT_SOURCE_DIR}/desktop/client/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/desktop/client/src/canvas.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/desktop/client/src/window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/desktop/client/src/comservice.cpp
)

set(SOURCES_SERVER
    ${CMAKE_CURRENT_SOURCE_DIR}/desktop/server/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/desktop/server/src/window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/desktop/server/src/comservice.cpp
)

# Create custom targets
add_custom_target(use_uart cd ${CMAKE_BINARY_DIR} && cmake .. -DUART:BOOL=ON)
add_custom_target(use_tcpip cd ${CMAKE_BINARY_DIR} && cmake .. -DUART:BOOL=OFF)

if(UART)
    add_compile_definitions(client PRIVATE UART)

    # Append UART-specific source files to the existing lists
    list(APPEND SOURCES_CLIENT
        ${CMAKE_CURRENT_SOURCE_DIR}/desktop/client/src/serialservice.cpp
    )
    list(APPEND SOURCES_SERVER
        ${CMAKE_CURRENT_SOURCE_DIR}/desktop/server/src/serialservice.cpp
    )

    #### Custom target to build and monitor ESP32 client using gnome-terminal
    add_custom_target(upload_client cd ${CMAKE_SOURCE_DIR}/esp32/client && pio run -t upload)

    ##### Custom target to build and monitor ESP32 server using gnome-terminal
    add_custom_target(upload_server cd ${CMAKE_SOURCE_DIR}/esp32/server && pio run -t upload)

else()
    # Append TCP-specific source files to the existing lists
    list(APPEND SOURCES_CLIENT
        ${CMAKE_CURRENT_SOURCE_DIR}/desktop/client/src/tcpservice.cpp
    )
    list(APPEND SOURCES_SERVER
        ${CMAKE_CURRENT_SOURCE_DIR}/desktop/server/src/tcpservice.cpp
    )

endif()

# Create executables outside of if statement
add_executable(client ${SOURCES_CLIENT})
add_executable(server ${SOURCES_SERVER})

# Link common Qt libraries
target_link_libraries(client PUBLIC Qt6::Core Qt6::Widgets Qt6::Multimedia Qt6::SerialPort)
target_link_libraries(server PUBLIC Qt6::Core Qt6::Widgets Qt6::SerialPort)

# Include the header files directory
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/desktop/client/include shared)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/desktop/server/include shared)

