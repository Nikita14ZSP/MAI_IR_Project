cmake_minimum_required(VERSION 3.10)
project(MAI_IR_Core)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# UTF-8 поддержка
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finput-charset=UTF-8 -fexec-charset=UTF-8")
endif()

# Директории исходников
set(CORE_SOURCES
    tokenizer/tokenizer.cpp
    stemmer/stemmer.cpp
    analysis/zipf_analyzer.cpp
    index/boolean_index.cpp
    search/boolean_search.cpp
    utils/file_utils.cpp
    utils/string_utils.cpp
)

set(CORE_HEADERS
    tokenizer/tokenizer.h
    stemmer/stemmer.h
    analysis/zipf_analyzer.h
    index/boolean_index.h
    search/boolean_search.h
    utils/file_utils.h
    utils/string_utils.h
    utils/vector.h
    utils/map.h
    utils/set.h
    utils/sort.h
)

# Основная библиотека
add_library(mai_ir_core STATIC ${CORE_SOURCES} ${CORE_HEADERS})
target_include_directories(mai_ir_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# CLI приложение
set(CLI_SOURCES
    cli/main_cli.cpp
    cli/search_cli.cpp
)

set(CLI_HEADERS
    cli/search_cli.h
)

add_executable(search_cli ${CLI_SOURCES} ${CLI_HEADERS})
target_link_libraries(search_cli mai_ir_core)

# Установить выходную директорию для удобства
set_target_properties(search_cli PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# Утилита для построения индекса
add_executable(build_index cli/build_index.cpp)
target_link_libraries(build_index mai_ir_core)

# Утилита для анализа закона Ципфа
add_executable(zipf_analysis cli/zipf_analysis.cpp)
target_link_libraries(zipf_analysis mai_ir_core)

# Тесты удалены

