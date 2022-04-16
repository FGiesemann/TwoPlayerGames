add_library(tictactoe
    TicTacToe.cpp
)

target_include_directories(tictactoe
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(tictactoe
    twoplayergames
)

add_executable(tictactoe-test
    test/board-tests.cpp
    test/gamestate-test.cpp
)

target_link_libraries(tictactoe-test
    tictactoe
    gtest gtest_main
)
