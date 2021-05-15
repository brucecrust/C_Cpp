#include "engine.h"
// TODO: Remove for curses or remove methods
#include <iostream>

// Engine Lifecycle:
void Engine::Engine::on_create() {
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    refresh();
}

void Engine::Engine::on_update() {
    for (;;) {
        break;
    }
    on_destroy();
}

void Engine::Engine::on_destroy() {
    endwin();
    delwin(stdscr);
}

Engine::Engine::Engine() {}

// Utilities:

void Engine::Engine::draw_window_border(WINDOW *p_window) {
    wborder(p_window,
            Numerics::default_int,
            Numerics::default_int,
            Numerics::default_int,
            Numerics::default_int,
            Numerics::default_int,
            Numerics::default_int,
            Numerics::default_int,
            Numerics::default_int);

    wrefresh(p_window);
}

void Engine::Engine::print_to_window(WINDOW *window, int column, int line, const char *text) {
    mvwprintw(window, column, line, text);
    wrefresh(window);
}

std::vector<std::string> Engine::Engine::get_file_contents(std::string file_name) {
    std::vector<std::string> contents;
    std::ifstream reader(file_name);
    std::string file_name_no_back_path = file_name.erase(0, Numerics::back_path_length);

    if (!reader.good()) {
        m_ascii_state[file_name_no_back_path] = false;
    }

    while (reader.good()) {
        m_ascii_state[file_name_no_back_path] = true;
        std::string buffer;
        std::getline(reader, buffer);
        contents.push_back(buffer);
    }

    reader.close();

    return contents;
}

std::pair<int, int> Engine::Engine::read_file_contents(std::string file_name) {
    std::vector<std::string> contents;
    std::pair<int, int> dimensions = { 0, 0 };
    std::ifstream reader(Strings::back_path + file_name);

    while (reader.good()) {
        std::string buffer;
        std::getline(reader, buffer);
        dimensions.first += 1;
        dimensions.second += buffer.length();
    }

    dimensions.second /= dimensions.first;

    return dimensions;
}

void Engine::Engine::print_ascii(WINDOW *window, int column, int line, const std::string& file_name, short foreground_color, short background_color) {
    // Since the executable runs from a build directory, we need to back up and provide an appropriate path.

    std::vector<std::string> ascii = get_file_contents(Strings::back_path + file_name);

    ++m_default_color_pair;
    init_pair(m_default_color_pair, foreground_color, background_color);

    for (auto &i : ascii) {
        wattron(window, COLOR_PAIR(m_default_color_pair));
        mvwprintw(window, column, line, i.c_str());
        wattroff(window, COLOR_PAIR(m_default_color_pair));
        ++column;
    }

    wrefresh(window);
}

void Engine::Engine::print_circle(WINDOW *window, int h, int k, int radius) {
    int circumference = radius * 2 * M_PI;

    double x, y;
    for (int i = 0; i < circumference; ++i) {
        x = radius * cos(i * 2 * M_PI / circumference) + h;
        y = radius * sin(i * 2 * M_PI / circumference) + k;
        mvwprintw(window, ceil(y), ceil(x), ".");
    }

    wrefresh(window);
}

int Engine::Engine::determine_median(int n) {
    return (n + 1) / 2;
}

std::pair<int, int> Engine::Engine::determine_ascii_center(std::string file_name) {
    auto dimensions = read_file_contents(file_name);
    return std::pair<int, int> { determine_median(dimensions.first), determine_median(dimensions.second) };
}

// Deprecated:
void Engine::modify_2D_vector_at_position(std::vector<std::vector<char>> &p_matrix, std::pair<std::size_t, std::size_t> p_coordinates, char p_sprite) {
    if (p_coordinates.first > p_matrix.size()) return;
    if (p_coordinates.second > p_matrix[p_coordinates.first].size()) return;
    p_matrix[p_coordinates.first][p_coordinates.second] = p_sprite;
}

// TODO: Allow for users to enter 'q' to quit this loop. 
// TODO: Ignore casing on user input.
std::string Engine::handle_user_input(std::vector<std::string> p_acceptable_input, bool p_verify_input) {
    //print_string_vector(p_acceptable_input);

    std::string raw_input;

    std::cin >> raw_input;

    std::vector<std::string> input;

    // For each string in the provided answer vector, check if the user's input matches an option in the vector.
    for (std::string i : p_acceptable_input) {
        if (std::find(p_acceptable_input.begin(), p_acceptable_input.end(), i) != p_acceptable_input.end()) {
            if (i.rfind(raw_input, 0) == 0) input.push_back(i);
        }
    }

    if (input.empty()) {
        std::cout << Strings::invalid_input << "\n";
        handle_user_input(p_acceptable_input);
    }

    if (input.size() == 1) {
        if (p_verify_input) {
            std::cout << Strings::did_you_choose << input[0] << "\n";

            if (handle_user_input(std::vector<std::string> {Strings::yes, Strings::no}, false) == "Yes") {
                return input[0];
            }

            handle_user_input(p_acceptable_input);
        }

        return input[0];
    }

    std::cout << Strings::which_of_the_following << "\n";
    handle_user_input(input);

    return Strings::empty;
}
