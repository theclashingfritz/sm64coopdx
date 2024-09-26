#include "gfx_cc.h"
#include "gfx_pc.h"
#include "gfx_rendering_api.h"
#include "gfx_screen_config.h"
#include "gfx_vulkan.h"
#include "gfx_window_manager_api.h"

#include <PR/gbi.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <limits>
#include <set>
#include <vector>

#include <vulkan/vulkan.h>

#ifdef __MINGW32__
# define FOR_WINDOWS 1
#else
# define FOR_WINDOWS 0
#endif

#if defined(CAPI_SDL2)

#if FOR_WINDOWS
#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengl.h>
#else
#include <SDL2/SDL.h>
#define GL_GLEXT_PROTOTYPES 1

#ifdef OSX_BUILD
#include <SDL2/SDL_opengl.h>
#else
#include <SDL2/SDL_opengles2.h>
#endif

#endif // End of OS-Specific GL defines

#endif

struct VulkanSwapChainDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

struct VulkanFamilyIndex {
   uint32_t value;
   VkBool32 has_value;
};

struct VulkanFamilyIndices {
    VulkanFamilyIndex graphics_family;
    VulkanFamilyIndex present_family;
};

struct VulkanSwapChain {
    VkFormat format;
    VkExtent2D extent;
    VkSwapchainKHR chain;
    VkPresentModeKHR mode;
    std::vector<VkImage> images;
    std::vector<VkImageView> image_views;
}

struct Vulkan {
    VkApplicationInfo app_info;
    VkInstance instance;
    VkPhysicalDevice phys_device;
    VkDevice logic_device;
    VkSurfaceKHR surface;
    VkViewport viewport;
    VulkanSwapChain swap_chain;
    
    VkQueue graphics_queue;
    VkQueue present_queue;
}

static Vulkan vulkan{0};
vulkan.phys_device = VK_NULL_HANDLE;
vulkan.logic_device = VK_NULL_HANDLE;

static bool gfx_vulkan_z_is_from_0_to_1(void) {
    return true;
}

static void gfx_vulkan_unload_shader(struct ShaderProgram *old_prg) {
}

static void gfx_vulkan_load_shader(struct ShaderProgram *new_prg) {
}

static struct ShaderProgram *gfx_vulkan_create_and_load_new_shader(struct ColorCombiner* cc) {
    return NULL;
}

static struct ShaderProgram *gfx_vulkan_lookup_shader(struct ColorCombiner* cc) {
    return NULL;
}

static void gfx_vulkan_shader_get_info(struct ShaderProgram *prg, uint8_t *num_inputs, bool used_textures[2]) {
    *num_inputs = prg->num_inputs;
    used_textures[0] = prg->used_textures[0];
    used_textures[1] = prg->used_textures[1];
}

static uint32_t gfx_vulkan_new_texture() {
}

static void gfx_vulkan_select_texture(int tile, uint32_t texture_id) {
}

static void gfx_vulkan_upload_texture(const uint8_t *rgba32_buf, int width, int height) {
}

static void gfx_vulkan_set_sampler_parameters(int tile, bool linear_filter, uint32_t cms, uint32_t cmt) {
}

static void gfx_vulkan_set_depth_test(bool depth_test) {
}

static void gfx_vulkan_set_depth_mask(bool z_upd) {
}

static void gfx_vulkan_set_zmode_decal(bool zmode_decal) {
}

static void gfx_vulkan_set_viewport(int x, int y, int width, int height) {
}

static void gfx_vulkan_set_scissor(int x, int y, int width, int height) {
}

static void gfx_vulkan_set_use_alpha(bool use_alpha) {
}

static void gfx_vulkan_draw_triangles(float buf_vbo[], size_t buf_vbo_len, size_t buf_vbo_num_tris) {
}

static void vulkan_list_extensions() {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    
    std::vector<VkExtensionProperties> extensions(extensionCount);
    
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    
    std::cout << "Available Vulkan Extensions:\n";

    for (const auto& extension : extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }
}

