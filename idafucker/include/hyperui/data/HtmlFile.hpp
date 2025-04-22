#pragma once
#include <iostream>  // istream, istreambuf_iterator

#include <hyperui/CoreDefines.hpp>

HYPERUI_NAMESPACE_BEGIN

class HtmlFile {
public:
  explicit constexpr HtmlFile(std::string data) noexcept
      : data_{std::move(data)}
  {
  }

  explicit HtmlFile(std::istream &stream)
      : data_{
            std::istreambuf_iterator<char>{stream},
            std::istreambuf_iterator<char>{}}
  {
  }

  constexpr HtmlFile(const HtmlFile &) noexcept = default;
  constexpr HtmlFile(HtmlFile &&) noexcept = default;

  [[nodiscard]] constexpr const std::string &data() const noexcept
  {
    return data_;
  }

private:
  std::string data_;
};

HYPERUI_NAMESPACE_END