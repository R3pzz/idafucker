#include <hyperui/widgets/CheckboxWidget.hpp>

HYPERUI_NAMESPACE_BEGIN

void CheckboxWidget::enable()
{
  _enabled = true;
  _stateChanged(true);
}

void CheckboxWidget::disable()
{
  _enabled = false;
  _stateChanged(false);
}

void CheckboxWidget::toggle()
{
  _enabled = !_enabled;
  _stateChanged(_enabled);
}

HYPERUI_NAMESPACE_END