static VulkanFamilyIndices vulkan_find_queue_indicies(VkPhysicalDevice device) {
    VulkanFamilyIndices indicies{0};
    
    {
        uint32_t count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

        std::vector<VkQueueFamilyProperties> families(count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, families.data());
        
        uint32_t i = 0;
        for (const auto &family : families) {
            if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indicies.graphics_family.value = i;
                indicies.graphics_family.has_value = true;
            }
            
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vulkan.surface, &indicies.present_family.has_value);
            if (indicies.present_family.has_value) {
                indices.present_family.value = i;
            }

            i++;
        }
    }
    
    return indicies;
}

static VulkanSwapChainDetails vulkan_query_swap_chain_support(VkPhysicalDevice device) {
    VulkanSwapChainDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
    
    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }
    
    uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.present_modes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.present_modes.data());
    }
    
    return details;
}

const std::vector<const char *> required_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

static bool vulkan_device_supports_extensions(VkPhysicalDevice device) {
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
    
    std::set<std::string> requiredExtensions(required_extensions.begin(), required_extensions.end());
    
    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

static bool vulkan_is_device_suitable(VkPhysicalDevice device) {
    //VkPhysicalDeviceProperties deviceProperties;
    //VkPhysicalDeviceFeatures deviceFeatures;
    
    //vkGetPhysicalDeviceProperties(device, &deviceProperties);
    //vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    
    // Get the family indicies for the device.
    VulkanFamilyIndices indices = vulkan_find_queue_indicies(device);
    
    bool families_supported = indices.graphics_family.has_value && indicies.present_family.has_value;
    bool extensions_supported = vulkan_device_supports_extensions(device);
    
    bool swap_chains_supported = false;
    if (extensions_supported) {
        VulkanSwapChainDetails support = vulkan_query_swap_chain_support(device);
        swap_chains_supported = !support.formats.empty() && !support.present_modes.empty();
    }
    
    // Return if our device is supported.
    return families_supported && extensions_supported && swap_chains_supported;
}

static VkDevice vulkan_create_logic_device(VkPhysicalDevice device, float queuePriority) {
    VulkanFamilyIndices indices = vulkan_find_queue_indicies(device);
    
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphics_family.value, indices.present_family.value};
    
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }
    
    VkPhysicalDeviceFeatures deviceFeatures{};
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.ppEnabledExtensionNames = required_extensions.data();
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
    
    createInfo.enabledExtensionCount = (uint32_t)required_extensions.size();
    createInfo.enabledLayerCount = 0;
    
    VkDevice newDevice = VK_NULL_HANDLE;
    if (vkCreateDevice(device, &createInfo, nullptr, &newDevice) != VK_SUCCESS) {
        sys_fatal("Failed to make Vulkan Logic Device!");
    }
    
    return newDevice;
}

static void vulkan_create_instance() {
    // Create our overall Vulkan instance.

    vulkan.app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    vulkan.app_info.pApplicationName = "sm64coopdx";
    vulkan.app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    vulkan.app_info.pEngineName = "Coop Engine";
    vulkan.app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    vulkan.app_info.apiVersion = VK_API_VERSION_1_0;
    
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    vulkan_list_extensions();
    
    std::vector<const char *> extensions;

#if defined(CAPI_SDL2)
    SDL_Window *window = gfx_sdl_get_window();
    
    // Get the amount of extensions we need to allocate for.
    unsigned int sdlext_count = 0;
    SDL_Vulkan_GetInstanceExtensions(window, &sdlext_count, NULL);
    
    // Allocate the memory needed for the extension names and get them.
    const char **sdlext_names = new const char *[sdlext_count + 1];
    sdlext_names[sdlext_count] = 0; // Safety first!
    SDL_Vulkan_GetInstanceExtensions(window, &sdlext_count, &sdlext_names);
    
    // Add all of our SDL extensions!
    for (unsigned int i = 0; i < sdlext_count; i++) {
        extensions.emplace_back(sdlext_names[i]);
    }
    
    // Free the temporary name holding array.
    delete[] sdlext_names;
#endif
    
#if defined(OSX_BUILD)
    extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    createInfo.enabledExtensionCount = (uint32_t)extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();
    
    createInfo.enabledLayerCount = 0;
    
    if (vkCreateInstance(&createInfo, nullptr, &vulkan.instance) != VK_SUCCESS) {
        sys_fatal("Failed to initalize Vulkan.");
    }
}

