#include <gtest/gtest.h>
#include "connectfour/ConnectFour.h"

#include <algorithm>
#include <array>

TEST(GameState, PlayMoves) {
    using namespace play::connectfour;
    using namespace play::game;

    GameState game = GameState::newGame(3, 4);

    EXPECT_EQ(game.activePlayer(), Player::Player1);

    auto state = game.dropStone(0);
    EXPECT_EQ(state.activePlayer(), Player::Player2);

    state = state.dropStone(0);
    EXPECT_EQ(state.activePlayer(), Player::Player1);

    auto state1 = state.dropStone(0);
    EXPECT_EQ(state1.activePlayer(), Player::Player2);
    auto state1_legal = state1.availableMoves();
    EXPECT_TRUE(std::none_of(state1_legal.begin(), state1_legal.end(), [](int i) { return i == 0; }));

    auto state2_1 = state1.dropStone(0);
    EXPECT_EQ(state2_1.activePlayer(), Player::Player2);

    auto state2_2 = state1.dropStone(2);
    EXPECT_EQ(state2_2.activePlayer(), Player::Player1);
}

TEST(GameState, GameOverDraw) {
    using namespace play::connectfour;

    std::array<int, 6 * 7 - 1> moves{
        1, 2, 0, 1, 2, 4, 3, 2, 1, 5, 6, 3, 4, 3, 3, 4, 0, 5, 5, 4, 5, 2, 5, 5, 1, 2, 0, 0, 2, 1, 1, 0, 0, 6, 6, 6, 6, 6, 3, 4, 4
    };
    int lastMove = 3;

    GameState game = GameState::newGame();
    for (auto& m : moves) {
        game = game.dropStone(m);
        EXPECT_FALSE(game.isOver());
    }
    game = game.dropStone(lastMove);
    EXPECT_TRUE(game.isOver());
}

bool vectorsSimilar(const std::vector<int>& v1, const std::vector<int>& v2) {
    return v1.size() == v2.size() && std::is_permutation(v1.begin(), v1.end(), v2.begin());
}

TEST(GameState, AvailableMoves) {
    using namespace play::connectfour;

    GameState game = GameState::newGame();
    EXPECT_TRUE(vectorsSimilar(game.availableMoves(), { 0, 1, 2, 3, 4, 5, 6 }));

    std::array<int, 10> moves1{ 1, 3, 2, 5, 3, 4, 5, 0, 1, 2 };
    for (auto& m : moves1) {
        game = game.dropStone(m);
        EXPECT_TRUE(vectorsSimilar(game.availableMoves(), { 0, 1, 2, 3, 4, 5, 6 }));
    }
    for (int i = 0; i < 3; ++i) {
        game = game.dropStone(5);
        EXPECT_TRUE(vectorsSimilar(game.availableMoves(), { 0, 1, 2, 3, 4, 5, 6 }));
    }
    game = game.dropStone(5);
    EXPECT_TRUE(vectorsSimilar(game.availableMoves(), { 0, 1, 2, 3, 4, 6 }));
}