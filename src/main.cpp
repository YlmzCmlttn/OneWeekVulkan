#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "App.hpp"

int main() {
    learnVulkan::App app{};
    try
    {
        app.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return 0;
}