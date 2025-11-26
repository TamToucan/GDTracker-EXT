#ifndef _GDTRACKER_HPP_
#define _GDTRACKER_HPP_

#include <godot_cpp/classes/node.hpp>
#include <functional>
#include <unordered_map>
#include <mutex>

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
    mutable std::mutex mutex;
    std::unordered_map<Node*, void*> tracked_nodes;
    std::function<void(Node*, void*)> untrack_callback;

    static GDTracker* g_tracker;

    void _cleanup_node(Node* node);

protected:
    static void _bind_methods();

    // Signal handlers
    void _on_node_ready(Node* node);
    void _on_node_exit_tree(Node* node);

public:
    // Godot-exposed methods
    void track_node(Node* node);
    void untrack_node(Node* node);
    bool is_tracking(Node* node) const;

    // C++ Client API
    static GDTracker* getInstance() {
        return g_tracker;
    }

    template<typename T>
    void setContext(Node* node, T* context) {
        std::lock_guard<std::mutex> lock(mutex);
        if (! tracked_nodes.count(node)) {
            tracked_nodes[node] = static_cast<void*>(context);
        }
    }

    template<typename T>
    T* getContext(Node* node) const {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = tracked_nodes.find(node);
        return (it != tracked_nodes.end()) ? static_cast<T*>(it->second) : nullptr;
    }

    void set_untrack_callback(std::function<void(Node*, void*)> callback) {
        std::lock_guard<std::mutex> lock(mutex);
        untrack_callback = callback;
    }

    GDTracker();
    ~GDTracker() override;
};

#endif

