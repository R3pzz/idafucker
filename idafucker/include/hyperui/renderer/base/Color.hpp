#pragma once
#include <hyperui/CoreDefines.hpp>
#include <idafucker/base/math/Vector4.hpp>

//
// An auxiliary color data type. Supports HSL/RGB conversions.
// Supports single-precision and integral encodings.
//

HYPERUI_NAMESPACE_BEGIN

class Color : private idafucker::Vector4<float> {
public:
  constexpr Color() noexcept = default;

  constexpr Color(float r, float g, float b) noexcept : idafucker::Vector4<float>{r, g, b, 1.f} {}
  constexpr Color(float r, float g, float b, float a) noexcept : idafucker::Vector4<float>{r, g, b, a} {}

  constexpr Color(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept
      : idafucker::Vector4<float>{
            static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f, 1.f}
  {}
  constexpr Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept
      : idafucker::Vector4<float>{
            static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f,
            static_cast<float>(a) / 255.f}
  {}

  [[nodiscard]] constexpr auto r() const noexcept { return x; }
  [[nodiscard]] constexpr auto g() const noexcept { return y; }
  [[nodiscard]] constexpr auto b() const noexcept { return z; }
  [[nodiscard]] constexpr auto a() const noexcept { return w; }

private:
};

HYPERUI_NAMESPACE_END