#include "GDTracker.hpp"
#include <iostream>

GDTracker *g_gdtracker = nullptr;

void GDTracker::_bind_methods() {
  ClassDB::bind_method(D_METHOD("track_node", "node"), &GDTracker::track_node);
  ClassDB::bind_method(D_METHOD("untrack_node", "node"),
                       &GDTracker::untrack_node);
  ClassDB::bind_method(D_METHOD("is_tracking", "node"),
                       &GDTracker::is_tracking);
  ClassDB::bind_method(D_METHOD("_on_node_ready", "node"),
                       &GDTracker::_on_node_ready);
  ClassDB::bind_method(D_METHOD("_on_node_exit_tree", "node"),
                       &GDTracker::_on_node_exit_tree);

  ADD_SIGNAL(MethodInfo("node_added", PropertyInfo(Variant::OBJECT, "node")));
  ADD_SIGNAL(MethodInfo("node_removed", PropertyInfo(Variant::OBJECT, "node")));
}

GDTracker::GDTracker() { g_gdtracker = this; }

GDTracker::~GDTracker() {
  if (g_gdtracker == this) {
    g_gdtracker = nullptr;
  }
}

GDTracker *GDTracker::getInstance() { return g_gdtracker; }

void GDTracker::track_node(Node *node) {
  // std::cout << "#########TRACK_NODE: " << node << std::endl;
  ERR_FAIL_NULL(node);

  if (is_tracking(node))
    return;

  Tracker::instance().track(node);

  node->connect("ready", Callable(this, "_on_node_ready").bind(node));
  node->connect("tree_exited", Callable(this, "_on_node_exit_tree").bind(node));

  emit_signal("node_added", node);
}

void GDTracker::untrack_node(Node *node) {
  ERR_FAIL_NULL(node);

  if (!is_tracking(node))
    return;

  // Disconnect first?
  if (node->is_connected("ready", Callable(this, "_on_node_ready")))
    node->disconnect("ready", Callable(this, "_on_node_ready"));
  if (node->is_connected("tree_exited", Callable(this, "_on_node_exit_tree")))
    node->disconnect("tree_exited", Callable(this, "_on_node_exit_tree"));

  Tracker::instance().untrack(node);
  emit_signal("node_removed", node);
}

bool GDTracker::is_tracking(Node *node) const {
  ERR_FAIL_NULL_V(node, false);
  return Tracker::instance().is_tracking(node);
}

void GDTracker::_on_node_ready(Node *node) {
  // Implementation if needed
}

void GDTracker::_on_node_exit_tree(Node *node) {
  if (is_tracking(node)) {
    untrack_node(node);
  }
}

void GDTracker::set_untrack_callback(
    std::function<void(Node *, void *)> callback) {
  // Adapt the typed callback to void* callback
  Tracker::instance().set_untrack_callback([callback](void *obj, void *ctx) {
    if (callback) {
      callback(static_cast<Node *>(obj), ctx);
    }
  });
}
