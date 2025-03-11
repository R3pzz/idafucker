#pragma once
#include <idafucker/CoreDefines.hpp>
#include "Application.hpp"

#include <memory> // unique_ptr

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
  [[nodiscard]] std::shared_ptr<Application> create(int argc, char *argv[]);

  [[nodiscard]] auto instance() const noexcept { return _instance; }

private:
  std::shared_ptr<Application> _instance{};
};

extern ApplicationFactory applicationFactory;

IDAFUCKER_NAMESPACE_END