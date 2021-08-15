#include <window.hpp>

window::window(int width, int height, const std::string &title, GLboolean resizable,
               const window::event_handler_func &handler)
    : title_(title),
      width_(width),
      height_(height) {

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_RESIZABLE, resizable);
  window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

  if (window_ == nullptr) {
    glfwTerminate();
    std::cout << "Failed to create window" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  glfwSetWindowUserPointer(window_, &data_);
  glfwMakeContextCurrent(window_);
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK) {
    glfwTerminate();
    std::cout << "glewInit failed" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  data_.event_handler = handler;
  use_standard_cursor();

  glfwSetWindowCloseCallback(window_, [](GLFWwindow *window) {
    auto data = static_cast<window_data *>(glfwGetWindowUserPointer(window));
    window_close_event e;
    data->event_handler(e);
  });

  glfwSetMouseButtonCallback(window_, [](auto w, int b, int a, int m) {
    auto data = static_cast<window_data *>(glfwGetWindowUserPointer(w));
    if (a == GLFW_PRESS) {
      mouse_press_event e{b, m};
      data->event_handler(e);
    }
  });

  glfwSetKeyCallback(window_, [](auto w, int b, int, int a, int mods) {
    auto data = static_cast<window_data *>(glfwGetWindowUserPointer(w));
    if (a == GLFW_PRESS) {
      key_down_event e{b, mods};
      data->event_handler(e);
    }
  });

  glfwSetCursorPosCallback(window_, [](auto w, double x, double y) {
    auto data = static_cast<window_data *>(glfwGetWindowUserPointer(w));
    mouse_moves_event e{static_cast<int>(x), static_cast<int>(y)};
    data->event_handler(e);
  });

  glfwSetDropCallback(window_, [](auto w, int count, const char **paths) {
    auto data = static_cast<window_data *>(glfwGetWindowUserPointer(w));
    drop_event e{std::vector<std::string>{paths, paths + count}};
    data->event_handler(e);
  });
}

window::window(int width, int height, const std::string &title, GLboolean resizable,
               const std::filesystem::path &iconPath, const window::event_handler_func &handler)
    : window(width, height, title, resizable, handler) {
  image icon{iconPath};
  GLFWimage iconImg = icon.get_glfw_image();
  glfwSetWindowIcon(window_, 1, &iconImg);
}

void window::update() {
  glfwPollEvents();
  glfwSwapBuffers(window_);
}

void window::set_cursor(GLFWimage *img) {
  auto newCursor = glfwCreateCursor(img, img->width / 2, img->height / 2);
  glfwSetCursor(window_, newCursor);
  if (customCursor_ != nullptr) {
    glfwDestroyCursor(customCursor_);
  }
  customCursor_ = newCursor;
}

void window::use_custom_cursor() {
  glfwSetCursor(window_, customCursor_);

}

void window::use_standard_cursor() {
  glfwSetCursor(window_, standardCursor_);
}
