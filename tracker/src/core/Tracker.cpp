#include "Tracker.hpp"

Tracker *Tracker::g_instance = nullptr;

Tracker::Tracker() { g_instance = this; }

Tracker::~Tracker() {
  std::lock_guard<std::mutex> lock(mutex);
  if (g_instance == this) {
    g_instance = nullptr;
  }
}

Tracker &Tracker::instance() {
  // If no instance exists, we might want to create one or assert.
  // For now assuming lifecycle is managed elsewhere or we return the global
  // one. If g_instance is null, this will crash. Ideally we create one on
  // demand if appropriate, but here we follow the pattern that something
  // instantiates it. However, to be safe for this refactor, let's create a
  // static local if g_instance is null? Actually, let's stick to the pattern
  // that an owner creates it, but for safety:
  if (!g_instance) {
    static Tracker static_instance; // This is risky if destructor order
                                    // matters, but safe for simple use.
    return static_instance;
  }
  return *g_instance;
}

void Tracker::update() {
  // Placeholder update logic
}

void Tracker::track(void *obj, void *context) {
  if (!obj) {
    return;
  }
  std::lock_guard<std::mutex> lock(mutex);
  if (tracked_objects.find(obj) == tracked_objects.end()) {
    tracked_objects[obj] = context;
  }
}

void Tracker::untrack(void *obj) {
  if (!obj)
    return;
  std::lock_guard<std::mutex> lock(mutex);
  auto it = tracked_objects.find(obj);
  if (it != tracked_objects.end()) {
    if (untrack_callback) {
      untrack_callback(obj, it->second);
    }
    tracked_objects.erase(it);
  }
}

bool Tracker::is_tracking(void *obj) const {
  if (!obj)
    return false;
  std::lock_guard<std::mutex> lock(mutex);
  return tracked_objects.find(obj) != tracked_objects.end();
}

void Tracker::set_context(void *obj, void *context) {
  std::lock_guard<std::mutex> lock(mutex);
  if (tracked_objects.find(obj) != tracked_objects.end()) {
    tracked_objects[obj] = context;
  }
}

void *Tracker::get_context(void *obj) const {
  std::lock_guard<std::mutex> lock(mutex);
  auto it = tracked_objects.find(obj);
  if (it != tracked_objects.end()) {
    return it->second;
  }
  return nullptr;
}

void Tracker::set_untrack_callback(
    std::function<void(void *, void *)> callback) {
  std::lock_guard<std::mutex> lock(mutex);
  untrack_callback = callback;
}
