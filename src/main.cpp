#include <iostream>

#include "server.h"

int main()
{
    try
    {
        tsrpp::Server server;
        server.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}