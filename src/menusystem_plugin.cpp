#include <menusystem_plugin.hpp>

#include <filesystem.h>
#include <tier1/keyvalues3.h>

#define PLUGIFY_BINDING(module, error, functionName, res) \
	res = module.GetFunctionByName(functionName).RCast<decltype(res)>(); \
 \
	if (!res) { \
		sError.Format("Can't found \"%s\" function", functionName); \
 \
		return false; \
	}

namespace menu
{
	static MenuSystem_Plugin s_aMenuSystemPlugin;
	MenuSystem_Plugin *g_pMenuSystemPlugin = &s_aMenuSystemPlugin;
	IMenuBinging *g_pMenuBinging = static_cast<IMenuBinging *>(g_pMenuSystemPlugin);

	void MenuSystem_Plugin::OnPluginStart() {
		m_pLogger = std::make_unique<Logger>(GetName().data(), [](LoggingChannelID_t nTagChannelID)
		    {
		    	LoggingSystem_AddTagToChannel(nTagChannelID, s_aMenuSystemPlugin.Interface_t::GetName().data());
		    }, 0, LV_DEFAULT, PLUGIFY_MENUSYSTEM_LOGGINING_COLOR);

		LoadVDF();
		Load();
	}

	void MenuSystem_Plugin::OnPluginEnd() {
		m_pLogger.reset();
	}

	bool MenuSystem_Plugin::Init(DynLibUtils::CModule &pModule, CUtlString &sError) {
		PLUGIFY_BINDING(pModule, sError, "MenuSystem", m_pfnMenuSystem);

		PLUGIFY_BINDING(pModule, sError, "MenuSystem_GetProfiles", m_pfnMenuSystem_GetProfiles);
		PLUGIFY_BINDING(pModule, sError, "MenuSystem_CreateInstance", m_pfnMenuSystem_CreateInstance);
		PLUGIFY_BINDING(pModule, sError, "MenuSystem_DisplayInstanceToPlayer", m_pfnMenuSystem_DisplayInstanceToPlayer);
		PLUGIFY_BINDING(pModule, sError, "MenuSystem_CloseInstance", m_pfnMenuSystem_CloseInstance);

		PLUGIFY_BINDING(pModule, sError, "MenuProfileSystem_Get", m_pfnMenuProfileSystem_Get);

		PLUGIFY_BINDING(pModule, sError, "Menu_GetTitle", m_pfnMenu_GetTitle);
		PLUGIFY_BINDING(pModule, sError, "Menu_SetTitle", m_pfnMenu_SetTitle);

		PLUGIFY_BINDING(pModule, sError, "Menu_GetItemStyles", m_pfnMenu_GetItemStyles);
		PLUGIFY_BINDING(pModule, sError, "Menu_AddItem", m_pfnMenu_AddItem);
		PLUGIFY_BINDING(pModule, sError, "Menu_RemoveItem", m_pfnMenu_RemoveItem);

		PLUGIFY_BINDING(pModule, sError, "Menu_GetItemControls", m_pfnMenu_GetItemControls);
		PLUGIFY_BINDING(pModule, sError, "Menu_SetItemControls", m_pfnMenu_SetItemControls);

		PLUGIFY_BINDING(pModule, sError, "Menu_GetCurrentPosition", m_pfnMenu_GetCurrentPosition);

		return true;
	}

	IMenuSystem *MenuSystem_Plugin::MenuSystem() {
		return m_pfnMenuSystem();
	}

	IMenuProfileSystem *MenuSystem_Plugin::MenuSystem_GetProfiles(IMenuSystem *pSystem) {
		return m_pfnMenuSystem_GetProfiles(pSystem);
	}

	// MenuSystem_Plugin::

	IMenu *MenuSystem_Plugin::MenuSystem_CreateInstance(IMenuSystem *pSystem, IMenuProfile *pProfile) {
		return m_pfnMenuSystem_CreateInstance(pSystem, pProfile);
	}

	bool MenuSystem_Plugin::MenuSystem_DisplayInstanceToPlayer(IMenuSystem *pSystem, IMenu *pMenu, CPlayerSlot aSlot, int iStartPostion, int nManyTimes) {
		return m_pfnMenuSystem_DisplayInstanceToPlayer(pSystem, pMenu, aSlot, iStartPostion, nManyTimes);
	}

