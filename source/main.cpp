#include <iostream>

#include "fs.hpp"

int main(int argc, char *argv[])
{
    oni::fs::path_t p = "/tmp/test.txt";
    p.update();
    std::cout << p.get_extension() << " is a directory " << std::to_string(p.is_directory()) << "\n";
    return 0;
}