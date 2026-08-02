cmake_minimum_required(VERSION 3.10)
project(Restora VERSION 1.1.4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Release build only
set(CMAKE_BUILD_TYPE Release)

if(MINGW)
    # MinGW flags - static linking to avoid DLL dependencies
    add_compile_options(-O2 -Wall -Wno-unused-result -static)
    add_link_options(-static -static-libgcc -static-libstdc++)
elseif(MSVC)
    # MSVC flags for release
    add_compile_options(/O2 /W3 /DNDEBUG)
    add_link_options(/SUBSYSTEM:CONSOLE)
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        add_link_options(/LARGEADDRESSAWARE)
    endif()
else()
    # GCC/Clang flags
    add_compile_options(-O2 -Wall)
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
