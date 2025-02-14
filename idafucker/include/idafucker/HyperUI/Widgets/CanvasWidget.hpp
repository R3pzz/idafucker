#pragma once
#include <idafucker/CoreDefines.hpp>
#include "BaseWidget.hpp"

// A canvas widget encapsulates the whole widget tree, meaning it is a root object for all following widgets.

IDAFUCKER_NAMESPACE_BEGIN

namespace hyperui
{

class CanvasWidget : public BaseWidget {
public:
  constexpr CanvasWidget() noexcept = default;

  [[nodiscard]] virtual bool isRoot() const noexcept override { return true; }

  

private:
  
};

} // namespace hyperui

IDAFUCKER_NAMESPACE_END