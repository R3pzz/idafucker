#pragma once
#include <hyperui/Config.hpp>

namespace hyperui::detail
{

constexpr auto k_jsBridgeScript{ // clang-format disable
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
  )js"
};

}  // namespace hyperui::detail