#include "App.hpp"

namespace learnVulkan
{
    void App::run(){
        while(!m_Window.shouldClose()){
            glfwPollEvents();
        }
    }    
} // namespace learnVulkan
