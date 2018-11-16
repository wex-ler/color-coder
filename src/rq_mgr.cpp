#include <iostream>
#include <cctype>
#include <ccoder/rq_mgr.hpp>

//constructors

ccoder::rq_mgr::rq_mgr() {
    this->console_handle         = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO temp_info;

    if (!GetConsoleScreenBufferInfo(this->console_handle, &temp_info))
        throw std::runtime_error("Unable to get console screen buffer info!");

    this->default_color = temp_info.wAttributes;
}

//destructor

ccoder::rq_mgr::~rq_mgr() {
    
}

//private methods

inline
short ccoder::rq_mgr::match(std::string color) /*const*/ {
    if (this->colors.find(color) != this->colors.end())
        return this->colors[color];

    return this->default_color;
}

inline
void ccoder::rq_mgr::set_default() const {
    SetConsoleTextAttribute(this->console_handle, this->default_color);
}

inline
void ccoder::rq_mgr::set_requested(short requested_color) const {
    SetConsoleTextAttribute(this->console_handle, requested_color);
}

inline
void ccoder::rq_mgr::print(std::string& output) const {
    std::cout << output;
}

inline
void ccoder::rq_mgr::show_buffer() const {

}

//public methods

void ccoder::rq_mgr::parse(std::string input) {
    std::string::size_type current = 0;

    std::vector<short> foreground_colors{this->default_color},
                       background_colors;

    for (auto i = 0; i < input.length(); ++i)
        if (input[i] == '/') {

            if (i - current) {
                this->buffer.push_back({std::string(input.begin() + current, input.begin() + i), {foreground_colors, background_colors}});

                foreground_colors.clear();
                background_colors.clear();
            }

            if (input[i + 1] != '/') {
                ++i;

                if (foreground_colors.size() || background_colors.size())
                    foreground_colors.clear(), background_colors.clear();

                while (input[i] != '/' && input[i] != ':')
                    foreground_colors.push_back(this->match(std::string(1, input[i++])));

                if (input[i] == ':') {
                    ++i;

                    while (input[i] != '/')
                        background_colors.push_back(this->match(std::string(1, std::toupper(input[i++]))));
                }
            } else {
                foreground_colors.push_back(this->default_color);

                ++i;
            }

            current = i + 1;
        }

    if (*(input.end() - 1) != '/')
        this->buffer.push_back({std::string(input.begin() + current, input.end()), {foreground_colors, background_colors}});    
}

void ccoder::rq_mgr::clear() {
    while (this->buffer.size()) {
        short fg = 0,
              bg = 0;

        for (auto color : this->buffer[0].second.first)
            fg |= color;

        for (auto color : this->buffer[0].second.second)
            bg |= color;

        this->set_requested(fg | bg);

        this->print(this->buffer[0].first);

        buffer.pop_front();
    }

    this->set_default();
}