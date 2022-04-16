/* *********************************************************** *
 * InvisiableMatch.h
 * *********************************************************** */

#ifndef GAMEPLAY_INVISIBLE_MATCH_H
#define GAMEPLAY_INVISIBLE_MATCH_H

#include "../agent/Agent.h"
#include "../random_selection.h"
#include "Player.h"

namespace play::game {

template<class GameState, class Move, class... Args>
Player playInvisibleMatch(play::agent::Agent<GameState, Move>* player1, play::agent::Agent<GameState, Move>* player2, Args... args) {
    GameState game = GameState::newGame(args...);
    random_selector<> selector{};

    while (!isGameOver(game)) {
        const auto player1Moves = player1->selectMoves(game);
        const auto& m = selector(player1Moves);
        game = applyMove(m, game);
        if (!isGameOver(game)) {
            const auto player2Moves = player2->selectMoves(game);
            const auto& m = selector(player2Moves);
            game = applyMove(m, game);
        }
    }
    return getWinner(game);
}

}
#endif
