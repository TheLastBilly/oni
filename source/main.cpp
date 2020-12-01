#include <iostream>

#include "fs.hpp"
#include "packages.hpp"

int main(int argc, char *argv[])
{
    oni::package::directory_t dir =  oni::package::scan_directory("/home/lastc/Documents/oni/dependencies");
    std::cout << json11::Json(dir).dump() << "\n";
    return 0;
}