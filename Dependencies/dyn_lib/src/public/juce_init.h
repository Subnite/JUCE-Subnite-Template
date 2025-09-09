#pragma once

#include "common.h"
#include <thread>
#include <atomic>
#include <memory>

namespace subnite::dynlib {

struct MY_API MessageManagerLock {
    MessageManagerLock();
    bool lockWasGained;
private:
    struct Impl;
    std::shared_ptr<Impl> impl; // hiding the implementation. Shared pointers don't need a complete type at declaration or destruction.
};

class MY_API JuceInit {
public:
    JuceInit();
    ~JuceInit();
private:
    std::thread messageThread;
    std::atomic<bool> shouldRunMessageThread { true };
};

} // namespace source
