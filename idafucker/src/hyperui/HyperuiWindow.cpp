#include <hyperui/HyperuiWindow.hpp>
#include <hyperui/data/HtmlFile.hpp>

HYPERUI_NAMESPACE_BEGIN

HyperuiWindow::HyperuiWindow(
    const idafucker::WindowOptions &options,
    const idafucker::CommandLine<idafucker::impl::win::Character> &commandLine)
    : idafucker::Window{options}
{
    auto userDataFolder = commandLine.find(L"--user-data-folder").toPath();
    engine_ = std::make_shared<Engine>(
        handle_,
        userDataFolder.empty() ? DefaultUserDataFolder : userDataFolder);
}

LRESULT HyperuiWindow::onWindowMesasge(
    UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message) {
        case WM_SIZE: {
            RECT clientRect{};
            ::GetClientRect(handle_, &clientRect);
            engine_->onResize(clientRect);
        } break;
        default:
            break;
    }

    // Preserve the native message processing
    return idafucker::Window::onWindowMesasge(message, wparam, lparam);
}

HYPERUI_NAMESPACE_END