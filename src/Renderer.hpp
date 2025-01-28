#pragma once
#include "Window.hpp"
#include "Model.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"


#include <memory>
#include <vector>

namespace learnVulkan
{
    class Renderer
    {
    private:
        Window& m_Window;
        Device& m_Device;
        std::unique_ptr<SwapChain> m_SwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex{0};
        bool isFrameStarted;

            
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();
    public:
        float getAspectRatio() const { return m_SwapChain->extentAspectRatio(); }
        VkRenderPass getSwapChainRenderPass() const { return m_SwapChain->getRenderPass(); }
        bool isFrameInProgress() const { return isFrameStarted; }
        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress");
            return currentFrameIndex;
        }

        Renderer(Window& window,Device& device);
        ~Renderer();
        Renderer(const Renderer&) = delete;
        Renderer &operator=(const Renderer&)=delete;

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    };    
} // namespace learnVulkan
