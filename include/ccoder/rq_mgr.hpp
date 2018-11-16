#include <deque>
#include <map>
#include <vector>
#include <string>

#include <windows.h>

namespace ccoder {
    
    class rq_mgr {
        private:
            //stores the default color of the console text
            short default_color;

            //stores the console handle
            HANDLE console_handle;

            //store the messages to print
            std::deque<std::pair<std::string, std::pair<std::vector<short>, std::vector<short>>>> buffer;

            //store colors
            std::map<std::string, short> colors{
                {"b", FOREGROUND_BLUE},
                {"g", FOREGROUND_GREEN},
                {"r", FOREGROUND_RED},

                {"B", BACKGROUND_BLUE},
                {"G", BACKGROUND_GREEN},
                {"R", BACKGROUND_RED}
            };

            //color matching
            //returns default color if not matched
            short match(std::string); //const

            //set color to default
            void set_default() const;

            //set color to requested
            void set_requested(short) const;

            //print to std output stream, uses set_requested and set_default
            void print(std::string&) const;

            //show contents of buffer
            //exists purely for debugging purposes
            void show_buffer() const;

        public:
            //default constructor
            rq_mgr();

            //parse text and prepare it for printing
            void parse(std::string);

            //print all messages in buffer
            void clear();

            //default destructor
            //relies on set_default() being called consistently
            ~rq_mgr();
    };
}