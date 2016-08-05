#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#include <emscripten/emscripten.h>
#include <array>
#include <exception>
#include <iostream>
#include <stdexcept>

namespace deus {

class window {
public:
  window(const char* name)
  {
    glfwSetErrorCallback([](int ec, const char* message) {
      std::cerr << "glfw: " << ec << ": " << message << std::endl;
    });

    if (!glfwInit()) {
      throw std::runtime_error("Could not initialize OpenGL ES 3.0.");
    }

    window_ = glfwCreateWindow(640, 480, name, nullptr, nullptr);
    if (!window_) {
      glfwTerminate();
      throw std::runtime_error("Could not initialize the canvas context.");
    }

    glfwMakeContextCurrent(window_);
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glfwGetFramebufferSize(window_, &cx_, &cy_);
    glViewport(0, 0, cx_, cy_);
  }

  ~window()
  {
    if (window_) {
      glfwMakeContextCurrent(nullptr);
      glfwDestroyWindow(window_);
    }
    glfwTerminate();
  }

  void run()
  {
    static std::array<float, 3> colors = {
      1.0f, 0.5f, 0.0f
    };

    for (auto& color : colors) {
      color += 0.004f;
      while (color > 2.0f) {
        color -= 2.0f;
      }
    }

    auto r = colors[0] > 1.0f ? 1.0f - (colors[0] - 1.0f) : colors[0];
    auto g = colors[1] > 1.0f ? 1.0f - (colors[1] - 1.0f) : colors[1];
    auto b = colors[2] > 1.0f ? 1.0f - (colors[2] - 1.0f) : colors[2];

    glClearColor(r, g, b, 1.0);

    glClear(GL_COLOR_BUFFER_BIT);

    // TODO: Draw scene.

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }

private:
  GLFWwindow* window_ = nullptr;
  int cx_ = 0;
  int cy_ = 0;
};

}  // namespace deus

int main(int argc, char *argv[]) {
  try {
    static deus::window window("DEUS");
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop([]() {
      window.run();
    }, 0, 0);
#endif
  }
  catch (const std::exception& e) {
    std::cerr << "error: " << e.what() << std::endl;
  }
}
