#ifndef _TRACKER_HPP_
#define _TRACKER_HPP_

#include <functional>
#include <mutex>
#include <unordered_map>


#ifdef _WIN32
#ifdef Tracker_EXPORTS
#define TRACKER_API __declspec(dllexport)
#else
#define TRACKER_API __declspec(dllimport)
#endif
#else
#define TRACKER_API
#endif

class TRACKER_API Tracker {
private:
  mutable std::mutex mutex;
  std::unordered_map<void *, void *> tracked_objects;
  std::function<void(void *, void *)> untrack_callback;

  static Tracker *g_instance;

public:
  Tracker();
  virtual ~Tracker();

  static Tracker &instance();

  void update();

  // Generic Tracking API
  void track(void *obj, void *context = nullptr);
  void untrack(void *obj);
  bool is_tracking(void *obj) const;

  void set_context(void *obj, void *context);
  void *get_context(void *obj) const;

  void set_untrack_callback(std::function<void(void *, void *)> callback);
};

#endif // _TRACKER_HPP_
