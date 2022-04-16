/* *********************************************************** *
 * InteractiveAgent.h
 * *********************************************************** */

#ifndef AGENT_INTERACTIVE_PLAYER_H
#define AGENT_INTERACTIVE_PLAYER_H

#include "Agent.h"

namespace play::agent {

template<class GameState, class Move>
class InteractivePlayer : public Agent<GameState, Move> {
public:
    std::vector<Move> selectMoves(const GameState& state) override {
        std::vector<Move> resultingMove;
        while (true) {
            const Move move = askForMove(state);
            if (isLegalMove(move, state)) {
                resultingMove.push_back(move);
                break;
            }
        }
        return resultingMove;
    }
};

}

#endif
