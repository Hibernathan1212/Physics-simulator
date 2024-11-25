//
//  Vulkan.hpp
//  Game
//
//  Created by Nathan Thurber on 17/5/24.
//

#pragma once

//#include "Scene/Scene.hpp"
#include "Scene/Camera.hpp"
#include "Resources/EntityComponentSystem.hpp"

#include "Renderer/ShaderTypes.hpp"

#include "Application/ApplicationSpecification.hpp"


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3native.h>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <array>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "matrix_transform.hpp"
#include "matrix_clip_space.hpp"
#include "string_cast.hpp"

#include <chrono>

class Vulkan
{
public:
    Vulkan(ApplicationSpecification& specification);
    ~Vulkan();
    
    void init();
    void cleanUp();
    
    void render(bool& running, Camera& camera, EntityComponentSystem& ecs);
    const void drawFrame(Camera& camera, EntityComponentSystem& ecs);
    
public:
    inline VkInstance GetInstance() const { return m_Instance; }
    inline VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
    inline uint32_t GetQueueFamily() const { return m_QueueFamily; }
    inline VkDevice GetDevice() const { return m_Device; }
    
public:
    bool g_FrameBufferResized = false;
    
private:
    const void CreateVulkan();
    const void setupPhysicalDevice();
    const void findQueueFamilies();
    const void createLogicalDevice();
    const void createSwapChain();
    const void createImageViews();
    const void createShaderModule();
    const void createDescriptorPool();
    const void createDescriptorSets();
    const void createDescriptorSetLayout();
    const void createGraphicsPipeline();
    const void createRenderPass();
    const void createFramebuffers();
    const void createCommandPool();
    const void createVertexBuffer();
    const void createIndexBuffer();
    const void createUniformBuffers();
    const void createCommandBuffers();
    const void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    const void createSyncObjects();
    
    void createColorResources();
    
    void createDepthResources();
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
    
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat findDepthFormat();
    bool hasStencilComponent(VkFormat format);
    
    const void updateUniformBuffer(uint32_t currentImage, Camera& camera, EntityComponentSystem& ecs);
    
    const void recreateSwapChain();
    const void cleanupSwapChain();
    
    const VkShaderModule createShaderModule(const std::string& filename);
    
    const void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    const void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

    
    struct SwapChainSupportDetails;
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    int rateDeviceSuitability(VkPhysicalDevice device);
    VkSampleCountFlagBits getMaxUsableSampleCount();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void iconifyCallback(GLFWwindow* window, int iconified);
private:
    ApplicationSpecification&           m_Specification;
    
    VkInstance                          m_Instance;
    VkPhysicalDevice                    m_PhysicalDevice;
    uint32_t                            m_QueueFamily;
    VkQueue                             m_GraphicsQueue;
    VkQueue                             m_PresentQueue;
    VkDevice                            m_Device;
    VkAllocationCallbacks*              m_Allocator;
    VkSurfaceKHR                        m_Surface;
    VkSwapchainKHR                      m_SwapChain;
    std::vector<VkImage>                m_SwapChainImages;
    VkFormat                            m_SwapChainImageFormat;
    VkExtent2D                          m_SwapChainExtent;
    std::vector<VkImageView>            m_SwapChainImageViews;
    VkPipelineShaderStageCreateInfo     m_ShaderStages[2];
    VkDescriptorPool                    m_DescriptorPool;
    VkDescriptorSetLayout               m_DescriptorSetLayout;
    std::vector<VkDescriptorSet>        m_DescriptorSets;
    VkPipelineLayout                    m_PipelineLayout;
    VkRenderPass                        m_RenderPass;
    VkPipeline                          m_GraphicsPipeline;
    VkShaderModule                      m_VertShaderModule;
    VkShaderModule                      m_FragShaderModule;
    std::vector<VkFramebuffer>          m_SwapChainFramebuffers;
    VkCommandPool                       m_CommandPool;
    VkBuffer                            m_VertexBuffer;
    VkDeviceMemory                      m_VertexBufferMemory;
    VkBuffer                            m_IndexBuffer;
    VkDeviceMemory                      m_IndexBufferMemory;
    
    std::vector<VkBuffer>               m_UniformBuffers;
    std::vector<VkDeviceMemory>         m_UniformBuffersMemory;
    
    std::vector<VkCommandBuffer>        m_CommandBuffers;
    
    std::vector<VkSemaphore>            m_ImageAvailableSemaphores;
    std::vector<VkSemaphore>            m_RenderFinishedSemaphores;
    std::vector<VkFence>                m_InFlightFences;
    
    VkImage                             m_DepthImage;
    VkDeviceMemory                      m_DepthImageMemory;
    VkImageView                         m_DepthImageView;
    
    VkSampleCountFlagBits               m_MsaaSamples = VK_SAMPLE_COUNT_1_BIT;
    VkImage                             m_ColorImage;
    VkDeviceMemory                      m_ColorImageMemory;
    VkImageView                         m_ColorImageView;
    
    
    UniformData*                        u_Data = nullptr;
    
    bool test = 1;
    int m_ViewportWidth, m_ViewportHeight;
    
private:
    const int                           MAX_FRAMES_IN_FLIGHT = 3;
    uint32_t                            m_CurrentFrame = 0;
    
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;
        
        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            
            return bindingDescription;
        }
        
        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);
            
            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);
            
            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
            
            return attributeDescriptions;
        }
    };
    
    const std::vector<Vertex> vertices = {
        {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{ 1.0f,  1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f,  1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
    };
    
    const std::vector<uint16_t> indices = {
        0, 1, 2,
        2, 3, 0
    };
};
