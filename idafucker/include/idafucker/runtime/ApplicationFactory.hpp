#pragma once
#include <memory>  // unique_ptr

#include <idafucker/CoreDefines.hpp>

#include "Application.hpp"

//
// Platform-independent application factory.
//
// Creates and stores a reference to a single application instance.
//

IDAFUCKER_NAMESPACE_BEGIN

class ApplicationFactory {
 public:
  constexpr ApplicationFactory() noexcept = default;

  // Creates the application. This function should only be called once!
  [[nodiscard]] static Application::Ref create(int argc, char* argv[]);

  [[nodiscard]] static constexpr const Application::Ref& instance() noexcept
  {
    return instance_;
  }

 private:
  inline static std::shared_ptr<Application> instance_{};
};

IDAFUCKER_NAMESPACE_END