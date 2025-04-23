#ifndef GD_TRACKER_H
#define GD_TRACKER_H

#include <mutex>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/binder_common.hpp>

using namespace godot;

class GDTracker : public RefCounted {
    GDCLASS(GDTracker, RefCounted);

private:
    mutable std::mutex mutex;

    // Store both nodes and associated context data
    std::unordered_map<Node2D*, Variant> tracked_nodes;

protected:
    static void _bind_methods();

public:
    void trackNode(Node2D* node, const Variant& ctx = Variant());
    void untrackNode(Node2D *node);
    Array getTrackedNodes() const;
    bool isTracking(Node2D *node) const;

    // Context management
    Variant getContext(Node2D* node) const;
    void setContext(Node2D* node, const Variant& ctx);

    // Signal callbacks
    void _on_node_ready(Node2D* node);
    void _on_node_exit_tree(Node2D* node);

    GDTracker();
    ~GDTracker();
};

#endif // GD_TRACKER_H

