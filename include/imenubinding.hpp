#pragma once

#include <playerslot.h>

namespace DynLibUtils {
	class CModule;
} // namespace DynLibUtils

class CUtlString;

class IMenuSystem;
class IMenuProfileSystem;
class IMenuProfile;
class IMenu;


namespace plugify {
	using MenuItemHandlerFn_t = void (*)(IMenu *pMenu, CPlayerSlot aSlot, int iItem, int iItemOnPage, void *pData);

	class IMenuBinging {
	public:
		virtual bool Init(DynLibUtils::CModule &pModule, CUtlString &sError) = 0;

	public:
		virtual IMenuSystem *MenuSystem() = 0;

		virtual IMenuProfileSystem *MenuSystem_GetProfiles(IMenuSystem *pSystem) = 0;

		virtual IMenu *MenuSystem_CreateInstance(IMenuSystem *pSystem, IMenuProfile *pProfile) = 0;
		virtual bool MenuSystem_DisplayInstanceToPlayer(IMenuSystem *pSystem, IMenu *pMenu, CPlayerSlot aSlot, int iStartPostion, int nManyTimes) = 0;
		virtual bool MenuSystem_CloseInstance(IMenuSystem *pSystem, IMenu *pMenuInstance) = 0;
	
		virtual IMenuProfile *MenuProfileSystem_Get(IMenuProfileSystem *pProfileSystem, const char *pszName) = 0;

		virtual const char *Menu_GetTitle(IMenu *pMenu) = 0;
		virtual void Menu_SetTitle(IMenu *pMenu, const char *pszNewText) = 0;
		virtual unsigned char Menu_GetItemSytles(IMenu *pMenu, int iItem) = 0;
		virtual int Menu_AddItem(IMenu *pMenu, unsigned char eFlags, const char *pszContent, MenuItemHandlerFn_t pfnItemHandler, void *pData) = 0;
		virtual void Menu_RemoveItem(IMenu *pMenu, int iItem) = 0;
		virtual unsigned char Menu_GetItemControls(IMenu *pMenu) = 0;
		virtual void Menu_SetItemControls(IMenu *pMenu, unsigned char eNewControls) = 0;
		virtual int Menu_GetCurrentPosition(IMenu *pMenu, CPlayerSlot aSlot) = 0;
	}; // class IMenuBinging

	extern IMenuBinging *g_pMenuBinging;
} // namespace plugify
