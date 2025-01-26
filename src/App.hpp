#pragma once
#include "Window.hpp"
#include "Pipeline.hpp"

namespace learnVulkan
{
    class App
    {
    private:
        Window m_Window{WIDTH,HEIGHT,"Hello Vulkan!"};
        Pipeline pipeline{"../src/shaders/compiled/simple_shader.vert.spv", "../src/shaders/compiled/simple_shader.frag.spv"};
    public:
        App(/* args */) = default;
        App(const App&) = delete;
        App &operator=(const App&)=delete;

        void run();
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
    };    
} // namespace learnVulkan
