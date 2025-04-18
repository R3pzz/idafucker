#pragma once
#include <hyperui/CoreDefines.hpp>

#include <nlohmann/json.hpp>

HYPERUI_NAMESPACE_BEGIN

template <typename T>
concept has_serializer = !std::is_same_v<decltype(nlohmann::to_json(std::declval<>std::declval<T>()))>;

HYPERUI_NAMESPACE_END