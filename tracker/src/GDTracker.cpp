#include "GDTracker.hpp"

void GDTracker::_bind_methods() {
    ClassDB::bind_method(D_METHOD("track_node", "node"), &GDTracker::track_node);
    ClassDB::bind_method(D_METHOD("untrack_node", "node"), &GDTracker::untrack_node);
    ClassDB::bind_method(D_METHOD("is_tracking", "node"), &GDTracker::is_tracking);
    ClassDB::bind_method(D_METHOD("_on_node_ready", "node"), &GDTracker::_on_node_ready);
    ClassDB::bind_method(D_METHOD("_on_node_exit_tree", "node"), &GDTracker::_on_node_exit_tree);

    ADD_SIGNAL(MethodInfo("node_added", PropertyInfo(Variant::OBJECT, "node")));
    ADD_SIGNAL(MethodInfo("node_removed", PropertyInfo(Variant::OBJECT, "node")));
}

GDTracker::GDTracker() {
    // Constructor implementation
}

GDTracker::~GDTracker() {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto it = tracked_nodes.begin(); it != tracked_nodes.end(); ) {
        Node* node = it->first;
        _cleanup_node(node);
        it = tracked_nodes.erase(it);
    }
}

void GDTracker::_cleanup_node(Node* node) {
    if (!node) return;

    if (untrack_callback) {
        void* context = tracked_nodes[node];
        untrack_callback(node, context);
    }

    node->disconnect("ready", Callable(this, "_on_node_ready"));
    node->disconnect("tree_exited", Callable(this, "_on_node_exit_tree"));

    emit_signal("node_removed", node);
}

void GDTracker::track_node(Node* node) {
    ERR_FAIL_NULL(node);
    std::lock_guard<std::mutex> lock(mutex);

    if (tracked_nodes.count(node)) return;

    tracked_nodes[node] = nullptr;
    node->connect("ready", Callable(this, "_on_node_ready").bind(node));
    node->connect("tree_exited", Callable(this, "_on_node_exit_tree").bind(node));

    emit_signal("node_added", node);
}

void GDTracker::untrack_node(Node* node) {
    ERR_FAIL_NULL(node);
    std::lock_guard<std::mutex> lock(mutex);

    if (!tracked_nodes.count(node)) return;

    _cleanup_node(node);
    tracked_nodes.erase(node);
}

bool GDTracker::is_tracking(Node* node) const {
    ERR_FAIL_NULL_V(node, false);
    std::lock_guard<std::mutex> lock(mutex);
    return tracked_nodes.count(node);
}

void GDTracker::_on_node_ready(Node* node) {
    // Implementation if needed
}

void GDTracker::_on_node_exit_tree(Node* node) {
    std::lock_guard<std::mutex> lock(mutex);
    if (tracked_nodes.count(node)) {
        _cleanup_node(node);
        tracked_nodes.erase(node);
    }
}