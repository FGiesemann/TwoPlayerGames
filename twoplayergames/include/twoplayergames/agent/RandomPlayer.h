/* *********************************************************** *
 * RandomPlayer.h
 * *********************************************************** */

#ifndef AGENT_RANDOM_PLAYER_H
#define AGENT_RANDOM_PLAYER_H

#include "Agent.h"

namespace play::agent {

template<class GameState, class Move>
class RandomPlayer : public Agent<GameState, Move> {
public:
    std::vector<Move> selectMoves(const GameState& state) override {
        return listLegalMoves(state);
    }
};

}

#endif
