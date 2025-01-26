#include "Pipeline.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
namespace learnVulkan
{
    Pipeline::Pipeline(const std::string& vertexFilePath,const std::string& fragmentFilePath)
    {
        createGraphicsPipeline(vertexFilePath,fragmentFilePath);
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
    void Pipeline::createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath) {
        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);

        std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
        std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
    }
} // namespace learnVulkan
