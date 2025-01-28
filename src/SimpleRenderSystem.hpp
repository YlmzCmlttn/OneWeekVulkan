#pragma once

#include "Device.hpp"
#include "GameObject.hpp"
#include "Pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace learnVulkan {
    class SimpleRenderSystem {
    public:
    SimpleRenderSystem(Device &device, VkRenderPass renderPass);
    ~SimpleRenderSystem();

    SimpleRenderSystem(const SimpleRenderSystem &) = delete;
    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

    void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject> &gameObjects);

    private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    Device &m_Device;

    std::unique_ptr<Pipeline> m_Pipeline;
    VkPipelineLayout pipelineLayout;
    };
}  // namespace learnVulkan