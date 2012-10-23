#include <iostream>
#include "module_map.h"

int
main(int argc, char **argv)
{
    ModuleMap modmap;
    fill_map(modmap);
    std::cout << modmap.size() << " functions registered." << std::endl;
    return 0;
}
