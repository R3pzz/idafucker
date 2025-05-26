#pragma once
#include <fuse/Config.hpp>

#define FUSE_NONCOPYABLE(class_name) \
  class_name(const class_name &) = delete; \
  class_name &operator=(const class_name &) = delete;

#define FUSE_NONMOVABLE(class_name) \
  class_name(class_name &&) = delete; \
  class_name &operator=(class_name &&) = delete;

#define FUSE_SINGLE_INSTANCE(class_name) \
  FUSE_NONCOPYABLE(class_name) \
  FUSE_NONMOVABLE(class_name)