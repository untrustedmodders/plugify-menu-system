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

		IMenuProfileSystem *MenuSystem_GetProfiles(IMenuSystem *pSystem) override;

		IMenu *MenuSystem_CreateInstance(IMenuSystem *pSystem, IMenuProfile *pProfile) override;
		bool MenuSystem_DisplayInstanceToPlayer(IMenuSystem *pSystem, IMenu *pMenu, CPlayerSlot aSlot, int iStartPostion, int nManyTimes) override;
		bool MenuSystem_CloseInstance(IMenuSystem *pSystem, IMenu *pMenuInstance) override;

		IMenuProfile *MenuProfileSystem_Get(IMenuProfileSystem *pProfileSystem, const char *pszName) override;

		const char *Menu_GetTitle(IMenu *pMenu) override;
		void Menu_SetTitle(IMenu *pMenu, const char *pszNewText) override;
		unsigned char Menu_GetItemSytles(IMenu *pMenu, int iItem) override;
		void Menu_AddItem(IMenu *pMenu, unsigned char eFlags, const char *pszContent, MenuItemHandlerFn_t pfnItemHandler, void *pData) override;
		void Menu_RemoveItem(IMenu *pMenu, int iItem) override;
		unsigned char Menu_GetItemControls(IMenu *pMenu) override;
		void Menu_SetItemControls(IMenu *pMenu, unsigned char eNewControls) override;
		int Menu_GetCurrentPosition(IMenu *pMenu, CPlayerSlot aSlot) override;

	protected:
		bool LoadVDF();
		bool ParseVDF(KeyValues3 *pRoot, CUtlString &sError);

		bool Load();

	private:
		std::unique_ptr<Logger> m_pLogger;

	private:
		DynLibUtils::CModule m_pMetamodPlugin;

		using MenuSystemFn_t = IMenuSystem *(*)();

		using MenuSystem_GetProfilesFn_t = IMenuProfileSystem *(*)(IMenuSystem *pSystem);

		using MenuSystem_CreateInstanceFn_t = IMenu *(*)(IMenuSystem *pSystem, IMenuProfile *pProfile);
		using MenuSystem_DisplayInstanceToPlayerFn_t = bool (*)(IMenuSystem *pSystem, IMenu *pMenu, CPlayerSlot aSlot, int iStartPostion, int nManyTimes);
		using MenuSystem_CloseInstanceFn_t = bool (*)(IMenuSystem *pSystem, IMenu *pMenuInstance);

		using MenuProfileSystem_GetFn_t = IMenuProfile *(*)(IMenuProfileSystem *pProfileSystem, const char *pszName);

		using Menu_GetTitleFn_t = const char *(*)(IMenu *pMenu);
		using Menu_SetTitleFn_t = void (*)(IMenu *pMenu, const char *pszNewText);
		using Menu_GetItemStylesFn_t = unsigned char (*)(IMenu *pMenu, int iItem);
		using Menu_AddItemFn_t = void (*)(IMenu *pMenu, unsigned char eFlags, const char *pszContent, MenuItemHandlerFn_t pfnItemHandler, void *pData);
		using Menu_RemoveItemFn_t = void (*)(IMenu *pMenu, int iItem);

		using Menu_GetItemControlsFn_t = unsigned char (*)(IMenu *pMenu);
		using Menu_SetItemControlsFn_t = void (*)(IMenu *pMenu, unsigned char eNewControls);

		using Menu_GetCurrentPositionFn_t = int (*)(IMenu *pMenu, CPlayerSlot aSlot);

		MenuSystemFn_t m_pfnMenuSystem;

		MenuSystem_GetProfilesFn_t m_pfnMenuSystem_GetProfiles;

		MenuSystem_CreateInstanceFn_t m_pfnMenuSystem_CreateInstance;
		MenuSystem_DisplayInstanceToPlayerFn_t m_pfnMenuSystem_DisplayInstanceToPlayer;
		MenuSystem_CloseInstanceFn_t m_pfnMenuSystem_CloseInstance;

		MenuProfileSystem_GetFn_t m_pfnMenuProfileSystem_Get;

		Menu_GetTitleFn_t m_pfnMenu_GetTitle;
		Menu_SetTitleFn_t m_pfnMenu_SetTitle;

		Menu_GetItemStylesFn_t m_pfnMenu_GetItemStyles;
		Menu_AddItemFn_t m_pfnMenu_AddItem;
		Menu_RemoveItemFn_t m_pfnMenu_RemoveItem;

		Menu_GetItemControlsFn_t m_pfnMenu_GetItemControls;
		Menu_SetItemControlsFn_t m_pfnMenu_SetItemControls;

		Menu_GetCurrentPositionFn_t m_pfnMenu_GetCurrentPosition;
	}; // class MenuSystem_Plugin
} // namespace plugify
