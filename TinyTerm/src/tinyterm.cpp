#include "tinyterm.h"
#include <cstring>

// Life Cycles:
void TinyTerm::on_create() {
    Engine::on_create();

    initialize_windows();

    print_to_window(p_m_text_window, Numerics::default_int, Numerics::default_int, Strings::menu);

    print_to_window(p_m_text_window,
                    Numerics::Windows::default_dialog_offset,
                    Numerics::Windows::default_dialog_offset,
                    Strings::press_q_to_quit);

    wrefresh(p_m_main_window);

    on_update();
}

void TinyTerm::on_update() {
    handle_main_window_print();

    for (;;) {
        auto start = std::chrono::system_clock::now();
        if (m_last_key_press == 'q') {
            break;
        }

        if (m_player.moved) {
            handle_main_window_print();
        }

        m_last_key_press = m_player.move_player();
        m_grass.move_rel_player(m_last_key_press);
    }

    Engine::on_destroy();
}

// Utilities:
void TinyTerm::initialize_windows() {
    if (m_debug_mode) {
        m_debug_window_size = {Numerics::Windows::text_window_y, Numerics::Windows::debug_window_x};
        m_text_window_size = {Numerics::Windows::text_window_y, COLS - m_debug_window_size.second};
        m_main_window_size = {LINES - m_text_window_size.first, COLS};

        p_m_main_window = newwin(m_main_window_size.first,
                                 m_main_window_size.second,
                                 Numerics::default_int,
                                 Numerics::default_int);

        p_m_text_window = newwin(m_text_window_size.first,
                                 m_text_window_size.second,
                                 m_main_window_size.first,
                                 Numerics::default_int);

        p_m_debug_window = newwin(m_debug_window_size.first,
                                  m_debug_window_size.second,
                                  m_main_window_size.first,
                                  COLS - m_debug_window_size.second);

        draw_window_border(p_m_debug_window);

    } else {
        m_text_window_size = {Numerics::Windows::text_window_y, COLS};
        m_main_window_size = {LINES - m_text_window_size.first, COLS};

        p_m_main_window = newwin(m_main_window_size.first,
                                 m_main_window_size.second,
                                 Numerics::default_int,
                                 Numerics::default_int);

        p_m_text_window = newwin(m_text_window_size.first,
                                 m_text_window_size.second,
                                 m_main_window_size.first,
                                 Numerics::default_int);
    }

    draw_window_border(p_m_text_window);

}

void TinyTerm::evaluate_ascii_state() {
    for (const auto &[key, val] : m_ascii_state) {
        bool string_exists = std::find(m_printed_ascii_images.begin(), m_printed_ascii_images.end(), key) != m_printed_ascii_images.end();
        if (val && !string_exists) {
            m_printed_ascii_images.push_back(key);
        }
    }
}

void TinyTerm::set_ascii_dimensions(EntityStore::Entity &entity) {
    entity.m_dimensions = read_file_contents(entity.m_ascii);
}

void TinyTerm::set_ascii_center(EntityStore::Entity &entity) {
    entity.m_ascii_center = determine_ascii_center(entity.m_ascii);
}

void TinyTerm::print_ascii(WINDOW *window, int column, int line, EntityStore::Entity &entity, short foreground_color, short background_color) {
    Engine::print_ascii(window, column, line, entity.m_ascii, foreground_color, background_color);
}

void TinyTerm::reset_main_window_state() {
    wclear(p_m_main_window);
}

void TinyTerm::adjust_ascii(EntityStore::Entity &entity) {
    set_ascii_dimensions(entity);
    set_ascii_center(entity);
}

void TinyTerm::handle_main_window_print() {
    reset_main_window_state();

    adjust_ascii(m_player);

    m_player.m_position = {
            (m_grass.m_ascii_center.first + m_grass.m_position.first),
            (m_grass.m_ascii_center.second + m_grass.m_position.second)
    };

    print_ascii(p_m_main_window,m_player.m_position.first, m_player.m_position.second, m_player, COLOR_WHITE, COLOR_BLACK);

    print_circle(p_m_main_window, m_player.m_position.second, m_player.m_position.first, Numerics::default_circle_radius);

    if (m_debug_mode) {
        debug_ascii_loading();
        debug_player_position();
    }
}

// Debug:
void TinyTerm::print_debug(std::vector<std::string> args, int column, int line) {
    int length_of_last_string;

    for (const auto &i : args) {
        print_to_window(p_m_debug_window, column, line, i.c_str());
        length_of_last_string = i.length();
        line += length_of_last_string;
        ++line;
    }

    wrefresh(p_m_debug_window);
}

void TinyTerm::debug_player_position() {
    std::vector<std::string> player_position_text = {
            Strings::player_position,
            "col:", std::to_string(m_player.m_position.first),
            "line:", std::to_string(m_player.m_position.second)
    };

    print_debug(player_position_text, 2, 1);
}

void TinyTerm::debug_ascii_loading() {
    evaluate_ascii_state();

    std::vector<std::string> ascii_text = { Strings::ascii };
    for (auto &i : m_printed_ascii_images) {
        ascii_text.push_back(i);
    }

    print_debug(ascii_text, 3, 1);
}
