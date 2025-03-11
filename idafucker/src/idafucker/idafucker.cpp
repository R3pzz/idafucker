#include <hyperui/renderer/shaders/ShaderProgram.hpp>
#include <hyperui/renderer/RenderContext.hpp>
#include <idafucker/resources/ResourceManager.hpp>
#include <idafucker/runtime/ApplicationFactory.hpp>
#include <idafucker/runtime/WindowFactory.hpp>

#include <spdlog/spdlog.h>

namespace
{

auto initWindowing(auto &app)
{
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  idafucker::WindowSpecs specs{};
  specs.extent = {1920.f, 1080.f};
  specs.title = L"gay";

  auto window = idafucker::windowFactory.create(specs);
  window->changeVisibility(idafucker::WindowVisibility::Minimized);
  app->registerWindow(window);

  idafucker::OpenGLContextSpecs glSpecs{};
  glSpecs.colorDepth = 16;
  glSpecs.depthBufferSize = 16;
  glSpecs.stencilBufferSize = 0;
  glSpecs.format = idafucker::PixelFormat::RGBA;

  return window->createOpenGLContext(glSpecs);
}

} // namespace

int main(int argc, char *argv[])
{
  try {
    spdlog::set_level(spdlog::level::debug);

    auto app = idafucker::applicationFactory.create(argc, argv);
    auto glCtx = initWindowing(app);

    hyperui::RenderContext renderCtx{};
    renderCtx.setClearColor({1.f, 0.f, 0.f, 0.f});
    renderCtx.setClearDepth(1.f);

    glCtx.lock()->attach();

    idafucker::resourceManager.createProceduralResource<hyperui::ShaderProgram>("Cool resource", "", "");
    
    while (app->running()) {
      MSG msg{};

      while (::PeekMessageW(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
        ::TranslateMessage(&msg);
        ::DispatchMessageW(&msg);
      }

      idafucker::resourceManager.observe();

      glViewport(0, 0, 1920.f, 1080.f);
      renderCtx.clearBuffers();
      renderCtx.finish();
      glCtx.lock()->swapBuffers();
    }
  } catch (std::exception &e) {
    spdlog::critical("Exception caught: {}", e.what());
    std::this_thread::sleep_for(std::chrono::seconds{10u});
  }

  return 0;
}