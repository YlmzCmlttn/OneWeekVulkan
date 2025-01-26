#pragma once
#include "Window.hpp"
#include "Device.hpp"
#include "Pipeline.hpp"

namespace learnVulkan
{
    class App
    {
    private:
        Window m_Window{WIDTH,HEIGHT,"Hello Vulkan!"};
        Device m_Device{m_Window};
        Pipeline m_Pipeline{m_Device,"../src/shaders/compiled/simple_shader.vert.spv", "../src/shaders/compiled/simple_shader.frag.spv",Pipeline::defaultPipelineConfigInfo(WIDTH,HEIGHT)};
    public:
        App(/* args */) = default;
        App(const App&) = delete;
        App &operator=(const App&)=delete;

        void run();
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
    };    
} // namespace learnVulkan
