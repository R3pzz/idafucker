#pragma once
#include <functional>     // function
#include <unordered_map>  // unordered_map
#include <vector>         // vector

#include <hyperui/CoreDefines.hpp>
#include <idafucker/base/String.hpp>
#include <idafucker/base/TypeTraits.hpp>
#include <idafucker/base/future/Promise.hpp>

#include <nlohmann/json.hpp>  // json

HYPERUI_NAMESPACE_BEGIN

template <class Engine> class RpcHost final {
public:
  static constexpr auto InitScript =
      LR"js(
(() => {
    const pending = new Map();
    let idCounter = 1;

    window.nativeBridge = new Proxy({}, {
        get(target, method) {
            return (...args) => {
                const id = idCounter++;
                const payload = {
                    type: "call",
                    id,
                    method,
                    args
                };

                return new Promise((resolve, reject) => {
                    pending.set(id, { resolve, reject });
                    window.chrome.webview.postMessage(payload);
                });
            };
        }
    });

    window.chrome.webview.addEventListener("message", (event) => {
        const msg = event.data;
        if (msg.type === "response") {
            const handler = pending.get(msg.id);
            if (handler) {
                pending.delete(msg.id);
                msg.error ? handler.reject(msg.error) : handler.resolve(msg.result);
            }
        } else if (msg.type === "call") {
            const { id, method, args } = msg;
            try {
                const result = window[method](...args);
                Promise.resolve(result).then((res) => {
                    window.chrome.webview.postMessage({
                        type: "response",
                        id,
                        result: res
                    });
                });
            } catch (e) {
                window.chrome.webview.postMessage({
                    type: "response",
                    id,
                    exception: e.message
                });
            }
        }
    });
})();
  )js";

  explicit RpcHost(Engine &engine) noexcept : engine{engine}
  {
#if defined(IDAFUCKER_PLATFORM_WIN32)
    const auto MessageListener =
        [this](
            ICoreWebView2 *,
            ICoreWebView2WebMessageReceivedEventArgs *args) -> HRESULT {
      LPWSTR message{};
      args->get_WebMessageAsJson(&message);
      onMessage(nlohmann::json::parse(idafucker::convertToUtf8(message)));
      return S_OK;
    };
#elif defined(IDAFUCKER_PLATFORM_LINUX)
  #pragma error "Platform not supported"
#else
  #pragma error "Platform not supported"
#endif  // defined(IDAFUCKER_PLATFORM_WIN32)

    engine.addInitScript(InitScript);
    engine.addMessageListener(MessageListener);
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