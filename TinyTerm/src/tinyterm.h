#ifndef TINYTERM_H
#define TINYTERM_H

#include "engine.h"
#include "entity_store.h"
#include <cstdarg>

class TinyTerm : public Engine::Engine {

public:
    TinyTerm(bool debug_mode) : Engine::Engine() { m_debug_mode = debug_mode; }

    // Members:
    bool m_debug_mode;

    EntityStore::Player m_player = EntityStore::Player("player.txt", std::pair<int, int> { 17, 27 });
    EntityStore::Grass m_grass = EntityStore::Grass("grass.txt", std::pair{ 0,0 });

    std::pair<int, int> m_main_window_size, m_text_window_size, m_debug_window_size;

    WINDOW *p_m_main_window, *p_m_text_window, *p_m_debug_window;

    std::vector<std::string> m_printed_ascii_images;

    // Life Cycles:
    void on_create() override;

    void on_update() override;

    // Utilities:
    void initialize_windows();

    void evaluate_ascii_state();

    void reset_main_window_state();

    void set_ascii_dimensions(EntityStore::Entity &entity);

    void set_ascii_center(EntityStore::Entity &entity);

    void print_ascii(WINDOW *window, int column, int line, EntityStore::Entity &entity, short foreground_color, short background_color);

    void adjust_ascii(EntityStore::Entity &entity);

    void handle_main_window_print();

    // Debug:
    void print_debug(std::vector<std::string> args, int column, int line);

    void debug_player_position();

    void debug_ascii_loading();
};

#endif
