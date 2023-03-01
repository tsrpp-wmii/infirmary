
#include "server.h"

#include "fmt/core.h"
#include "fmt/color.h"
#include "fmt/ostream.h"

#include <iostream>

int main()
{
    try
    {
        tsrpp::Server server;
        server.run();
    }
    catch(const std::exception& exception)
    {
        fmt::print(std::cerr, fmt::format(fmt::fg(fmt::color::red), "main::exception {}\n", exception.what()));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}