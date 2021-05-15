#include <vector>
#include "entity_store.h"

EntityStore::Entity::Entity(std::string ascii, std::pair<int, int> position)  {
    m_ascii = ascii;
    m_position = position;

    moved = false;
}

int EntityStore::Entity::move_player() {
    moved = false;

    int ch = getch();

    std::vector<int> allowed_keys = { 'w', 'a', 's', 'd' };

    if (ch == 'w') m_position.first -= 1;
    if (ch == 's') m_position.first += 1;
    if (ch == 'a') m_position.second -= 1;
    if (ch == 'd') m_position.second += 1;

    for (int &i : allowed_keys) {
        if (ch == i) {
            moved = true;
        }
    }

    return ch;
}

void EntityStore::Entity::move_rel_player(int last_key_press) {
    if (last_key_press == 'w') m_position.first -= 1;
    if (last_key_press == 's') m_position.first += 1;
    if (last_key_press == 'a') m_position.second -= 1;
    if (last_key_press == 'd') m_position.second += 1;
}