#pragma once
#include <format>  // format
#include <memory>  // unique_ptr

#include <idafucker/CoreDefines.hpp>

#include "TypeTraits.hpp"

IDAFUCKER_NAMESPACE_BEGIN

namespace detail
{
template <std::size_t Size> class AnyBase {
 private:
  enum class Request {
    CopyConstruct,  //< Call a copy-constructor of a contained object
    MoveConstruct,  //< Call a move-constructor of a contained object
    Destruct,       //< Call a destructor of a contained object, free memory if
                    // we're owning it
    Copy,           //< Call a copy-assign operator of a contained object
    Move,           //< Call a move-assign operator of a contained object
  };

  // Original idea taken from
  // https://github.com/skypjack/entt/blob/master/src/entt/core/any.hpp
  template <typename T>
    requires std::is_object<T>::value
  static void rttiFunction(Request req, const AnyBase* any, const void* rhs)
  {
    auto lhs = static_cast<const T*>(any->data());

    switch (req) {
      case Request::CopyConstruct:
        // Copy-construct Any from rhs assuming that any->reset() has
        // already been called
        if constexpr (std::is_copy_constructible<T>::value) {
          std::construct_at(
              const_cast<T*>(lhs),
              *static_cast<const T*>(static_cast<const AnyBase*>(rhs)->data()));
        }
        break;
      case Request::Destruct:
        // Destruct an object owned by Any
        if constexpr (std::is_array<T>::value) {
          delete[] lhs;
        } else {
          any->repr_ == Representation::Embedded ? lhs->~T() : delete lhs;
        }
        break;
      case Request::Copy:
        if constexpr (std::is_copy_assignable<T>::value) {
          *const_cast<T*>(lhs) = *static_cast<const T*>(rhs);
        }
        break;
      case Request::Move:
        if constexpr (std::is_move_assignable<T>::value) {
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
      sizeof(typename std::remove_cvref<T>::type) <= Size;

 public:
  enum class Representation {
    Empty,      //< No data is stored
    Embedded,   //< Data is stored internally, inside the Any object
    Remote,     //< Data is stored externally, we have the ownership of data
    Reference,  //< Data is stored externally, we don't have an ownership of
                // data
  };

  constexpr AnyBase() noexcept = default;

  AnyBase(auto&& value)
  {
    using Base = typename std::remove_reference<decltype(value)>::type;

    if constexpr (is_weak_reference_v<Base>) {
      // Construct from weak
      rttiFunction_ =
          &rttiFunction<typename weak_reference_traits<Base>::contained_type>;
      type_ = &typeid(typename weak_reference_traits<Base>::contained_type);
      repr_ = Representation::Reference;
      reference_ = std::move(value);
    } else if constexpr (is_embeddable_v<Base>) {
      rttiFunction_ = &rttiFunction<Base>;
      type_ = &typeid(Base);
      repr_ = Representation::Embedded;

      std::construct_at(
          reinterpret_cast<Base*>(&embedded_),
          std::forward<decltype(value)>(value));
    } else {
      rttiFunction_ = &rttiFunction<Base>;
      type_ = &typeid(Base);
      repr_ = Representation::Remote;
      remote_ = std::make_shared<Base>(std::move(value));
    }
  }

  ~AnyBase()
  {
    reset();
  }

  // Copy-assign this to an Any
  auto& operator=(const AnyBase& rhs) noexcept
  {
    reset();

    repr_ = rhs.repr_;
    type_ = rhs.type_;

    rttiFunction_ = rhs.rttiFunction_;
    rttiFunction_(Request::CopyConstruct, this, static_cast<const void*>(&rhs));

    return *this;
  }

  // Get the raw data pointer
  [[nodiscard]] void* data() noexcept
  {
    switch (repr_) {
      case Representation::Empty:
        return nullptr;
      case Representation::Embedded:
        return reinterpret_cast<void*>(std::addressof(embedded_));
      case Representation::Remote:
        return remote_;
      case Representation::Reference:
        if (auto ptr = reference_.lock()) {
          return ptr.get();
        }
        return nullptr;
    }
  }

  // Get the raw data const pointer
  [[nodiscard]] const void* data() const noexcept
  {
    switch (repr_) {
      case Representation::Empty:
        return nullptr;
      case Representation::Embedded:
        return reinterpret_cast<const void*>(std::addressof(embedded_));
      case Representation::Remote:
        return remote_;
      case Representation::Reference:
        if (auto ptr = reference_.lock()) {
          return ptr.get();
        }
        return nullptr;
    }
  }

  // Type-compare and get a typed data pointer
  template <typename T> [[nodiscard]] auto get() noexcept -> T*
  {
    if (type() != typeid(T)) {
      return nullptr;
    }
    return static_cast<T*>(data());
  }

  // Type-compare and get a typed data const pointer
  template <typename T> [[nodiscard]] auto get() const noexcept -> const T*
  {
    if (type() != typeid(T)) {
      return nullptr;
    }
    return static_cast<const T*>(data());
  }

  // Get type information if it is valid, type information of 'void' otherwise
  [[nodiscard]] const std::type_info& type() const noexcept
  {
    return type_ == nullptr ? typeid(void) : *type_;
  }

  // Check if this Any has a value
  [[nodiscard]] constexpr bool empty() const noexcept
  {
    return repr_ == Representation::Empty;
  }

  // Reset the value if it exists
  void reset()
  {
    if (rttiFunction_ != nullptr && repr_ != Representation::Reference) {
      rttiFunction_(Request::Destruct, this, nullptr);
    }

    remote_ = nullptr;
    rttiFunction_ = nullptr;
    type_ = nullptr;
    repr_ = Representation::Empty;
  }

 private:
  RTTIFunction* rttiFunction_{};
  const std::type_info* type_{};
  Representation repr_{};

  union {
    std::byte embedded_[Size]{};     //< Internal Any data
    void* remote_;                   //< Remote Any data
    std::weak_ptr<void> reference_;  //< Reference to an object
  };
};
}  // namespace detail

// 32 for an average class size and 8 for a vtable pointer
using Any = detail::AnyBase<40u>;

template <typename T, typename... Args>
[[nodiscard]] Any makeAny(Args&&... args)
{
  return Any{T{std::forward<Args>(args)...}};
}

IDAFUCKER_NAMESPACE_END