#pragma once
#include "Window.hpp"

namespace learnVulkan
{
    class App
    {
    private:
        Window m_Window{WIDTH,HEIGHT,"Hello Vulkan!"};
    public:
        App(/* args */) = default;
        App(const App&) = delete;
        App &operator=(const App&)=delete;

        void run();
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
    };    
} // namespace learnVulkan
