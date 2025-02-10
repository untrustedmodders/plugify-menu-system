#include <plugin_export.h>

class IMenuSystem;

extern "C" PLUGIN_API IMenuSystem *MenuSystem() {
	return nullptr;
}
