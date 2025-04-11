#pragma once
#include <vector>  // vector

#include <idafucker/CoreDefines.hpp>
#include <idafucker/runtime/Application.hpp>

IDAFUCKER_NAMESPACE_BEGIN

namespace win {
class ApplicationImpl final : public Application {
  IDAFUCKER_NONCOPYABLE(ApplicationImpl);

 public:
  ApplicationImpl(int argc, char* argv[]);
  ~ApplicationImpl() override;

  [[nodiscard]] Window::Ref makeWindow(const WindowSpecs& specs) override;

 private:
  void registerWindowClass();
  void unregisterWindowClass() noexcept;

  ATOM mainClassAtom_{};
  std::vector<Window::Ref> windows_{};
};
}  // namespace win

IDAFUCKER_NAMESPACE_END