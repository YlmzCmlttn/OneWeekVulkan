// std
#include <array>
#include "App.hpp"
#include <iostream>
#include <cassert>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <glm/gtc/constants.hpp>
#include "SimpleRenderSystem.hpp"
namespace learnVulkan{
    App::App() {        
        loadGameObjects();
    }

    App::~App() {
    }

    void App::run() {
        SimpleRenderSystem simpleRenderSystem{m_Device,m_Renderer.getSwapChainRenderPass()};
        while (!m_Window.shouldClose()) {
            glfwPollEvents();
            if(auto commandBuffer = m_Renderer.beginFrame()){
                m_Renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer,m_GameObjects);
                m_Renderer.endSwapChainRenderPass(commandBuffer);
                m_Renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(m_Device.device()); //CPU block untill everything finished;
    }

    void App::loadGameObjects() {
        //std::vector<Model::Vertex> vertices{{{0.0f, -0.5f}}, {{0.5f, 0.5f}}, {{-0.5f, 0.5f}}};
        std::vector<Model::Vertex> vertices{
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
        auto model = std::make_shared<Model>(m_Device, vertices);
        auto triangle = GameObject::createGameObject();
        triangle.model = model;
        triangle.color = {.1f, .8f, .1f};
        triangle.transform2d.translation.x = .2f;
        triangle.transform2d.scale = {2.f, .5f};
        triangle.transform2d.rotation = .25f * glm::two_pi<float>();
        m_GameObjects.push_back(std::move(triangle));
    }
}  // names