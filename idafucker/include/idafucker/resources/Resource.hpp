#pragma once
#include <any>     // any
#include <atomic>  // atomic_size_t
#include <memory>  // unique_ptr

#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/Any.hpp>
#include <idafucker/base/Flags.hpp>
#include <idafucker/base/IntrusiveHandle.hpp>

#include "ResourceDependencyResolver.hpp"
#include "ResourceError.hpp"
#include "ResourceFactory.hpp"

//
// A Resource is basically an object whose data is loaded from somewhere(whether
// it be a string, a drive, a website, etc...).
//
// Resource's on-disk representation has its own:
//
//  * Traits file - tells us information about the resource, meaning, what
//    is its data file, how the data is stored, what is the destination type,
//    when it was created/modified, its CRC, its dependencies, etc...;
//  * Data file - provides the actual data of the resource, meaning that if a
//    resource is an image, the data will be stored in either PNG, JPG, BMP,
//    etc...
//
// A Resource object has its own:
//
//  * Reference count - used for optimizing memory usage and unloading
//  unnecessary
//    resources;
//  * Traits object - a virtual representation of the traits file. Also contains
//    a pointer to the resource factory of a given type. If no factory exists,
//    the resource is not loaded;
//  * Data pointer - a pointer to an object of a specified data type(
//    ex: Data file is an image, Data pointer points to an object of a type
//    'Image');
//  * Flags - used for internal computations;
//  * Type - type identification of the resource;
//

IDAFUCKER_NAMESPACE_BEGIN

class ResourceManager;

enum class ResourceFlags {
  None = 0u,
  Required = 1u << 0u,  //< The resource is required to stay in memory even if
                        // not referenced.
};

class Resource final {
 public:
  explicit Resource(ResourceManager& manager, ResourceFactory::Ref factory,
                    ResourceDependencyResolver::Ref resolver,
                    ResourceFlags flags) noexcept
      : manager_{manager},
        factory_{std::move(factory)},
        resolver_{std::move(resolver)},
        flags_{flags}
  {
  }

  ~Resource() noexcept = default;

  // Load/unload data from a stream
  bool load(const std::istream& stream);
  void unload();

  template <class T> [[nodiscard]] auto get() -> T*
  {
    return data_.get<T>();
  }

  template <class T> [[nodiscard]] auto get() const -> const T*
  {
    return data_.get<T>();
  }

  [[nodiscard]] const std::type_info& type() const noexcept
  {
    return data_.type();
  }

  // Reference counting
  std::size_t onReferenceAdded() const
  {
    return ++ref_count_;
  }

  std::size_t onReferenceRemoved() const
  {
    return --ref_count_;
  }

  [[nodiscard]] bool hasReferences() const
  {
    return ref_count_ != 0u;
  }

  [[nodiscard]] std::size_t referenceCount() const
  {
    return ref_count_.load();
  }

  // Resource properties
  [[nodiscard]] constexpr bool empty() const noexcept
  {
    return data_.empty();
  }

  [[nodiscard]] constexpr bool required() const noexcept
  {
    return flags_.contains(ResourceFlags::Required);
  }

  constexpr void require() noexcept
  {
    flags_.add(ResourceFlags::Required);
  }

 private:
  ResourceManager& manager_;
  Any data_;                                  //< The data
  ResourceFactory::Ref factory_;              //< Resource constructor
  ResourceDependencyResolver::Ref resolver_;  //< Dependency resolver
  std::vector<Resource*> deps_;               //< Dependencies
  Flags<ResourceFlags> flags_;                //< Flags
  mutable std::atomic_size_t ref_count_;      //< Reference count

  IDAFUCKER_NONCOPYABLE(Resource);
};

IDAFUCKER_NAMESPACE_END