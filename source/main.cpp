#include <iostream>

#include "fs.hpp"

int main(int argc, char *argv[])
{
    for(oni::fs::path_t &p : oni::fs::get_subpaths_recursively("/home/lastc/Documents/coenpra"))
    {
        std::cout << p.get_path() << "\n";
    }
    return 0;
}