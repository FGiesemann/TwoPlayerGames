/* *********************************************************** *
 * Agent.h
 * *********************************************************** */

#ifndef AGENT_AGENT_H
#define AGENT_AGENT_H

#include <vector>

namespace play::agent {

template<class GameState, class Move>
class Agent {
public:
    virtual std::vector<Move> selectMoves(const GameState& state) = 0;
};

}

#endif
