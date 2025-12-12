#ifndef _GDTRACKER_HPP_
#define _GDTRACKER_HPP_

#include "../core/Tracker.hpp"
#include <functional>
#include <godot_cpp/classes/node.hpp>

#ifdef _WIN32
#ifdef GDTracker_EXPORTS
#define GDTRACKER_API __declspec(dllexport) // Export when building ExtA
#else
#define GDTRACKER_API __declspec(dllimport) // Import otherwise
#endif
#else
#define GDTRACKER_API
#endif

using namespace godot;

class GDTRACKER_API GDTracker : public RefCounted {
  GDCLASS(GDTracker, RefCounted);

private:
  // mutex and tracked_nodes removed - delegating to Core Tracker

protected:
  static void _bind_methods();

  // Signal handlers
  void _on_node_ready(Node *node);
  void _on_node_exit_tree(Node *node);

public:
  // Godot-exposed methods
  void track_node(Node *node);
  void untrack_node(Node *node);
  bool is_tracking(Node *node) const;

  // C++ Client API
  static GDTracker *getInstance();

  template <typename T> void setContext(Node *node, T *context) {
    if (node) {
      void *id = reinterpret_cast<void *>(node->get_instance_id());
      Tracker::instance().set_context(id, static_cast<void *>(context));
    }
  }

  template <typename T> T *getContext(Node *node) const {
    if (!node)
      return nullptr;
    void *id = reinterpret_cast<void *>(node->get_instance_id());
    return static_cast<T *>(Tracker::instance().get_context(id));
  }

  void set_untrack_callback(std::function<void(Node *, void *)> callback);

  GDTracker();
  ~GDTracker() override;
};

#endif
