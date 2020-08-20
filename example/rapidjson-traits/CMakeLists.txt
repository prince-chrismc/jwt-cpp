find_package(RapidJSON CONFIG REQUIRED)

add_executable(rapidjson-traits main.cpp)
target_link_libraries(rapidjson-traits jwt-cpp::jwt-cpp rapidjson)
