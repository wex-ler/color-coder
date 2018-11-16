#include <iostream>
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
    for (auto i = 0; i < this->buffer.size(); ++i)
        std::cout << "\"" << this->buffer[i].first << "\", color: " << this->buffer[i].second << std::endl;
}

//public methods

void ccoder::rq_mgr::parse(std::string input) {
    std::string::size_type current = 0;

    short last_color = this->default_color;

    for (auto i = 0; i < input.length(); ++i)
        if (input[i] == '/') {

            if (i - current)
                this->buffer.push_back({std::string(input.begin() + current, input.begin() + i), last_color});

            if (input[i + 1] != '/') {
                last_color = this->match(std::string(1, input[i + 1]));

                ++i, ++i;
            } else {
                last_color = this->default_color;

                ++i;
            }

            current = i + 1;
        }

    if (*(input.end() - 1) != '/')
        this->buffer.push_back({std::string(input.begin() + current, input.end()), last_color});    
}

void ccoder::rq_mgr::clear() {
    while (this->buffer.size()) {
        this->set_requested(buffer[0].second);
        this->print(buffer[0].first);

        buffer.pop_front();
    }

    this->set_default();
}