static void vulkan_create_surface() {
    // This will be different depending on the WindowApi.
    // We currently only support SDL2.
#if defined(CAPI_SDL2)
    SDL_Window *window = gfx_sdl_get_window();
    SDL_Vulkan_CreateSurface(window, vulkan.instance, &vulkan.surface);
#endif
}

static void vulkan_select_display_device() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vulkan.instance, &deviceCount, nullptr);
    if (deviceCount == 0) { sys_fatal("Failed to initalize Vulkan device."); }
    
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vulkan.instance, &deviceCount, devices.data());
    
    for (const auto& device : devices) {
        if (vulkan_is_device_suitable(device)) {
            vulkan.phys_device = device;
            break;
        }
    }
    
    if (vulkan.phys_device == VK_NULL_HANDLE) { sys_fatal("Failed to initalize Vulkan device."); }
    
    vulkan.logic_device = vulkan_create_logic_device(vulkan.phys_device, 1.0);
    
    VulkanFamilyIndices indices = vulkan_find_queue_indicies(vulkan.phys_device);
    vkGetDeviceQueue(vulkan.logic_device, indices.graphics_family.value, 0, &vulkan.graphics_queue);
    vkGetDeviceQueue(vulkan.logic_device, indices.present_family.value, 0, &vulkan.present_queue);
}

static VkSurfaceFormatKHR vulkan_get_swapchain_format(const std::vector<VkSurfaceFormatKHR> &formats) {
    // Admittably enough, There is ALOT of formats to choose from.
    // You can see them all here: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkFormat.html
    // and here: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkColorSpaceKHR.html
    // We'll just adjust later on.
    
    for (const auto &surface_format : formats) {
        if (surface_format.format == VK_FORMAT_B8G8R8A8_SRGB && surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return surface_format;
        }
    }
    
    // Return the default.
    return formats[0];
}

static VkPresentModeKHR vulkan_get_swapchain_present_mode(const std::vector<VkPresentModeKHR> &modes) {
    for (const auto &mode : modes) {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return mode;
        }
    }
    
    return VK_PRESENT_MODE_FIFO_KHR;
}

static VkExtent2D vulkan_get_swap_extent(const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    
    VkExtent2D extent = { 0, 0 };
    
#if defined(CAPI_SDL2)
    int width = 0;
    int height = 0;
    
    SDL_Window *window = gfx_sdl_get_window();
    SDL_GetWindowSize(window, &width, &height);
    
    extent.width = std::clamp((uint32_t)width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    extent.height = std::clamp((uint32_t)height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
#endif

    return extent;
}

static void vulkan_create_swap_chains(void) {
    VulkanSwapChain &swap_chain = vulkan.swap_chain;
    VulkanSwapChainDetails details = vulkan_query_swap_chain_support(vulkan.phys_device);
    
    swap_chain.format = vulkan_get_swapchain_format(details.formats);
    swap_chain.mode = vulkan_get_swapchain_present_mode(details.present_modes);
    swap_chain.extent = vulkan_get_swap_extent(details.capabilities);
    
    uint32_t imageCount = details.capabilities.minImageCount + 1;
    if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount) {
        imageCount = details.capabilities.maxImageCount;
    }
    
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = vulkan.surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = swap_chain.format.format;
    createInfo.imageColorSpace = swap_chain.format.colorSpace;
    createInfo.imageExtent = swap_chain.extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
    VulkanFamilyIndices indices = vulkan_find_queue_indicies(vulkan.phys_device);
    uint32_t familyIndices[] = { indices.graphics_family.value, indices.present_family.value };
    
    if (indices.graphics_family != indices.present_family) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = familyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }
    
    createInfo.preTransform = details.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = swap_chain.mode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    
    if (vkCreateSwapchainKHR(vulkan.logic_device, &createInfo, nullptr, &swap_chain.chain) != VK_SUCCESS) {
        sys_fatal("Failed to create swap chain for Vulkan!");
    }
    
    vkGetSwapchainImagesKHR(vulkan.logic_device, swap_chain.chain, &imageCount, nullptr);
    swap_chain.images.resize(imageCount);
    vkGetSwapchainImagesKHR(vulkan.logic_device, swap_chain.chain, &imageCount, swap_chain.images.data());
}

