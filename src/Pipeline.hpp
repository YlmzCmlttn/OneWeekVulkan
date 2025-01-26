#pragma once
#include <string>
#include <vector>

#include "Device.hpp"

namespace learnVulkan
{
    struct PipelineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class Pipeline
    {
    private:
        static std::vector<char> readFile(const std::string& filePath);
    public:
        Pipeline(
            Device& device,
            const std::string& vertexFilePath,
            const std::string& fragmentFilePath,
            const PipelineConfigInfo& configInfo);

        ~Pipeline();

        Pipeline(const Pipeline&) = delete;
        void operator=(const Pipeline&) = delete;
        
        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

        void createGraphicsPipeline(
            const std::string& vertexFilePath,
            const std::string& fragmentFilePath,
            const PipelineConfigInfo& configInfo);
        
        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        Device& device; //aggregation ??
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };    
} // namespace learnVulkan
