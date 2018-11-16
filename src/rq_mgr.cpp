#include <iostream>
#include <cctype>
#include <ccoder/rq_mgr.hpp>

//private methods, self explanatory / undocumented

inline
short ccoder::rq_mgr::match(std::string color) /*const*/ {
    //this way of "doing things" prevented me from declaring this function as const
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

//public methods, "documented"

//initialize variables
ccoder::rq_mgr::rq_mgr() {
    //get the output handle, aka the console handle
    this->console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    //variable to store the buffer info from which we get the default console color
    CONSOLE_SCREEN_BUFFER_INFO temp_info;

    //if you cant get info from the handle, exit / throw
    if (!GetConsoleScreenBufferInfo(this->console_handle, &temp_info))
        throw std::runtime_error("Unable to get console screen buffer info!");

    //color is one of the bugger info attributes
    this->default_color = temp_info.wAttributes;
}

//parse message and store tokens with color specifications
void ccoder::rq_mgr::parse(std::string input) {
    //starting position of the string token
    std::string::size_type current = 0;

    //store the colors for the current string token
    //foreground is initilaized because the text can start without color specification
    std::vector<short> foreground_colors{this->default_color},
                       background_colors;

    //iterate over string
    for (auto i = 0; i < input.length(); ++i)
        //if you run into a color specification
        if (input[i] == '/') {

            //if there is text to print
            if (i - current)
                //push it to buffer, with according color specifications
                //the string token begins at the first letter following previous specifications and ends with last letter before this specification
                this->buffer.push_back({std::string(input.begin() + current, input.begin() + i), {foreground_colors, background_colors}});

            //clear the storage for next specification
            foreground_colors.clear();
            background_colors.clear();

            //if specification exists
            if (input[i + 1] != '/') {
                //skip '/'
                ++i;

                //while there are colors to be read, note i++
                while (input[i] != '/' && input[i] != ':')
                    foreground_colors.push_back(this->match(std::string(1, input[i++])));

                //if you stopped on separator rather than closing / read background colors
                if (input[i] == ':') {
                    //skip ':'
                    ++i;

                    //while there are colors to be read, note i++
                    while (input[i] != '/')
                        background_colors.push_back(this->match(std::string(1, std::toupper(input[i++]))));
                }
            //if there is no colors, yet an empty specification '//'
            } else {
                //push default color
                foreground_colors.push_back(this->default_color);

                //move to the end '/'
                ++i;
            }

            //move to the letter after '/'
            current = i + 1;
        }

    //put the string token with previous color specification in the buffer even if you didnt encounter an ending specification
    if (*(input.end() - 1) != '/')
        this->buffer.push_back({std::string(input.begin() + current, input.end()), {foreground_colors, background_colors}});    
}

//print tokens
void ccoder::rq_mgr::clear() {
    //while there are tokens to print
    while (this->buffer.size()) {
        //initialize colors
        //notice that this relies on parse() to set default color..
        //..of a token when the specification is empty
        short fg = 0,
              bg = 0;

        //mix the foreground colors
        for (auto color : this->buffer[0].second.first)
            fg |= color;

        //mix the background colors
        for (auto color : this->buffer[0].second.second)
            bg |= color;

        //requested color is a mix between foreground and background colors
        this->set_requested(fg | bg);

        //print
        this->print(this->buffer[0].first);

        //get rid of token
        buffer.pop_front();
    }

    //set the colors to default
    this->set_default();
}