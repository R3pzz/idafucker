#pragma once
#include <platform/Application.hpp>
#include <platform/Window.hpp>

#include <print> // print

int main(int argc, char* argv[]) {
  try {
    idafucker::App app{argc, argv};
    auto &mainWnd = app.createWindow(
        {.style{WS_OVERLAPPEDWINDOW}, .rect{0, 0, 1920, 1080}, .title{L"idafucker"}, .customTitleBar{true}});
    mainWnd.setVisibility(true);

    while (app.running()) {
      MSG msg{};

      while (::PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
      }
    }
  } catch (std::exception &e) {
    std::print("Exception caught: {}\n", e.what());
    std::this_thread::sleep_for(std::chrono::seconds{10u});
  }

  return 0;
}