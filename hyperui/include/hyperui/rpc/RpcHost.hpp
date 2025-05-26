#pragma once
#include <functional>     // function
#include <unordered_map>  // unordered_map
#include <vector>         // vector

#include <hyperui/CoreDefines.hpp>
#include <idafucker/base/String.hpp>
#include <idafucker/base/TypeTraits.hpp>
#include <idafucker/base/future/Promise.hpp>

#include "RpcInitScript.hpp"

#include <nlohmann/json.hpp>  // json

HYPERUI_NAMESPACE_BEGIN

template <class Engine> class RpcHost final {
public:
  explicit RpcHost(Engine &engine) noexcept : engine{engine}
  {
    engine.addInitScript(detail::RpcInitScript);
    engine.webMessageReceivedEvent
        << std::bind(&RpcHost::onMessage, this, std::placeholders::_1);
  }

  void bind(const std::string &name, auto &&method) noexcept
  {
    native.emplace(name, std::forward<decltype(method)>(method));
  }

  [[nodiscard]] auto call(const std::string &name, auto &&...args)
      -> idafucker::Future<nlohmann::json>
  {
    auto &promise = promises.emplace_back();
    const auto id = promises.size() - 1;

    // Serialize the arguments.
    std::vector serialized{{nlohmann::json{args}...}};
    nlohmann::json message{
        {"type", "call"}, {"id", id}, {"method", name}, {"args", serialized}};

    engine.postMessage(idafucker::convertToUnicode(message.dump()).c_str());
    return promise.future();
  }

private:
  using Native = std::function<std::string(nlohmann::json)>;

  void onMessage(nlohmann::json message) noexcept
  {
    const auto &type = message["type"];

    if (type == "call")
      onCallMessage(message);
    else if (type == "response")
      onResponseMessage(message);
  }

  void onCallMessage(const nlohmann::json &message) const
  {
    const auto it = native.find(message["method"]);
    if (it == std::end(native))
      return;

    nlohmann::json response{{"type", "response"}, {"id", message["id"]}};

    try {
      response["result"] = it->second(message["args"]);
    } catch (std::exception &e) {
      response["exception"] = e.what();
    }

    engine.postMessage(idafucker::convertToUnicode(response.dump()).c_str());
  }

  void onResponseMessage(const nlohmann::json &message)
  {
    const std::size_t id = message["id"];
    if (id >= promises.size())
      return;

    spdlog::info(
        "RpcHost::onResponseMessage: message received: `{}`", message.dump());

    if (message.contains("exception")) {
      std::string exception = message["exception"];
      promises[id] = std::make_exception_ptr(std::exception{exception.c_str()});
    } else if (message.contains("result")) {
      promises[id] = std::move(message["result"]);
    }

    promises.erase(std::begin(promises) + id);
  }

  Engine &engine;

  std::vector<idafucker::Promise<nlohmann::json>> promises;
  std::unordered_map<std::string, Native> native;

  IDAFUCKER_NONCOPYABLE(RpcHost);
};

template <class Engine> RpcHost(Engine) -> RpcHost<Engine>;

HYPERUI_NAMESPACE_END