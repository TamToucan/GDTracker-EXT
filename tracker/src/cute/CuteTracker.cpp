#include "CuteTracker.hpp"

CuteTracker *g_cftracker = nullptr;

CuteTracker::CuteTracker() { g_cftracker = this; }

CuteTracker::~CuteTracker() {
  if (g_cftracker == this) {
    g_cftracker = nullptr;
  }
}

CuteTracker *CuteTracker::getInstance() { return g_cftracker; }

// Helper to get ID as void*
static void *get_id(Cute::IDX node) {
  return reinterpret_cast<void *>(node);
}

void CuteTracker::track_node(Cute::IDX node) {

  if (is_tracking(node))
    return;

  void *id = get_id(node);
  Tracker::instance().track(id);
}

void CuteTracker::untrack_node(Cute::IDX node) {

  if (!is_tracking(node))
    return;
  void *id = get_id(node);

  Tracker::instance().untrack(id);
}

bool CuteTracker::is_tracking(Cute::IDX node) const {
  return Tracker::instance().is_tracking(get_id(node));
}

void CuteTracker::set_untrack_callback(
    std::function<void(Cute::IDX , void *)> callback) {
  // Adapt the typed callback to void* callback
  Tracker::instance().set_untrack_callback([callback](void *obj, void *ctx) {
    if (callback) {
      Cute::IDX id = reinterpret_cast<Cute::IDX>(obj);
      callback(id, ctx);
    }
  });
}
