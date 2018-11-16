#include <deque>
#include <map>
#include <vector>
#include <string>

#include <windows.h>

namespace ccoder {
    
    //a request manager to handle messages and to print them
    //usual workflow is
    //  instantiate a class
    //  parse as many messages as you need
    //  print them when you need
    class rq_mgr {
        private:
            //stores the default color of the console text
            short default_color;

            //stores the console handle
            HANDLE console_handle;

            //stores the messages to print
            //deque used because we need fast front popping for printing, and fast back insertion for parsing
            std::deque<std::pair<std::string, std::pair<std::vector<short>, std::vector<short>>>> buffer;

            //stores the colors
            std::map<std::string, short> colors{
                {"b", FOREGROUND_BLUE},
                {"g", FOREGROUND_GREEN},
                {"r", FOREGROUND_RED},

                {"B", BACKGROUND_BLUE},
                {"G", BACKGROUND_GREEN},
                {"R", BACKGROUND_RED}
            };

            //color matching
            //returns default color if not matched..
            //..even tough this messes with debugging sometimes it's way less error prone
            short match(std::string); //const

            //set the color to default
            void set_default() const;

            //set the color to requested
            void set_requested(short) const;

            //print to std output stream, uses set_requested and set_default
            void print(std::string&) const;

        public:
            //default constructor
            //initializes the handle and sets the default color at the time of calling
            rq_mgr();

            //parse text and prepare it for printing by storing it into buffer
            void parse(std::string);

            //print all messages in buffer
            void clear();
    };
}