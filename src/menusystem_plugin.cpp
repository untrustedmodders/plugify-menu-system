#include <menusystem_plugin.hpp>

#include <filesystem.h>
#include <tier1/keyvalues3.h>

namespace plugify
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
		m_pfnMenuSystem = pModule.GetFunctionByName("MenuSystem").RCast<decltype(m_pfnMenuSystem)>();

		if (!m_pfnMenuSystem) {
			sError.Format("Can't found \"%s\" function", "MenuSystem");

			return false;
		}

		return true;
	}

	IMenuSystem *MenuSystem_Plugin::MenuSystem() {
		return m_pfnMenuSystem();
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
			m_pLogger->DetailedFormat("Menu's module handle is %p\n", m_pMetamodPlugin.GetModuleHandle());
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

EXPOSE_PLUGIN(PLUGIN_API, &plugify::s_aMenuSystemPlugin);
