@tool
extends EditorPlugin

var tracker_script = preload("res://addons/gdtracker/gdtracker.gd")
var icon = preload("res://icon.svg")

func _enter_tree():
    add_custom_type("GDTracker","RefCounted", tracker_script, icon)

func _exit_tree():
    remove_custom_type("GDTracker")

