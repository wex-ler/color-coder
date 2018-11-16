#include <iostream>
#include <ccoder/rq_mgr.hpp>

int main(int argc, char** argv) {
    std::string in;

    if (argc < 2)
        std::cin >> in,
        std::cout << in;
    else
        in = argv[1];
        

    ccoder::rq_mgr rq;
                   rq.parse(argv[1]);
                   rq.clear();

    asm("nop");
}