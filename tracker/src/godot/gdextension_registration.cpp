#include "gdextension_registration.hpp"
#include <iostream>

#include "GDTracker.hpp"
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>


using namespace godot;

void initialize_libgdtracker(ModuleInitializationLevel p_level) {
  if (p_level != MODULE_INITIALIZATION_LEVEL_CORE) {
    return;
  }
  std::cerr << "INFO: ################# REGISTER GDTracker" << std::endl;
  ClassDB::register_class<GDTracker>();
}

void uninitialize_libgdtracker(ModuleInitializationLevel p_level) {
  if (p_level != MODULE_INITIALIZATION_LEVEL_CORE) {
    return;
  }
}

extern "C" {

GDExtensionBool GDE_EXPORT
libgdtracker_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                  const GDExtensionClassLibraryPtr p_library,
                  GDExtensionInitialization *r_initialization) {
  godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library,
                                                 r_initialization);

  init_obj.register_initializer(initialize_libgdtracker);
  init_obj.register_terminator(uninitialize_libgdtracker);
  init_obj.set_minimum_library_initialization_level(
      MODULE_INITIALIZATION_LEVEL_CORE);

  return init_obj.init();
}
}
