#include "Window.hpp"

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
      glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);

      window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
      glfwSetWindowUserPointer(window, this);
  }
}  // namespace learnVulkan