	bool MenuSystem_Plugin::MenuSystem_CloseInstance(IMenuSystem *pSystem, IMenu *pMenu) {
		return m_pfnMenuSystem_CloseInstance(pSystem, pMenu);
	}

	IMenuProfile *MenuSystem_Plugin::MenuProfileSystem_Get(IMenuProfileSystem *pProfileSystem, const char *pszName) {
		return m_pfnMenuProfileSystem_Get(pProfileSystem, pszName);
	}

	const char *MenuSystem_Plugin::Menu_GetTitle(IMenu *pMenu) {
		return m_pfnMenu_GetTitle(pMenu);
	}

	void MenuSystem_Plugin::Menu_SetTitle(IMenu *pMenu, const char *pszNewText) {
		return m_pfnMenu_SetTitle(pMenu, pszNewText);
	}

	unsigned char MenuSystem_Plugin::Menu_GetItemSytles(IMenu *pMenu, int iItem) {
		return m_pfnMenu_GetItemStyles(pMenu, iItem);
	}

	int MenuSystem_Plugin::Menu_AddItem(IMenu *pMenu, unsigned char eFlags, const char *pszContent, MenuItemHandlerFn_t pfnItemHandler, void *pData) {
		return m_pfnMenu_AddItem(pMenu, eFlags, pszContent, pfnItemHandler, pData);
	}

	void MenuSystem_Plugin::Menu_RemoveItem(IMenu *pMenu, int iItem) {
		return m_pfnMenu_RemoveItem(pMenu, iItem);
	}

	unsigned char MenuSystem_Plugin::Menu_GetItemControls(IMenu *pMenu) {
		return m_pfnMenu_GetItemControls(pMenu);
	}

	void MenuSystem_Plugin::Menu_SetItemControls(IMenu *pMenu, unsigned char eNewControls) {
		return m_pfnMenu_SetItemControls(pMenu, eNewControls);
	}

	int MenuSystem_Plugin::Menu_GetCurrentPosition(IMenu *pMenu, CPlayerSlot aSlot) {
		return m_pfnMenu_GetCurrentPosition(pMenu, aSlot);
	}

	bool MenuSystem_Plugin::LoadVDF() {
		KeyValues3 aRootKV;

		CUtlString sError;

		if (!LoadKV3FromKV1File(&aRootKV, &sError, PLUGIFY_MENUSYSTEM_BASE_PATHID, PLUGIFY_MENUSYSTEM_METAMOD_VDF_DIR, KV1TEXT_ESC_BEHAVIOR_UNK1) || !ParseVDF(&aRootKV, sError)) {
			m_pLogger->ErrorFormat("Failed to load/parse \"%s\" (%s): %s\n", PLUGIFY_MENUSYSTEM_METAMOD_VDF_DIR, PLUGIFY_MENUSYSTEM_BASE_PATHID, sError.Get());

			return false;
		}

		return true;
	}

	bool MenuSystem_Plugin::ParseVDF(KeyValues3 *pRoot, CUtlString &sError) {
		// CKV3MemberName sRootKey = "Metamod Plugin";

		// if (!(pRoot = pRoot->FindMember(sRootKey))) {
		// 	sError.Format("Header key \"%s\" not found", sRootKey.GetString());

		// 	return false;
		// }

		CKV3MemberName sFileKey = "file";

		KeyValues3 *pFilename = pRoot->FindMember(sFileKey);

		if (!pFilename) {
			sError.Format("Key \"%s\" not found", sFileKey.GetString());

			return false;
		}

		const char *pszFilename = pFilename->GetString();

		if (!m_pMetamodPlugin.InitFromName(pszFilename)) {
			sError.Format("Menu System library not found by \"%s\" filename", pszFilename);

			return false;
		}

		if(m_pLogger->IsChannelEnabled(LV_DETAILED)) {
			m_pLogger->DetailedFormat("Menu's module handle is %p\n", m_pMetamodPlugin.GetHandle());
		}

		return true;
	}

	bool MenuSystem_Plugin::Load() {
		CUtlString sError;

		if (!Init(m_pMetamodPlugin, sError)) {
			m_pLogger->ErrorFormat("Failed to binding Menu System plugin: %s\n", sError.Get());

			return false;
		}

		return true;
	}
}; // namespace plugify

EXPOSE_PLUGIN(PLUGIN_API, menu::MenuSystem_Plugin, &menu::s_aMenuSystemPlugin);
