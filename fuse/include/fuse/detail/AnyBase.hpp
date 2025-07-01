#pragma once
#include <memory>  // weak_ptr

#include <fuse/Config.hpp>
#include <fuse/TypeTraits.hpp>

namespace fuse::detail
{
template <std::size_t Size> class AnyBase {
private:
  enum class Request {
    CopyConstruct,  //< Call a copy-constructor of a contained object
    MoveConstruct,  //< Call a move-constructor of a contained object
    Destroy,        //< Call a destructor of a contained object, free memory if
                    // we're owning it
    Copy,           //< Call a copy-assign operator of a contained object
    Move,           //< Call a move-assign operator of a contained object
  };

  // Original idea taken from
  // https://github.com/skypjack/entt/blob/master/src/entt/core/any.hpp
  template <typename T>
    requires std::is_object_v<T>
  static void rttiFunction(Request req, const AnyBase* any, const void* rhs) {
    auto lhs = static_cast<const T*>(any->data());

    switch (req) {
      case Request::CopyConstruct:
        // Copy-construct Any from rhs assuming that any->reset() has
        // already been called
        if constexpr (std::is_copy_constructible_v<T>) {
          auto data =
              static_cast<const T*>(static_cast<const AnyBase*>(rhs)->data());
          ::new (const_cast<T*>(lhs)) T{*data};
        }
        break;
      case Request::MoveConstruct:
        // Move-construct Any from rhs assuming that any->reset() has
        // already been called
        if constexpr (std::is_move_constructible_v<T>) {
          auto data =
              static_cast<const T*>(static_cast<const AnyBase*>(rhs)->data());
          ::new (const_cast<T*>(lhs)) T{std::move(*const_cast<T*>(data))};
        }
        break;
      case Request::Destroy:
        // Destruct an object owned by Any
        if constexpr (std::is_array_v<T>)
          delete[] lhs;
        else
          any->repr_ == Representation::Embedded ? lhs->~T() : delete lhs;
        break;
      case Request::Copy:
        if constexpr (std::is_copy_assignable_v<T>)
          *const_cast<T*>(lhs) = *static_cast<const T*>(rhs);
        break;
      case Request::Move:
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
    Empty,     //< No data is stored
    Embedded,  //< Data is stored internally, inside the Any object
    Remote,    //< Data is stored externally, we have the ownership of data
  };

  constexpr AnyBase() noexcept = default;

  AnyBase(auto&& value) {
    using Base = std::remove_reference_t<decltype(value)>;

    if constexpr (is_embeddable_v<Base>) {
      // Construct an object inside the embedded storage.
      rtti_ = &rttiFunction<Base>;
      type_ = &typeid(Base);
      repr_ = Representation::Embedded;
      ::new (reinterpret_cast<Base*>(&embedded_))
          Base{std::forward<decltype(value)>(value)};
    } else {
      // Construct an object in heap memory.
      rtti_ = &rttiFunction<Base>;
      type_ = &typeid(Base);
      repr_ = Representation::Remote;
      remote_ = new Base{std::move(value)};
    }
  }

  AnyBase(AnyBase&& other)
      : rtti_{other.rtti_}, type_{other.type_}, repr_{other.repr_} {
    if (other.repr_ == Representation::Embedded)
      rtti_(Request::MoveConstruct, this, static_cast<const void*>(&other));
    else if (other.repr_ == Representation::Remote)
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
    rtti_(Request::CopyConstruct, this, static_cast<const void*>(&rhs));
    return *this;
  }

  // Get the raw data pointer
  [[nodiscard]] void* data() noexcept {
    switch (repr_) {
      case Representation::Empty:
        return nullptr;
      case Representation::Embedded:
        return reinterpret_cast<void*>(std::addressof(embedded_));
      case Representation::Remote:
        return remote_;
      default:
        assert(false);
        return nullptr;
    }
  }

  // Get the raw data const pointer
  [[nodiscard]] const void* data() const noexcept {
    switch (repr_) {
      case Representation::Empty:
        return nullptr;
      case Representation::Embedded:
        return reinterpret_cast<const void*>(std::addressof(embedded_));
      case Representation::Remote:
        return remote_;
      default:
        assert(false);
        return nullptr;
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
    return repr_ == Representation::Empty;
  }

  // Reset the value if it exists
  void reset() {
    if (rtti_ != nullptr)
      rtti_(Request::Destroy, this, nullptr);

    remote_ = nullptr;
    rtti_ = nullptr;
    type_ = nullptr;
    repr_ = Representation::Empty;
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