#include <iostream>
#include <iomanip>
#include <memory>

#include "twoplayergames/agent/Agent.h"
#include "twoplayergames/agent/InteractivePlayer.h"
#include "twoplayergames/agent/RandomPlayer.h"

#include "twoplayergames/gameplay/ConsoleGame.h"

#include "tictactoe/TicTacToe.h"

int main() {
    namespace Game = play::tictactoe;

    using Move = Game::Move;
    using GameState = Game::GameState;

    auto player = std::make_unique<play::agent::InteractivePlayer<GameState, Move>>();
    auto agent = std::make_unique<play::agent::RandomPlayer<GameState, Move>>();

    const auto& winner = play::game::playConsoleGame<GameState, Move>(player.get(), agent.get());
    if (winner == play::game::Player::Player1) {
        std::cout << "Player 1 wins!\n";
    } else if (winner == play::game::Player::Player2) {
        std::cout << "Player 2 wins!\n";
    } else {
        std::cout << "That's a draw!\n";
    }
}
