#include "Pipeline.hpp"
#include "Model.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cassert>
namespace learnVulkan
{
    Pipeline::Pipeline(
            Device& device,
            const std::string& vertexFilePath,
            const std::string& fragmentFilePath,
            const PipelineConfigInfo& configInfo)
        : device{device}
    {
        createGraphicsPipeline(vertexFilePath,fragmentFilePath,configInfo);
    }

    Pipeline::~Pipeline() {
        vkDestroyShaderModule(device.device(), fragShaderModule, nullptr);
        vkDestroyShaderModule(device.device(), vertShaderModule, nullptr);
        vkDestroyPipeline(device.device(), graphicsPipeline, nullptr);
    }

    std::vector<char> Pipeline::readFile(const std::string& filePath){
        std::ifstream file{filePath, std::ios::ate | std::ios::binary};
        if (!file.is_open()) {
            throw std::runtime_error("failed to open file: " + filePath);
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }
    void Pipeline::createGraphicsPipeline(
        const std::string& vertFilepath,
        const std::string& fragFilepath,
        const PipelineConfigInfo& configInfo) {

        // Ensure that the pipelineLayout and renderPass are not null.
        // These are critical for creating a graphics pipeline.
        assert(
            configInfo.pipelineLayout != nullptr &&
            "Cannot create graphics pipeline: no pipelineLayout provided in config info");
        assert(
            configInfo.renderPass != nullptr &&
            "Cannot create graphics pipeline: no renderPass provided in config info");

        // Read the SPIR-V binary code for the vertex and fragment shaders from the file paths provided.
        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);

        // Create Vulkan shader modules for the vertex and fragment shaders.
        createShaderModule(vertCode, &vertShaderModule);
        createShaderModule(fragCode, &fragShaderModule);

        // Define the shader stage for the vertex shader.
        VkPipelineShaderStageCreateInfo shaderStages[2];
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = vertShaderModule;
        shaderStages[0].pName = "main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;
        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = fragShaderModule;
        shaderStages[1].pName = "main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;

        auto bindingDescriptions = Model::Vertex::getBindingDescriptions();
        auto attributeDescriptions = Model::Vertex::getAttributeDescriptions();
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount =
            static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

        vertexInputInfo.vertexAttributeDescriptionCount =static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();


        // Define the graphics pipeline configuration.
        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2; // Two shader stages: vertex and fragment.
        pipelineInfo.pStages = shaderStages; // Shader stage array.

        // Pipeline states provided from configInfo.
        pipelineInfo.pVertexInputState = &vertexInputInfo; // Vertex input state.
        pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo; // Input assembly state.
        pipelineInfo.pViewportState = &configInfo.viewportInfo; // Viewport state.
        pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo; // Rasterization state.
        pipelineInfo.pMultisampleState = &configInfo.multisampleInfo; // Multisampling state.
        pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo; // Depth and stencil state.
        pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo; // Color blending state.
        pipelineInfo.pDynamicState = &configInfo.dynamicStateInfo;

        // Specify pipeline layout and render pass from configInfo.
        pipelineInfo.layout = configInfo.pipelineLayout; // Pipeline layout.
        pipelineInfo.renderPass = configInfo.renderPass; // Render pass.
        pipelineInfo.subpass = configInfo.subpass; // Subpass index.

        // Optional parameters for pipeline derivation (not used here).
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // No base pipeline.
        pipelineInfo.basePipelineIndex = -1; // No base pipeline index.

        // Create the graphics pipeline.
        if (vkCreateGraphicsPipelines(
                device.device(), // Logical device.
                VK_NULL_HANDLE, // Pipeline cache (not used here).
                1, // Number of pipelines to create.
                &pipelineInfo, // Pipeline configuration.
                nullptr, // Custom allocator (optional).
                &graphicsPipeline) != VK_SUCCESS) {
            // Throw an error if pipeline creation fails.
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        // Cleanup: Destroy the shader modules now that the pipeline has been created.
        vkDestroyShaderModule(device.device(), fragShaderModule, nullptr);
        vkDestroyShaderModule(device.device(), vertShaderModule, nullptr);
        fragShaderModule = VK_NULL_HANDLE;
        vertShaderModule = VK_NULL_HANDLE;
    }

    void Pipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data()); //C style array this cast doesn't work.

        if (vkCreateShaderModule(device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module");
        }
    }

