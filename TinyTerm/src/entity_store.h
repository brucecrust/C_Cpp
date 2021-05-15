#ifndef ENTITIE_STORE_H
#define ENTITIE_STORE_H

#include <curses.h>
#include <string>

namespace EntityStore {

    // Base:
    class Entity {
    public:
        Entity(std::string ascii, std::pair<int, int> position);

        std::string m_ascii;

        std::pair<int, int> m_position, m_ascii_center, m_dimensions;

        bool moved;

        int move_player();

        void move_rel_player(int last_key_press);
    };

    // Player:
    class Player : public Entity {
    public:
        Player(std::string ascii, std::pair<int, int> position) : Entity(ascii, position) {}
    };

    // Entity 
    class Grass : public Entity {
    public:
        Grass(std::string ascii, std::pair<int, int> position) : Entity(ascii, position) {}
    };

    class Shadow : public Entity {
    public:
        Shadow(std::string ascii, std::pair<int, int> position) : Entity(ascii, position) {}
    };

}

#endif
