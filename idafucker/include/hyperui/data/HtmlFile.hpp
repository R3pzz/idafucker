#pragma once
#include <iostream>  // istream, istreambuf_iterator

#include <hyperui/CoreDefines.hpp>

HYPERUI_NAMESPACE_BEGIN

class HtmlFile {
 public:
  explicit HtmlFile(std::istream &stream)
      : data_{std::istreambuf_iterator<char>{stream},
              std::istreambuf_iterator<char>{}}
  {
  }

  [[nodiscard]] constexpr const std::string &data() const noexcept
  {
    return data_;
  }

 private:
  std::string data_;
};

HYPERUI_NAMESPACE_END