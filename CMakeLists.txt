cmake_minimum_required(VERSION 3.26)

project(ETW LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ---- Common ETW libraries ----
# advapi32 -> StartTrace, ControlTrace, etc.
# tdh      -> Trace Data Helper (decoding events)
set(ETW_LIBS
    advapi32
    tdh
)

# ---- Provider target ----
add_executable(Provider
    ${CMAKE_CURRENT_SOURCE_DIR}/src/provider/provider.cpp
)

target_compile_definitions(Provider PRIVATE UNICODE _UNICODE) # for windows SDK string-type unicode support
target_link_libraries(Provider PRIVATE ${ETW_LIBS})


# ---- Consumer target ----
add_executable(Consumer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/consumer/consumer.cpp
)

target_compile_definitions(Consumer PRIVATE UNICODE _UNICODE)
target_link_libraries(Consumer PRIVATE ${ETW_LIBS})