static void vulkan_create_image_views(void) {
    VulkanSwapChain &swap_chain = vulkan.swap_chain;
    
    swap_chain.image_views.resize(swap_chain.images.size());
    for (size_t i = 0; i < swap_chain.images.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swap_chain.images[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swap_chain.format;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        if (vkCreateImageView(vulkan.logic_device, &createInfo, nullptr, &swap_chain.image_views[i]) != VK_SUCCESS) {
            sys_fatal("Failed to create image view for Vulkan!");
            break;
        }
    }
}

static void vulkan_create_graphics_pipeline(void) {
    VkShaderModule vert_shader_module;
    VkShaderModule frag_shader_module;
    
    // TODO: Generate a shader.
    
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vert_shader_module;
    vertShaderStageInfo.pName = "main";
    
    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = frag_shader_module;
    fragShaderStageInfo.pName = "main";
    
    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
    
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = (uint32_t)dynamicStates.size();
    dynamicState.pDynamicStates = dynamicStates.data();
    
    // Cleanup
    vkDestroyShaderModule(device, frag_shader_module, nullptr);
    vkDestroyShaderModule(device, vert_shader_module, nullptr);
}

static void gfx_vulkan_init() {
#if FOR_WINDOWS || defined(OSX_BUILD)
    GLenum err;
    if ((err = glewInit()) != GLEW_OK)
        sys_fatal("Could not init GLEW:\n%s", glewGetErrorString(err));
#endif

    vulkan_create_instance();
    vulkan_create_surface();
    vulkan_select_display_device();
    vulkan_create_swap_chains();
    vulkan_create_image_views();
}

static void gfx_vulkan_on_resize() {
}

static void gfx_vulkan_start_frame() {
}

static void gfx_vulkan_end_frame() {
}

static void gfx_vulkan_finish_render() {
}

static void gfx_vulkan_shutdown() {
    for (auto view : wap_chain.image_views) {
        vkDestroyImageView(vulkan.logic_device, view, nullptr);
    }
    vkDestroySwapchainKHR(vulkan.logic_device, vulkan.swap_chain, nullptr);
    vkDestroySurfaceKHR(vulkan.instance, vulkan.surface, nullptr);
    vkDestroyInstance(vulkan.instance, nullptr);
}

struct GfxRenderingAPI gfx_vulkan_api = {
    gfx_vulkan_z_is_from_0_to_1,
    gfx_vulkan_unload_shader,
    gfx_vulkan_load_shader,
    gfx_vulkan_create_and_load_new_shader,
    gfx_vulkan_lookup_shader,
    gfx_vulkan_shader_get_info,
    gfx_vulkan_new_texture,
    gfx_vulkan_select_texture,
    gfx_vulkan_upload_texture,
    gfx_vulkan_set_sampler_parameters,
    gfx_vulkan_set_depth_test,
    gfx_vulkan_set_depth_mask,
    gfx_vulkan_set_zmode_decal,
    gfx_vulkan_set_viewport,
    gfx_vulkan_set_scissor,
    gfx_vulkan_set_use_alpha,
    gfx_vulkan_draw_triangles,
    gfx_vulkan_init,
    gfx_vulkan_on_resize,
    gfx_vulkan_start_frame,
    gfx_vulkan_end_frame,
    gfx_vulkan_finish_render,
    gfx_vulkan_shutdown
};