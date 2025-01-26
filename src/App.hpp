#pragma once
#include "Window.hpp"
#include "Device.hpp"
#include "Pipeline.hpp"
#include "SwapChain.hpp"

#include <memory>
#include <vector>

namespace learnVulkan
{
    class App
    {
    private:
        Window m_Window{WIDTH,HEIGHT,"Hello Vulkan!"};
        Device m_Device{m_Window};
        SwapChain m_SwapChain{m_Device,m_Window.getExtent()};
        std::unique_ptr<Pipeline> m_Pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        //Pipeline m_Pipeline{m_Device,"../src/shaders/compiled/simple_shader.vert.spv", "../src/shaders/compiled/simple_shader.frag.spv",Pipeline::defaultPipelineConfigInfo(WIDTH,HEIGHT)};

        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();
    public:
        App(/* args */);
        ~App();
        App(const App&) = delete;
        App &operator=(const App&)=delete;

        void run();
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
    };    
} // namespace learnVulkan
