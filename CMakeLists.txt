cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)

if(DEFINED TYPES)
    execute_process(
        COMMAND python3 generate_code.py "${TYPES}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    
    add_executable(fluid main.cpp)

    target_include_directories(fluid PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
else()
    message(FATAL_ERROR "TYPES variable is not defined. Use -DTYPES=<value>.")
endif()