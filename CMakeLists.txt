cmake_minimum_required(VERSION 3.10)
project(Restora VERSION 1.1.4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Release build only
set(CMAKE_BUILD_TYPE Release)

if(MINGW)
    # MinGW flags - static linking to avoid DLL dependencies
    # Optimize for OLD CPUs (Pentium 4 era) - no SSE2/SSE3 requirements
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        # 32-bit build for old PCs
        add_compile_options(-O2 -Wall -Wno-unused-result -static -march=i686 -mtune=generic)
        add_link_options(-static -static-libgcc -static-libstdc++)
    else()
        # 64-bit build
        add_compile_options(-O2 -Wall -Wno-unused-result -static -march=x86-64 -mtune=generic)
        add_link_options(-static -static-libgcc -static-libstdc++)
    endif()
    
    # Target Windows 7 (NT 6.1) for maximum compatibility
    add_compile_definitions(WINVER=0x0601 _WIN32_WINNT=0x0601 _WIN32_IE=0x0700)
    
    # Set subsystem to Windows 7 compatible
    add_link_options(-Wl,--subsystem,windows:6.01)
    
elseif(MSVC)
    add_compile_options(/O2 /W3 /DNDEBUG)
    add_compile_definitions(WINVER=0x0601 _WIN32_WINNT=0x0601)
    add_link_options(/SUBSYSTEM:CONSOLE,6.01)
else()
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

install(TARGETS restora DESTINATION bin)
