#pragma once
#include <memory>  // weak_ptr

#include <fuse/Config.hpp>
#include <fuse/TypeTraits.hpp>

namespace fuse::detail
{
template <std::size_t Size> class AnyBase {
private:
  enum class Request {
    kCopyConstruct,  //< Call a copy-constructor of a contained object
    kMoveConstruct,  //< Call a move-constructor of a contained object
    kDestroy,        //< Call a destructor of a contained object, free memory if
                     // we're owning it
    kCopy,           //< Call a copy-assign operator of a contained object
    kMove,           //< Call a move-assign operator of a contained object
  };

  // Original idea taken from
  // https://github.com/skypjack/entt/blob/master/src/entt/core/any.hpp
  template <typename T>
    requires std::is_object_v<T>
  static void rttiFunction(Request req, const AnyBase* any, const void* rhs) {
    auto lhs = static_cast<const T*>(any->data());

    switch (req) {
      case Request::kCopyConstruct:
        // Copy-construct Any from rhs assuming that any->reset() has
        // already been called
        if constexpr (std::is_copy_constructible_v<T>) {
          auto data =
              static_cast<const T*>(static_cast<const AnyBase*>(rhs)->data());
          ::new (const_cast<T*>(lhs)) T{*data};
        }
        break;
      case Request::kMoveConstruct:
        // Move-construct Any from rhs assuming that any->reset() has
        // already been called
        if constexpr (std::is_move_constructible_v<T>) {
          auto data =
              static_cast<const T*>(static_cast<const AnyBase*>(rhs)->data());
          ::new (const_cast<T*>(lhs)) T{std::move(*const_cast<T*>(data))};
        }
        break;
      case Request::kDestroy:
        // Destruct an object owned by Any
        if constexpr (std::is_array_v<T>)
          delete[] lhs;
        else
          any->repr_ == Representation::kEmbedded ? lhs->~T() : delete lhs;
        break;
      case Request::kCopy:
        if constexpr (std::is_copy_assignable_v<T>)
          *const_cast<T*>(lhs) = *static_cast<const T*>(rhs);
        break;
      case Request::kMove:
        if constexpr (std::is_move_assignable_v<T>) {
          *const_cast<T*>(lhs) =
              std::move(*const_cast<T*>(static_cast<const T*>(rhs)));
        }
        break;
    }
  }

  using RTTIFunction = decltype(rttiFunction<int>);

  // Can we embed an object inside this any?
  template <typename T>
  static constexpr auto is_embeddable_v =
      sizeof(std::remove_cvref_t<T>) <= Size;

public:
  enum class Representation {
    kEmpty,     //< No data is stored
    kEmbedded,  //< Data is stored internally, inside the Any object
    kRemote,    //< Data is stored externally, we have the ownership of data
  };

  constexpr AnyBase() noexcept = default;

  AnyBase(auto&& value) {
    using Base = std::remove_reference_t<decltype(value)>;

    if constexpr (is_embeddable_v<Base>) {
      // Construct an object inside the embedded storage.
      rtti_ = &rttiFunction<Base>;
      type_ = &typeid(Base);
      repr_ = Representation::kEmbedded;
      ::new (reinterpret_cast<Base*>(&embedded_))
          Base{std::forward<decltype(value)>(value)};
    } else {
      // Construct an object in heap memory.
      rtti_ = &rttiFunction<Base>;
      type_ = &typeid(Base);
      repr_ = Representation::kRemote;
      remote_ = new Base{std::move(value)};
    }
  }

  AnyBase(AnyBase&& other)
      : rtti_{other.rtti_}, type_{other.type_}, repr_{other.repr_} {
    if (other.repr_ == Representation::kEmbedded)
      rtti_(Request::kMoveConstruct, this, static_cast<const void*>(&other));
    else if (other.repr_ == Representation::kRemote)
      remote_ = std::move(other.remote_);
  }

  ~AnyBase() {
    reset();
  }

  // Copy-assign an Any to this
  auto& operator=(const AnyBase& rhs) noexcept {
    reset();

    repr_ = rhs.repr_;
    type_ = rhs.type_;
    rtti_ = rhs.rtti_;
    rtti_(Request::kCopyConstruct, this, static_cast<const void*>(&rhs));
    return *this;
  }

  // Get the raw data pointer
  [[nodiscard]] void* data() noexcept {
    switch (repr_) {
      case Representation::kEmpty:
        return nullptr;
      case Representation::kEmbedded:
        return reinterpret_cast<void*>(std::addressof(embedded_));
      case Representation::kRemote:
        return remote_;
    }
  }

  // Get the raw data const pointer
  [[nodiscard]] const void* data() const noexcept {
    switch (repr_) {
      case Representation::kEmpty:
        return nullptr;
      case Representation::kEmbedded:
        return reinterpret_cast<const void*>(std::addressof(embedded_));
      case Representation::kRemote:
        return remote_;
    }
  }

  // Type-compare and get a typed data pointer
  template <typename T> [[nodiscard]] auto get() noexcept -> T* {
    return type() == typeid(T) ? static_cast<T*>(data()) : nullptr;
  }

  // Type-compare and get a typed data const pointer
  template <typename T> [[nodiscard]] auto get() const noexcept -> const T* {
    return type() == typeid(T) ? static_cast<const T*>(data()) : nullptr;
  }

  // Get type information if it is valid, type information of 'void' otherwise
  [[nodiscard]] const std::type_info& type() const noexcept {
    return type_ == nullptr ? typeid(void) : *type_;
  }

  // Check if this Any has a value
  [[nodiscard]] constexpr bool empty() const noexcept {
    return repr_ == Representation::kEmpty;
  }

  // Reset the value if it exists
  void reset() {
    if (rtti_ != nullptr)
      rtti_(Request::kDestroy, this, nullptr);

    remote_ = nullptr;
    rtti_ = nullptr;
    type_ = nullptr;
    repr_ = Representation::kEmpty;
  }

private:
  RTTIFunction* rtti_{};
  const std::type_info* type_{};
  Representation repr_{};

  union {
    std::byte embedded_[Size]{};  //< Internal Any data
    void* remote_;                //< Remote Any data
  };
};
}  // namespace fuse::detail