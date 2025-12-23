#include <iostream>
#include <string>
#include "Tracker.hpp"

// Global to verify the callback actually ran
bool g_callback_fired = false;

int main() {
    std::cout << "--- Starting Tracker Core Interface Test ---" << std::endl;

    // 1. Get the Singleton instance
    // Note: Tracker::instance() handles the lifecycle 
    Tracker& tracker = Tracker::instance();

    // 2. Setup Test Data
    int myObject = 12345;
    std::string contextStr = "MetaData_Alpha";

    // 3. Set the Global Untrack Callback 
    tracker.set_untrack_callback([](void* obj, void* context) {
        g_callback_fired = true;
        std::cout << "[Callback] Untracking detected!" << std::endl;
        
        if (context) {
            std::string* ctx = static_cast<std::string*>(context);
            std::cout << "[Callback] Context value: " << *ctx << std::endl;
        }
    });

    // 4. Test Track and Is_Tracking
    std::cout << "[Test] Tracking object..." << std::endl;
    tracker.track(&myObject, &contextStr);

    if (tracker.is_tracking(&myObject)) {
        std::cout << "[Pass] Object is successfully tracked." << std::endl;
    } else {
        std::cerr << "[Fail] Object was not tracked." << std::endl;
        return 1;
    }

    // 5. Test Context Retrieval
    void* retrieved = tracker.get_context(&myObject);
    if (retrieved == &contextStr) {
        std::cout << "[Pass] Context matches: " << *static_cast<std::string*>(retrieved) << std::endl;
    } else {
        std::cerr << "[Fail] Context mismatch!" << std::endl;
        return 1;
    }

    // 6. Test Update Context
    std::string newContext = "MetaData_Beta";
    tracker.set_context(&myObject, &newContext);
    if (tracker.get_context(&myObject) == &newContext) {
        std::cout << "[Pass] Context update successful." << std::endl;
    }

    // 7. Test Untrack and Callback Trigger
    std::cout << "[Test] Untracking object..." << std::endl;
    tracker.untrack(&myObject);

    if (!tracker.is_tracking(&myObject) && g_callback_fired) {
        std::cout << "[Pass] Untrack successful and callback executed." << std::endl;
    } else {
        std::cerr << "[Fail] Untrack or Callback failed." << std::endl;
        return 1;
    }

    std::cout << "--- All Tracker Tests Passed ---" << std::endl;
    return 0;
}
