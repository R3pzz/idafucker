#pragma once
#include <CoreDefines.hpp>
#include <hyperui/core/math/Rectangle.hpp>

IDAFUCKER_NAMESPACE_BEGIN

namespace hyperui {
namespace widgets {

class Label {};
class Checkbox {};

} // namespace widgets

class GenericLayout {
public:
  class Style {
  public:
  };

private:
};

class LayoutDescription {};

class FlexLayout : public GenericLayout {
public:
  class FlexStyle : public Style {
    
  };

private:
};

// Each layout leaf is an entry in a GenericLayout subclass that should implement such members:
// calcSize() -
// getLayoutDesc() -
//
class LayoutLeaf {
public:
  constexpr LayoutLeaf() noexcept = default;

  virtual void setRectangle(const Rectangle<float> &rectangle) = 0;
  [[nodiscard]] virtual const LayoutDescription &getLayoutDesc() const = 0;
};

// checkbox widget
// lets start by drawing the checkbox widget on a simple frame.
// pipeline: markup -> styling -> tree -> layout -> extract draw calls -> render -> present
// we want to split widget classes from core classes, so we will use this widget's functions
// to create a markup which then will be styled and loaded into the layout tree
class CheckboxWidget : public LayoutLeaf {
public:
  

private:
};

void a() { using namespace hyperui; }

} // namespace hyperui

IDAFUCKER_NAMESPACE_END