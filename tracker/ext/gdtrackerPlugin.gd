@tool
extends EditorPlugin


func _enter_tree():
	add_custom_type("GDTracker", "Object", preload("gdtracker.gd"),preload("res://icon.svg"))
	pass


func _exit_tree():
	remove_custom_type("GDTracker")
