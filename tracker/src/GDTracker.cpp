#include "GDTracker.hpp"

void GDTracker::_bind_methods() {
    // Basic tracking methods
    ClassDB::bind_method(D_METHOD("track_node", "node", "context"), &GDTracker::trackNode, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("untrack_node", "node"), &GDTracker::untrackNode);
    ClassDB::bind_method(D_METHOD("get_tracked_nodes"), &GDTracker::getTrackedNodes);
    ClassDB::bind_method(D_METHOD("is_tracking", "node"), &GDTracker::isTracking);

    // Context management methods
    ClassDB::bind_method(D_METHOD("get_context", "node"), &GDTracker::getContext);
    ClassDB::bind_method(D_METHOD("set_context", "node", "context"), &GDTracker::setContext);

    // Internal signal callbacks (not exposed to GDScript)
    ClassDB::bind_method(D_METHOD("_on_node_ready", "node"), &GDTracker::_on_node_ready);
    ClassDB::bind_method(D_METHOD("_on_node_exit_tree", "node"), &GDTracker::_on_node_exit_tree);

    // Signals
    ADD_SIGNAL(MethodInfo("node_added", PropertyInfo(Variant::OBJECT, "node"), PropertyInfo(Variant::NIL, "context")));
    ADD_SIGNAL(MethodInfo("node_removed", PropertyInfo(Variant::OBJECT, "node"), PropertyInfo(Variant::NIL, "context")));
}

GDTracker::GDTracker() {
    // Initialize any variables here
}

GDTracker::~GDTracker() {
    std::lock_guard<std::mutex> lock(mutex); // Ensure thread safety

    for (auto it = tracked_nodes.begin(); it != tracked_nodes.end(); ) {
        Node2D* node = it->first;
        if (node && node->is_inside_tree()) {
            node->disconnect("ready", Callable(this, "_on_node_ready"));
            node->disconnect("tree_exited", Callable(this, "_on_node_exit_tree"));
        }
        it = tracked_nodes.erase(it);
    }
}

Array GDTracker::getTrackedNodes() const {
    Array nodes;
    for (const auto& pair : tracked_nodes) {
        nodes.append(pair.first);
    }
    return nodes;
}

bool GDTracker::isTracking(Node2D* node) const {
    std::lock_guard<std::mutex> lock(mutex);
    return node && (tracked_nodes.find(node) != tracked_nodes.end());
}

void GDTracker::trackNode(Node2D* node, const Variant& ctx) {
    if (!node || isTracking(node)) {
        return;
    }
    tracked_nodes[node] = ctx;

    // Connect using the node's own callable
    node->connect("ready", Callable(this, "_on_node_ready").bind(node));
    node->connect("tree_exited", Callable(this, "_on_node_exit_tree").bind(node));

    emit_signal("node_added", node);
}

void GDTracker::untrackNode(Node2D *node) {
    if (!node || !isTracking(node)) {
        return;
    }

    // Disconnect signals
    node->disconnect("ready", Callable(this, "_on_node_ready"));
    node->disconnect("tree_exited", Callable(this, "_on_node_exit_tree"));

    tracked_nodes.erase(node);
    emit_signal("node_removed", node);
}

Variant GDTracker::getContext(Node2D* node) const {
    auto it = tracked_nodes.find(node);
    return it != tracked_nodes.end() ? it->second : Variant();
}

void GDTracker::setContext(Node2D* node, const Variant& ctx) {
    if (isTracking(node)) {
        tracked_nodes[node] = ctx;
    }
}

void GDTracker::_on_node_ready(Node2D* node) {
    // Now node is passed directly as parameter
    if (node) {
        // Handle ready event
    }
}

void GDTracker::_on_node_exit_tree(Node2D* node) {
    if (node) {
        untrackNode(node);
    }
}


