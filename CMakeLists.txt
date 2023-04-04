cmake_minimum_required(VERSION 3.0.0)
project(Vaultware VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_subdirectory(deps)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include(CTest)
enable_testing()

add_executable(Vaultware main.cpp)
target_compile_definitions(Vaultware PRIVATE
    DEBUG=$<CONFIG:Debug>
)
if(${MSVC})
    target_link_options(Vaultware PRIVATE
        /SUBSYSTEM:$<$<CONFIG:Debug>:CONSOLE>$<$<NOT:$<CONFIG:Debug>>:WINDOWS>
    )
endif(${MSVC})

target_link_libraries(Vaultware PRIVATE botan wx::core wx::base sqlite3)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
