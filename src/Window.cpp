#include "Window.hpp"

#include <stdexcept>

namespace learnVulkan {
    Window::Window(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
        initWindow();
    }

    Window::~Window() {
      glfwDestroyWindow(window);
      glfwTerminate();
    }

    void Window::initWindow() {
      glfwInit();
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //NOT_OPENGL_CONTENT
      glfwWindowHint(GLFW_RESIZABLE,GLFW_TRUE);

      window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
      glfwSetWindowUserPointer(window, this);
      glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface){
      if(glfwCreateWindowSurface(instance,window,nullptr,surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Window Surface");
      }
    }
    void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
      auto Window_ = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
      Window_->framebufferResized = true;
      Window_->width = width;
      Window_->height = height;
    }

}  // namespace learnVulkan