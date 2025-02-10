#pragma once

#include "plugify/cpp_plugin.hpp"
#include <plugin_export.h>

#include <unordered_map>
#include <memory>
#include <mutex>

namespace plugify {
	class MenuSystemPlugin final : public plg::IPluginEntry {
	public:
		MenuSystemPlugin() = default;
		~MenuSystemPlugin() = default;

	private:
		void OnPluginStart() override;
		void OnPluginEnd() override;
	};
}