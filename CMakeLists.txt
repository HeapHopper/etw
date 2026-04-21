cmake_minimum_required(VERSION 3.26)

# Force MSVC (optional but explicit)
# if(NOT MSVC)
    # message(FATAL_ERROR "This project requires MSVC.")
# endif()

project(ETW LANGUAGES CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Provider target ----
add_executable(Provider
    ${CMAKE_CURRENT_SOURCE_DIR}/src/provider/provider.cpp
)

# ---- Consumer target ----
add_executable(Consumer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/consumer/consumer.cpp
)

# ---- Common ETW libraries ----
# advapi32 -> StartTrace, ControlTrace, etc.
# tdh      -> Trace Data Helper (decoding events)
# evntcons -> Consumer API (OpenTrace, ProcessTrace)
# evntprov -> Provider API (TraceLogging / EventWrite)
set(ETW_LIBS
    advapi32
    tdh
    # evntcons
    # evntprov
)

target_compile_definitions(Consumer PRIVATE UNICODE _UNICODE)
target_compile_definitions(Provider PRIVATE UNICODE _UNICODE)

target_link_libraries(Provider PRIVATE ${ETW_LIBS})
target_link_libraries(Consumer PRIVATE ${ETW_LIBS})

# Optional: stricter compile settings for MSVC
# target_compile_options(Provider PRIVATE /W4 /permissive-)
# target_compile_options(Consumer PRIVATE /W4 /permissive-)