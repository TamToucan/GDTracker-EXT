#ifndef _CuteTRACKER_HPP_
#define _CuteTRACKER_HPP_
#include <functional>

#include "../core/Tracker.hpp"

#ifdef _WIN32
#ifdef CuteTracker_EXPORTS
#define CuteTRACKER_API __declspec(dllexport) // Export when building ExtA
#else
#define CuteTRACKER_API __declspec(dllimport) // Import otherwise
#endif
#else
#define CuteTRACKER_API
#endif

namespace Cute {
	typedef uint64_t IDX;
}

class CuteTRACKER_API CuteTracker {

public:
  // Cute-exposed methods
  void track_node(Cute::IDX node);
  void untrack_node(Cute::IDX node);
  bool is_tracking(Cute::IDX node) const;

  // C++ Client API
  static CuteTracker *getInstance();

  template <typename T> void setContext(Cute::IDX node, T *context) {
    if (node) {
      void *id = reinterpret_cast<void *>(node);
      Tracker::instance().set_context(id, static_cast<void *>(context));
    }
  }

  template <typename T> T *getContext(Cute::IDX node) const {
    if (!node)
      return nullptr;
    void *id = reinterpret_cast<void *>(node);
    return static_cast<T *>(Tracker::instance().get_context(id));
  }

  void set_untrack_callback(std::function<void(Cute::IDX, void *)> callback);

  CuteTracker();
  ~CuteTracker();
};

#endif

