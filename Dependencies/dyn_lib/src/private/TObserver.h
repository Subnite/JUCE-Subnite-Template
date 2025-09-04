#pragma once
#include "common.h"
#include <memory>

namespace source {

template <typename T>
struct Observer {
  std::unique_ptr<T> obj;
};

}
