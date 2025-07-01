#pragma once
#include <functional>     // function
#include <unordered_map>  // unordered_map
#include <vector>         // vector

#include <hyperui/Config.hpp>
#include <hyperui/Engine.hpp>

#include <fuse/Future.hpp>
#include <fuse/String.hpp>
#include <nlohmann/json.hpp>  // json

namespace hyperui
{

class Bridge final {
public:
  explicit Bridge(Engine &engine) noexcept;

  template <typename Fn>
  void bind(const std::string &name, Fn &&function) noexcept {
    // JS-to-native bindings use JSON objects to pass arguments around. `function` MUST be
    // callable with an instance of a JSON object containing the arguments of the call.
    static_assert(std::is_invocable_v<Fn, const nlohmann::json &>,
                  "`function` must be invocable with an instance of `nlohmann::json`");
    // JavaScript has its own type system that differs from C++. So we tend to serialize
    // the result of the call into a string to further embed it into a JSON response.
    static_assert(
        std::is_same_v<std::invoke_result_t<Fn, const nlohmann::json &>, std::string>,
        "`function` must return an instance of `std::string`");

    bindings_.emplace(name, std::forward<Fn>(function));
  }

  [[nodiscard]] auto call(const std::string &name, auto &&...args)
      -> fuse::Future<nlohmann::json> {
    auto &promise = promises_.emplace_back();
    const auto id = promises_.size() - 1;

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
  std::vector<fuse::Promise<nlohmann::json>> promises_;

  using Native = std::function<std::string(const nlohmann::json &)>;
  std::unordered_map<std::string, Native> bindings_;

  FUSE_NONCOPYABLE(Bridge);
};

}  // namespace hyperui