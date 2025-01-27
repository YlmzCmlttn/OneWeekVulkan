// std
#include <array>
#include "App.hpp"
#include <iostream>
namespace learnVulkan{

    App::App() {
        loadModels();
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    App::~App() {
        vkDestroyPipelineLayout(m_Device.device(), pipelineLayout, nullptr);
        vkFreeCommandBuffers(
            m_Device.device(),
            m_Device.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data());
        commandBuffers.clear();
    }

    void App::run() {
        while (!m_Window.shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(m_Device.device()); //CPU block untill everything finished;
    }

    void App::loadModels() {
        std::vector<Model::Vertex> vertices{{{0.0f, -0.5f}}, {{0.5f, 0.5f}}, {{-0.5f, 0.5f}}};
        m_Model = std::make_unique<Model>(m_Device, vertices);
    }

    // Function to create a pipeline layout in Vulkan.
    // The pipeline layout defines the interface between the application and the pipeline,
    // including descriptor sets and push constants.
    void App::createPipelineLayout() {
        // Define the pipeline layout creation information structure.
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO; 
        // Specifies the type of this structure (mandatory for Vulkan structs).

        pipelineLayoutInfo.setLayoutCount = 0; 
        // Number of descriptor set layouts being used. Here, no descriptor sets are used.

        pipelineLayoutInfo.pSetLayouts = nullptr; 
        // Pointer to the descriptor set layouts (none are provided in this case).

        pipelineLayoutInfo.pushConstantRangeCount = 0; 
        // Number of push constant ranges being defined. Here, push constants are not used.

        pipelineLayoutInfo.pPushConstantRanges = nullptr; 
        // Pointer to an array of push constant ranges (not defined here).

        // Create the pipeline layout using the logical Vulkan device.
        if (vkCreatePipelineLayout(m_Device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS) {
            // Throw an error if pipeline layout creation fails.
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void App::createPipeline() {
        auto pipelineConfig =
            Pipeline::defaultPipelineConfigInfo(m_SwapChain.width(), m_SwapChain.height()); //It importnat to use swapchain width height because like retina display width height may differ pixel resolution
        pipelineConfig.renderPass = m_SwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout; //Multiple render pipeline so different like postprocessing_?
        m_Pipeline = std::make_unique<Pipeline>(
            m_Device,
            "../src/shaders/compiled/simple_shader.vert.spv",
            "../src/shaders/compiled/simple_shader.frag.spv",
            pipelineConfig);
        }

    void App::createCommandBuffers() {
        // Resize the command buffer vector to match the number of swap chain images.
        commandBuffers.resize(m_SwapChain.imageCount());

        // Specify how command buffers will be allocated.
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO; // Specify the type of structure.
        allocInfo.commandPool = m_Device.getCommandPool(); // Command pool from which command buffers are allocated.
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; 
        // PRIMARY: Can be submitted to a queue, cannot be called from other command buffers.
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size()); 
        // Number of command buffers to allocate.

        // Allocate the command buffers.
        if (vkAllocateCommandBuffers(m_Device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        // Record commands into each command buffer.
        for (size_t i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO; // Specify the type of structure.
            beginInfo.flags = 0; // Optional: Flags for controlling how the command buffer is used.
            beginInfo.pInheritanceInfo = nullptr; 
            // Optional: Used only for secondary command buffers.

            // Begin recording commands into the current command buffer.
            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            // Define the render pass information.
            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO; // Specify the type of structure.
            renderPassInfo.renderPass = m_SwapChain.getRenderPass(); // The render pass object to use.
            renderPassInfo.framebuffer = m_SwapChain.getFrameBuffer(i); 
            // The framebuffer corresponding to the current swap chain image.

            renderPassInfo.renderArea.offset = {0, 0}; 
            // Start rendering from the top-left corner.
            renderPassInfo.renderArea.extent = m_SwapChain.getSwapChainExtent(); 
            // Render to the entire swap chain image extent.

            // Specify clear values for the color and depth attachments.
            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f}; 
            // Clear color (dark gray).
            //clearValues[0].depthStencil because frame first is color other depth attachment;
            clearValues[1].depthStencil = {1.0f, 0}; 
            // Clear depth buffer to 1.0 (far) and stencil to 0.
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size()); 
            renderPassInfo.pClearValues = clearValues.data(); // Pointer to the clear values.

            // Begin the render pass.
            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE); 
            // INLINE: Commands are directly recorded in the primary command buffer.

            // Bind the pipeline (shaders and states) to the command buffer.
            m_Pipeline->bind(commandBuffers[i]);

            // Issue a draw command.
            //vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);  //DRAW count instances is also here like particles if you use offset we can use it.
            m_Model->bind(commandBuffers[i]);
            m_Model->draw(commandBuffers[i]);
            // Draw 3 vertices to form a single triangle. No instances are used (instance count = 1).

            // End the render pass.
            vkCmdEndRenderPass(commandBuffers[i]);

            // Finalize the recording of the command buffer.
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }
    }

    void App::drawFrame() {
        
        uint32_t imageIndex;
        auto result = m_SwapChain.acquireNextImage(&imageIndex);
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image");
        }

        result = m_SwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }

}  // names