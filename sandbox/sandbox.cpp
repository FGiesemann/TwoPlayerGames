#include <iostream>
#include <iomanip>
#include <memory>

#include "twoplayergames/agent/Agent.h"
#include "twoplayergames/agent/InteractivePlayer.h"
#include "twoplayergames/agent/RandomPlayer.h"
#include "twoplayergames/agent/MinimaxPlayer.h"
#include "twoplayergames/gameplay/GameStateEvaluator.h"
#include "twoplayergames/agent/MCTSPlayer.h"

#include "twoplayergames/gameplay/ConsoleGame.h"
#include "twoplayergames/gameplay/InvisibleMatch.h"

#include "tictactoe/TicTacToe.h"
#include "connectfour/ConnectFour.h"

void mainInteractive() {
    namespace Game = play::connectfour;

    using Move = Game::Move;
    using GameState = Game::GameState;

    auto player = std::make_unique<play::agent::InteractivePlayer<GameState, Move>>();
    auto agent = std::make_unique<play::agent::MCTSPlayer<GameState, Move, 1000>>();

    const auto& winner = play::game::playConsoleGame<GameState, Move>(player.get(), agent.get());
    if (winner == play::game::Player::Player1) {
        std::cout << "Player 1 wins!\n";
    } else if (winner == play::game::Player::Player2) {
        std::cout << "Player 2 wins!\n";
    } else {
        std::cout << "That's a draw!\n";
    }
}

void printPlayerPercent(int wins, int rounds) {
    std::cout << std::fixed << std::setw(7) << std::setprecision(3) << static_cast<float>(wins) / (rounds + 1) * 100.f << '%';
}

void mainInvisibleTournament() {
    namespace Game = play::connectfour;

    using Move = Game::Move;
    using GameState = Game::GameState;

    auto bot1 = std::make_unique<play::agent::MinimaxPlayer<GameState, Move, play::connectfour::ConnectFourEvaluator_Streaks>>(3);
    auto bot2 = std::make_unique<play::agent::MCTSPlayer<GameState, Move, 2000>>();

    int player1Win = 0;
    int player2Win = 0;
    constexpr int rounds = 1000;

    for (int round = 0; round < rounds; ++round) {
        const auto& winner = play::game::playInvisibleMatch<GameState, Move>(bot1.get(), bot2.get());
        std::cout << "Round " << std::setw(4) << round + 1 << ": ";
        if (winner == play::game::Player::Player1) {
            ++player1Win;
            std::cout << "Player 1";
        } else if (winner == play::game::Player::Player2) {
            ++player2Win;
            std::cout << "Player 2";
        } else {
            std::cout << "Draw    ";
        }
        std::cout << " (";
        printPlayerPercent(player1Win, round);
        std::cout << " | ";
        printPlayerPercent(player2Win, round);
        std::cout << ")\n";
    }

    std::cout << "\nTournament result:\n";
    std::cout << "Rounds played: " << rounds << '\n';
    std::cout << "  Player 1 wins: " << std::setw(4) << player1Win << " (";
    printPlayerPercent(player1Win, rounds);
    std::cout << ")\n";
    std::cout << "  Player 2 wins: " << std::setw(4) << player2Win << " (";
    printPlayerPercent(player2Win, rounds);
    std::cout << ")\n";
    int drawCount = rounds - player1Win - player2Win;
    std::cout << "  Draw         : " << std::setw(4) << drawCount << " (";
    printPlayerPercent(drawCount, rounds);
    std::cout << ")\n";
}

int main() {
    //*
    mainInteractive();
    /*/
    mainInvisibleTournament();
    //*/
}
