#pragma once

#include "imenubinding.hpp"
#include "plugify/cpp_plugin.hpp"
#include <plugin_export.h>

#include <tier0/interface.h>
#include <tier0/strtools.h>

#include <logger.hpp>

#include <dynlibutils/module.hpp>

#include <unordered_map>
#include <memory>
#include <mutex>

#define PLUGIFY_MENUSYSTEM_LOGGINING_COLOR Color(255, 200, 63, 255) // Orange.

#define PLUGIFY_MENUSYSTEM_BASE_PATHID "GAME"
#define PLUGIFY_MENUSYSTEM_METAMOD_BASE_DIR "addons" CORRECT_PATH_SEPARATOR_S "metamod"
#define PLUGIFY_MENUSYSTEM_METAMOD_VDF_DIR PLUGIFY_MENUSYSTEM_METAMOD_BASE_DIR CORRECT_PATH_SEPARATOR_S "menu_system.vdf"

class CUtlString;
class IFileSystem;
class KeyValues3;

namespace plugify {
	class MenuSystem_Plugin final : public plg::IPluginEntry, public IMenuBinging {
	public:
		using Interface_t = plg::IPluginEntry;
		using BingingInterface_t = IMenuBinging;

		MenuSystem_Plugin() = default;

	private: // plg::IPluginEntry
		void OnPluginStart() override;
		void OnPluginEnd() override;

	public: // plugify::IMenuBinging
		bool Init(DynLibUtils::CModule &pModule, CUtlString &sError) override;

		IMenuSystem *MenuSystem() override;

	protected:
		bool LoadVDF();
		bool ParseVDF(KeyValues3 *pRoot, CUtlString &sError);

		bool Load();

	private:
		std::unique_ptr<Logger> m_pLogger;

	private:
		DynLibUtils::CModule m_pMetamodPlugin;

		using MenuSystemFn_t = IMenuSystem *(*)();

		MenuSystemFn_t m_pfnMenuSystem;
	}; // class MenuSystem_Plugin
} // namespace plugify
