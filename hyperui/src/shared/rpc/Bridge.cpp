#include <hyperui/rpc/Bridge.hpp>
#include <hyperui/rpc/JSBridgeScript.hpp>

#include <spdlog/spdlog.h>

namespace hyperui
{

Bridge::Bridge(Engine &engine) noexcept : engine_{engine} {
  engine.addJSInitializationScript(detail::k_jsBridgeScript);
  engine.jsMessageReceivedEvent.connect(
      std::bind(&Bridge::handleIncomingMessage, this, std::placeholders::_1));
}

void Bridge::handleIncomingMessage(nlohmann::json message) {
  const auto &type = message["type"];
  if (type == "call")
    handleCallMessage(message);
  else if (type == "response")
    handleResponseMessage(message);
  else
    throw std::runtime_error{"bad message type"};
}

void Bridge::handleCallMessage(const nlohmann::json &message) const {
  const auto it = jsToNativeBindings_.find(message["method"]);
  if (it == std::end(jsToNativeBindings_))
    return;

  nlohmann::json response{{"type", "response"}, {"id", message["id"]}};
  try {
    response["result"] = it->second(message["args"]);
  } catch (std::exception &e) {
    response["exception"] = e.what();
  }
  engine_.postJSMessage(fuse::unicodeCast(response.dump()).c_str());
}

void Bridge::handleResponseMessage(const nlohmann::json &message) {
  const std::size_t id = message["id"];
  if (id >= nativeToJSPromises_.size())
    return;

  if (message.contains("exception")) {
    std::string exception = message["exception"];
    nativeToJSPromises_[id] =
        std::make_exception_ptr(std::exception{exception.c_str()});
  } else if (message.contains("result")) {
    nativeToJSPromises_[id] = std::move(message["result"]);
  }

  nativeToJSPromises_.erase(std::begin(nativeToJSPromises_) + id);
}

}  // namespace hyperui