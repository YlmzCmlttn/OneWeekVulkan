#pragma once
#include "Window.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "GameObject.hpp"
#include "Device.hpp"
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
        Renderer m_Renderer{m_Window,m_Device};
        std::vector<GameObject> m_GameObjects;

        
        void loadGameObjects();
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