    void Pipeline::bind(VkCommandBuffer commandBuffer){
        //IF none setting can be added here.
        vkCmdBindPipeline(commandBuffer,VK_PIPELINE_BIND_POINT_GRAPHICS,graphicsPipeline); //COMPUTE AD RAY_TRACING PIPELINE
    }


    // Function to set up a default pipeline configuration structure for Vulkan.
    // This function initializes various pipeline state structures with default values
    // that can be customized later as needed. It provides a starting point for creating a
    // graphics pipeline.
    void Pipeline::defaultPipelineConfigInfo(PipelineConfigInfo& configInfo) {

        // Input Assembly State: Specifies how input data (vertices) is assembled into primitives.
        configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; 

        //Triangle strip previes two vertices if connected you can use stip this is 
        //Indices
        // Input data will be interpreted as a list of triangles, where every three vertices form one triangle.
        configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE; 
        // Disables restarting primitives, which is used for strip topologies (not relevant for triangle lists).

       
        configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        configInfo.viewportInfo.viewportCount = 1;
        configInfo.viewportInfo.pViewports = nullptr;
        configInfo.viewportInfo.scissorCount = 1;
        configInfo.viewportInfo.pScissors = nullptr;

        // Rasterization State: Configures how geometry is rasterized.
        configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.rasterizationInfo.depthClampEnable = VK_FALSE; 
        // Disable clamping fragments outside the near/far depth range (useful for shadow maps).
        configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE; 
        // Ensure geometry is passed through the rasterization stage (do not discard).
        configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL; 
        // Fill polygons during rasterization (other options: line or point modes).
        configInfo.rasterizationInfo.lineWidth = 1.0f; // Set line width (relevant for VK_POLYGON_MODE_LINE).
        configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE; // Disable face culling.
        configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE; 
        // Clockwise vertices are considered front-facing.
        configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE; 
        // Disable depth biasing (used for shadow mapping).
        configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional: Depth bias constant factor.
        configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional: Maximum depth bias.
        configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional: Slope-based depth bias.

        // Multisample State: Configures anti-aliasing and multisampling.
        configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE; 
        // Disable per-sample shading (anti-aliasing disabled).
        configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; 
        // Single sample per pixel (no multisampling).
        configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional: Minimum fraction of samples for shading.
        configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional: Bitmask of samples to enable.
        configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional: Disable alpha-to-coverage.
        configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional: Disable alpha-to-one conversion.

        // Color Blend Attachment State: Configures how colors are blended.
        configInfo.colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT; 
        // Write all color channels (red, green, blue, alpha).
        configInfo.colorBlendAttachment.blendEnable = VK_FALSE; 
        // Disable blending (colors from the pipeline directly overwrite framebuffer contents).
        configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional: Source color blend factor.
        configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional: Destination color blend factor.
        configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional: Color blend operation.
        configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional: Source alpha blend factor.
        configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional: Destination alpha blend factor.
        configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional: Alpha blend operation.

        // Color Blend State: Configures overall color blending behavior.
        configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configInfo.colorBlendInfo.logicOpEnable = VK_FALSE; 
        // Disable logical operations for blending.
        configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional: Logical operation for blending.
        configInfo.colorBlendInfo.attachmentCount = 1; // Number of attachments (one color attachment in this case).
        configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment; // Reference the color blend attachment.
        configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional: Constant blend factor (red channel).
        configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional: Constant blend factor (green channel).
        configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional: Constant blend factor (blue channel).
        configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional: Constant blend factor (alpha channel).

        // Depth Stencil State: Configures depth and stencil testing.
        configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.depthStencilInfo.depthTestEnable = VK_TRUE; 
        // Enable depth testing (fragments will be compared to depth buffer values).
        configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE; 
        // Enable writing to the depth buffer (updates depth values).
        configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS; 
        // Pass fragments that are closer to the camera.
        configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE; 
        // Disable testing against depth bounds.
        configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional: Minimum depth bounds.
        configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional: Maximum depth bounds.
        configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE; 
        // Disable stencil testing.
        configInfo.depthStencilInfo.front = {};  // Optional: Front-facing stencil operations.
        configInfo.depthStencilInfo.back = {};   // Optional: Back-facing stencil operations.

        configInfo.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
        configInfo.dynamicStateInfo.dynamicStateCount =
            static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
        configInfo.dynamicStateInfo.flags = 0;
    }

} // namespace learnVulkan
