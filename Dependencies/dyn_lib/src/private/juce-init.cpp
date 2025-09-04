#include "juce-init.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_events/juce_events.h>

using namespace source;

struct MessageManagerLock::Impl {
    Impl() : lock() {}
    ~Impl() = default;
    juce::MessageManagerLock lock;
};

extern "C" {

MessageManagerLock::MessageManagerLock()
: lockWasGained(false)
{
    impl = std::make_shared<Impl>();
    lockWasGained = impl->lock.lockWasGained();
}

JuceInit::JuceInit() {
    shouldRunMessageThread = true;
    // start the message thread
    messageThread = std::thread([this] {
        juce::MessageManager* juceManager = juce::MessageManager::getInstance();
        juceManager->setCurrentThreadAsMessageThread();
        juce::ScopedJuceInitialiser_GUI juceInit{}; // has to be on the message thread apparently
        while (this->shouldRunMessageThread.load())
        {
            juceManager->runDispatchLoopUntil(10);                     // timeout in ms
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // keep CPU sane
        }
    });
}

JuceInit::~JuceInit() {
    shouldRunMessageThread = false;
    if (messageThread.joinable())
        messageThread.join();
}

} // extern C
