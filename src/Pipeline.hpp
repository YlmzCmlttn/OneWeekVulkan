#pragma once
#include <string>
#include <vector>

namespace learnVulkan
{
    class Pipeline
    {
    private:
        static std::vector<char> readFile(const std::string& filePath);
    public:
        Pipeline(const std::string& vertexFilePath,const std::string& fragmentFilePath);        
        void createGraphicsPipeline(const std::string& vertexFilePath,const std::string& fragmentFilePath);
    };    
} // namespace learnVulkan
