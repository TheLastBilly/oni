#include <iostream>

#include "target.hpp"

int main(int argc, char *argv[])
{
    oni::target_t target("/Archive/Steam/steamapps/common/Morrowind");
    target.set_data_path("/Archive/Steam/steamapps/common/Morrowind/Data Files");
    target.update();
    std::cout << target.build_json() << "\n";
    return 0;
}