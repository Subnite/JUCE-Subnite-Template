#pragma once
#include "common.h"
#include <memory>

namespace subnite::dynlib {

template <typename T>
struct Observer {
  std::unique_ptr<T> obj;
};

}
