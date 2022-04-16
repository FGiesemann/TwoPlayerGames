add_library(connectfour
    ConnectFour.cpp
)

target_include_directories(connectfour
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(connectfour
    twoplayergames
)

add_executable(connectfour-test
    test/board-test.cpp
    test/gamestate-test.cpp
)

target_link_libraries(connectfour-test 
    connectfour
    gtest gtest_main
)
