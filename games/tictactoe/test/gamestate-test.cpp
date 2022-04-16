#include <gtest/gtest.h>
#include "tictactoe/TicTacToe.h"
#include <algorithm>

TEST(GameState, PlayMoves) {
    using namespace play::tictactoe;
    using namespace play::game;

    GameState state = GameState::newGame();

    EXPECT_EQ(state.activePlayer(), Player::Player1);
    auto state2 = state.applyMove(Move{ {1, 1} });
    EXPECT_EQ(state2.activePlayer(), Player::Player2);
    
    auto state3 = state2.applyMove(Move{ {1, 1} }); // invalid move: cell already played by X
    EXPECT_EQ(state3.activePlayer(), Player::Player2);
    EXPECT_EQ((state3.board()[{1, 1}]), Player::Player1);

    state3 = state2.applyMove(Move{ {0, 1} });
    EXPECT_EQ(state3.activePlayer(), Player::Player1);
    EXPECT_EQ((state3.board()[{0, 1}]), Player::Player2);
}

TEST(GameState, GameOverNoWinner) {
    using namespace play::tictactoe;
    using namespace play::game;

    GameState state = GameState::newGame();

    const std::array<Move, 8> moves{
        Move{{1, 1}}, Move{{0, 0}},
        Move{{0, 1}}, Move{{2, 1}},
        Move{{1, 0}}, Move{{1, 2}},
        Move{{2, 0}}, Move{{0, 2}},
    };

    EXPECT_FALSE(state.isOver());
    EXPECT_EQ(state.winner(), Player::None);

    auto s = state;
    for (const Move& m : moves) {
        s = s.applyMove(m);
        EXPECT_FALSE(s.isOver());
        EXPECT_EQ(s.winner(), Player::None);
    }
    s = s.applyMove(Move{ { 2, 2} });
    EXPECT_TRUE(s.isOver());
    EXPECT_EQ(s.winner(), Player::None);
}

TEST(GameState, GameOverWinnerX) {
    using namespace play::tictactoe;
    using namespace play::game;

    GameState state = GameState::newGame();

    const std::array<Move, 6> moves{
        Move{{ 1, 1 }}, Move{{ 0, 0 }},
        Move{{ 0, 1 }}, Move{{ 2, 1 }},
        Move{{ 2, 0 }}, Move{{ 1, 0 }}
    };

    auto s = state;
    for (const Move& m : moves) {
        s = s.applyMove(m);
        EXPECT_FALSE(s.isOver());
        EXPECT_EQ(s.winner(), Player::None);
    }
    s = s.applyMove(Move{ { 0, 2 } });
    EXPECT_TRUE(s.isOver());
    EXPECT_EQ(s.winner(), Player::Player1);
}

std::vector<play::tictactoe::Move> generateAllMoves() {
    std::vector<play::tictactoe::Move> moves;
    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
            moves.emplace_back(play::tictactoe::Point{ row, col });
    return moves;
}

namespace play::tictactoe {
bool operator==(const Move& m1, const Move& m2) {
    const auto& p1 = m1.point();
    const auto& p2 = m2.point();
    return p1.row() == p2.row() && p1.col() == p2.col();
}
}

TEST(GameState, AvailableMoves) {
    using namespace play::tictactoe;

    GameState state = GameState::newGame();

    auto moves = state.availableMoves();
    const auto allMoves = generateAllMoves();
    EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), allMoves.begin(), allMoves.end()));

    // TODO Test after a few moves
}