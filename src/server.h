#pragma once

#include <iostream>

#include "tools.hpp"

namespace tsrpp
{
class Server final
{
    NOT_COPYABLE_AND_MOVEABLE(Server);
public:
    Server();
    ~Server();

    void run();
private:
};
}