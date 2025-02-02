// std
#include <array>
#include "App.hpp"
#include <iostream>
#include <cassert>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <chrono>
#include <glm/gtc/constants.hpp>
#include "SimpleRenderSystem.hpp"
#include "KeyboardMovementController.hpp"
#include "Camera.hpp"
namespace learnVulkan{
    App::App() {        
        loadGameObjects();
    }

    App::~App() {
    }

    void App::run() {
        SimpleRenderSystem simpleRenderSystem{m_Device,m_Renderer.getSwapChainRenderPass()};
        Camera camera{};
        camera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));

        auto viewerObject = GameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!m_Window.shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime =
                std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(m_Window.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);



            float aspect = m_Renderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

            if(auto commandBuffer = m_Renderer.beginFrame()){
                m_Renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer,m_GameObjects,camera);
                m_Renderer.endSwapChainRenderPass(commandBuffer);
                m_Renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(m_Device.device()); //CPU block untill everything finished;
    }

    std::unique_ptr<Model> createCubeModel(Device& device, glm::vec3 offset) {
        Model::Builder modelBuilder{};
        modelBuilder.vertices = {
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        };
        for (auto& v : modelBuilder.vertices) {
            v.position += offset;
        }

        modelBuilder.indices = {0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                                12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};

        return std::make_unique<Model>(device, modelBuilder);
        }
    // std::unique_ptr<Model> createCubeModel(Device& device, glm::vec3 offset){
        
    //     std::vector<Model::Vertex> vertices{            
    //     // left face (white)
    //     {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
    //     {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
    //     {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
    //     {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
    //     {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
    //     {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
    //     // right face (yellow)
    //     {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
    //     {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
    //     {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
    //     {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
    //     {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
    //     {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
    //     // top face (orange, remember y axis points down)
    //     {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
    //     {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
    //     {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
    //     {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
    //     {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
    //     {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
    //     // bottom face (red)
    //     {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
    //     {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
    //     {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
    //     {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
    //     {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
    //     {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
    //     // nose face (blue)
    //     {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
    //     {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
    //     {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
    //     {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
    //     {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
    //     {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
    //     // tail face (green)
    //     {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    //     {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    //     {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    //     {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    //     {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    //     {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    //     };
    //     for (auto& v : vertices) {
    //         v.position += offset;
    //     }
    //     return std::make_unique<Model>(device, vertices);
    // }
    void App::loadGameObjects() {
        std::shared_ptr<Model> model = createCubeModel(m_Device, {.0f, .0f, .0f});
        auto cube = GameObject::createGameObject();
        cube.model = model;
        cube.transform.translation = {.0f, .0f, 2.5f};
        cube.transform.scale = {.5f, .5f, .5f};
        m_GameObjects.push_back(std::move(cube));
    }
}  // names