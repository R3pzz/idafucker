#pragma once
#include <idafucker/CoreDefines.hpp>

#include <list> // list
#include <unordered_map> // unordered_map

IDAFUCKER_NAMESPACE_BEGIN

class CacheKeyNotFoundError : public std::runtime_error {
public:
  CacheKeyNotFoundError() : std::runtime_error{"Cache key not found"} {}
  explicit CacheKeyNotFoundError(const std::string &err) : std::runtime_error{err} {}
  explicit CacheKeyNotFoundError(const char *err) : std::runtime_error{err} {}

  ~CacheKeyNotFoundError() = default;
};

template <typename Key, typename Value, std::size_t MaxSize>
class Cache {
public:
  constexpr Cache() noexcept = default;

  void put(const Key &key, const Value &value)
  {
    const auto it = _iteratorMap.find(key);
    _items.push_front(value);
    if (it != _iteratorMap.end()) {
      _items.erase(it->second);
      _iteratorMap.erase(it);
    }
    _iteratorMap[key] = _items.begin();

    if (_iteratorMap.size() > MaxSize) {
      auto last = --_items.end();
      _iteratorMap.erase(last->first);
      _items.pop_back();
    }
  }

  [[nodiscard]] auto &at(const Key &key)
  {
    auto it = _iteratorMap.find(key);
    if (it == _iteratorMap.end())
      throw CacheKeyNotFoundError{};
    
    _items.splice(_items.begin(), _items, it->second);
    return it->second->second;
  }

  [[nodiscard]] const auto &at(const Key &key) const
  {
    auto it = _iteratorMap.find(key);
    if (it == _iteratorMap.end())
      throw CacheKeyNotFoundError{};

    _items.splice(_items.begin(), _items, it->second);
    return it->second->second;
  }

  [[nodiscard]] auto *atOr(const Key &key, Value *fallback)
  {
    auto it = _iteratorMap.find(key);
    if (it == _iteratorMap.end())
      return fallback;

    _items.splice(_items.begin(), _items, it->second);
    return it->second->second;
  }

  [[nodiscard]] const auto *atOr(const Key &key, const Value *fallback) const
  {
    auto it = _iteratorMap.find(key);
    if (it == _iteratorMap.end())
      return fallback;

    _items.splice(_items.begin(), _items, it->second);
    return it->second->second;
  }

  void erase(const Key &key)
  {
    const auto it = _iteratorMap.find(key);
    if (it == _iteratorMap.end())
      return;

    _items.erase(it->second);
    _iteratorMap.erase(it);
  }

  template <typename Fn, typename... Args>
    requires std::is_invocable<Fn, Value &, Args...>::value
  void apply(const Fn& fn, Args&&... args)
  {
    for (auto &&it : _iteratorMap) {
      fn(it->second->second, std::forward<Args>(args));
    }
  }

  template <typename Fn, typename... Args>
    requires std::is_invocable<Fn, const Value &, Args...>::value
  void apply(const Fn &fn, Args &&...args) const
  {
    for (auto &&it : _iteratorMap) {
      fn(it->second->second, std::forward<Args>(args));
    }
  }

  [[nodiscard]] constexpr auto size() const noexcept { return _items.size(); }
  [[nodiscard]] constexpr auto maxSize() const noexcept { return MaxSize; }

  [[nodiscard]] constexpr auto empty() const noexcept { return _items.empty(); }

  [[nodiscard]] auto contains(const Key &key) const noexcept { return _iteratorMap.contains(key); }
  
  [[nodiscard]] auto &mostUsed() noexcept { return _items.front(); }
  [[nodiscard]] const auto &mostUsed() const noexcept { return _items.front(); }

  [[nodiscard]] auto &leastUsed() noexcept { return _items.back(); }
  [[nodiscard]] const auto &leastUsed() const noexcept { return _items.back(); }

private:
  using KeyValuePair = std::pair<Key, Value>;

  mutable std::list<KeyValuePair> _items{};
  std::unordered_map<Key, typename std::list<KeyValuePair>::iterator> _iteratorMap{};
};

IDAFUCKER_NAMESPACE_END