#pragma once
#include <functional>     // function
#include <unordered_map>  // unordered_map
#include <vector>         // vector

#include <hyperui/Config.hpp>
#include <hyperui/Engine.hpp>

#include <fuse/String.hpp>
#include <fuse/Future.hpp>

#include <nlohmann/json.hpp>  // json

namespace hyperui
{

class Bridge final {
public:
  explicit Bridge(Engine &engine) noexcept;

  void bindJSToNative(const std::string &name, auto &&method) noexcept {
    jsToNativeBindings_.emplace(name, std::forward<decltype(method)>(method));
  }

  [[nodiscard]] auto call(const std::string &name, auto &&...args)
      -> fuse::Future<nlohmann::json> {
    auto &promise = nativeToJSPromises_.emplace_back();
    const auto id = nativeToJSPromises_.size() - 1;

    // Serialize the arguments.
    std::vector serialized{{nlohmann::json{args}...}};
    nlohmann::json message{
        {"type", "call"}, {"id", id}, {"method", name}, {"args", serialized}};

    engine_.postJSMessage(fuse::unicodeCast(message.dump()).c_str());
    return promise.future();
  }

private:
  void handleIncomingMessage(nlohmann::json message);
  void handleCallMessage(const nlohmann::json &message) const;
  void handleResponseMessage(const nlohmann::json &message);

  Engine &engine_;
  std::vector<fuse::Promise<nlohmann::json>> nativeToJSPromises_;
  
  using Native = std::function<std::string(nlohmann::json)>;
  std::unordered_map<std::string, Native> jsToNativeBindings_;

  FUSE_NONCOPYABLE(Bridge);
};

}  // namespace hyperui