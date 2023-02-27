#include "tools.hpp"

#include "SQLiteCpp/SQLiteCpp.h"

#include <iostream>

namespace tsrpp
{
class Database final
{
    NOT_COPYABLE_AND_MOVEABLE(Database);
public:
    Database();
    ~Database() = default;
};
}