cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(lab1 VERSION ${PROJECT_VESRION})

add_executable(lab1 main.cpp)

set_target_properties(lab1 PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(lab1 PRIVATE
        /W4
    )
else ()
    target_compile_options(lab1 PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()