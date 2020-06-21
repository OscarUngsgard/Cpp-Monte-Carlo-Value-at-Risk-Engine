# this can be much lower
cmake_minimum_required(VERSION 3.17)

project(monte_carlo)

add_executable(monte_carlo)


#Copy CSV files with data to binary dir
file(GLOB MY_INPUT_CSV_FILES *.csv)
file(COPY ${MY_INPUT_CSV_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


file(GLOB_RECURSE ALL_FILES src/*.cpp include/*.h)

target_sources(monte_carlo
        PRIVATE
                ${ALL_FILES}
)

target_include_directories(monte_carlo
        PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/include
)

