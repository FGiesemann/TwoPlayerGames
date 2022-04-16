/* *********************************************************** *
 * Player.h
 * *********************************************************** */

#ifndef GAMEPLAY_PLAYER_H
#define GAMEPLAY_PLAYER_H

namespace play::game {

class Player {
public:
    constexpr const Player& other() const {
        switch (m_id) {
        case Player1_id: return Player2;
        case Player2_id: return Player1;
        default: return None;
        }
    }

    constexpr Player(const Player& other) : m_id{ other.m_id } {}

    bool operator==(const Player& other) const { return m_id == other.m_id; }
    bool operator!=(const Player& other) const { return !(*this == other); }

    int id() const { return m_id; }
private:
    constexpr Player(int id) : m_id{ id } {};
    int m_id;

    static const int None_id = 0;
    static const int Player1_id = 1;
    static const int Player2_id = 2;
public:
    static const Player None, Player1, Player2;
};

inline constexpr Player Player::None{ Player::None_id };
inline constexpr Player Player::Player1{ Player::Player1_id };
inline constexpr Player Player::Player2{ Player::Player2_id };

}

#endif

