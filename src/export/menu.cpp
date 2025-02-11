#include <plugin_export.h>
#include <imenubinding.hpp>

extern "C" PLUGIN_API IMenuSystem *MenuSystem() {
	return plugify::g_pMenuBinging->MenuSystem();
}
