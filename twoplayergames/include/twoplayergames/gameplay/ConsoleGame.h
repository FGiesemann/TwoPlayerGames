/* *********************************************************** *
 * ConsoleGame.h
 * *********************************************************** */

#ifndef GAMEPLAY_CONSOLEGAME_H
#define GAMEPLAY_CONSOLEGAME_H

#include "../agent/Agent.h"
#include "../random_selection.h"
#include "Player.h"
#include <iostream>

namespace play::game {

template<class GameState, class Move, class... Args>
Player playConsoleGame(play::agent::Agent<GameState, Move>* player1, play::agent::Agent<GameState, Move>* player2, Args... args) {
    GameState game = GameState::newGame(args...);
    random_selector<> selector{};
    int round{ 1 };
    
    std::cout << game;
    while (!isGameOver(game)) {
        std::cout << "\nRound " << round << '\n';
        const auto player1Moves = player1->selectMoves(game);
        const auto& m = selector(player1Moves);
        std::cout << "Player 1 chooses " << m << '\n';
        game = applyMove(m, game);
        std::cout << game;

        if (!isGameOver(game)) {
            const auto player2Moves = player2->selectMoves(game);
            const auto& m = selector(player2Moves);
            std::cout << "Player 2 chooses " << m << '\n';
            game = applyMove(m, game);
            std::cout << game;
        }
        ++round;
    }
    return getWinner(game);
}

}

#endif
