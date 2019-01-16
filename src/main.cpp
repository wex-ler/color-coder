#include <iostream>
#include <ccoder/rq_mgr.hpp>
#include <ccoder/input_parser.hpp>

//simple tester program
//enter argument through cmd line or standard input
int main(int argc, char** argv) {
    std::string in;

    if (argc < 2)
        std::cin >> in;
    else
        in = argv[1];
        
    ccoder::input_parser ip(in);

    ccoder::rq_mgr rq;
                   rq.parse(ip.clear());
                   rq.clear();

    //debug me
    asm("nop");
}