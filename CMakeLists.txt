cmake_minimum_required(VERSION 3.10)
project(Restora VERSION 1.1.4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /O2)
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        add_link_options(/SUBSYSTEM:CONSOLE /LARGEADDRESSAWARE)
    endif()
else()
    add_compile_options(-Wall -O2)
endif()

add_executable(restora
    src/main.cpp
    src/scanner.cpp
    src/recovery.cpp
    src/repair.cpp
    src/console.cpp
)

target_include_directories(restora PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(WIN32)
    target_link_libraries(restora PRIVATE shlwapi)
endif()

# Install
install(TARGETS restora DESTINATION bin)
