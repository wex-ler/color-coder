#include <string>
#include <map>

namespace ccoder {

    //a parser that looks for newlines and other special characters in text
    //usual workflow:
    //  create parser
    //  parse chunks of text
    //  take back parsed text with clear
    class input_parser {
        private:
            //store parsed text
            std::string buffer;

            //store special characters
            std::map<std::string, char> characters{
                {"\\n", '\n'},
                {"\\t", '\t'},
                {"\\v", '\v'},
                {"\\\\", '\\'},
                {"\\\?", '\?'},
                {"\\\'", '\''},
                {"\\\"", '\"'}
            };

            //match the characters
            //unlike rq_mgr::match this one returns 0 on error
            char match(std::string);

        public:
            //default constructor
            input_parser() = default;

            //same as default but calls parse
            input_parser(std::string);

            //parse some text and add it to buffer
            void parse(std::string);

            //clear the buffer and return it
            std::string clear();
    };
}