#include <ccoder/input_parser.hpp>

ccoder::input_parser::input_parser(std::string input) {
    this->parse(input);
}

inline
char ccoder::input_parser::match(std::string input) {
    if (this->characters.find(input) != this->characters.end())
        return this->characters[input];

    return 0;
}

void ccoder::input_parser::parse(std::string input) {
    this->buffer.reserve(input.size());

    std::string::size_type current = 0;

    for (auto i = 0; i < input.length(); ++i) {
        if (input[i] == '\\') {
            if (i - current)
                this->buffer += std::string(input.begin() + current, input.begin() + i);

            char c = this->match(std::string(input.begin() + i, input.begin() + i + 2));

            if (c)
                this->buffer += c;

            ++i, current = i + 1;
        }
    }

    if (current < input.length())
        this->buffer += std::string(input.begin() + current, input.end());

    this->buffer.shrink_to_fit();
}


std::string ccoder::input_parser::clear() {
    std::string retval = this->buffer;
                         this->buffer.clear();

    return retval;
}