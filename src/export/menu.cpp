#include <plugin_export.h>

#include <plugify/any.hpp>
#include <plugify/string.hpp>

#include <imenubinding.hpp>

PLUGIFY_WARN_PUSH()

#if defined(__clang__)
PLUGIFY_WARN_IGNORE("-Wreturn-type-c-linkage")
#elif defined(_MSC_VER)
PLUGIFY_WARN_IGNORE(4190)
#endif

/**
 * @brief Gets the menu system.
 * 
 * @return              Pointer to the menu system.
 */
extern "C" PLUGIN_API IMenuSystem *GetMenuSystem() {
	return plugify::g_pMenuBinging->MenuSystem();
}

/**
 * @brief Gets the menu profile system.
 * 
 * @param pSystem       The menu system instance.
 * 
 * @return              Pointer to the menu profile system.
 */
extern "C" PLUGIN_API IMenuProfileSystem *GetMenuSystemProfiles(IMenuSystem *pSystem) {
	return plugify::g_pMenuBinging->MenuSystem_GetProfiles(pSystem);
}

/**
 * @brief Creates a new menu instance.
 * 
 * @param pSystem       The menu system instance.
 * @param pProfile      The profile style for the menu.
 * 
 * @return              Pointer to the created menu instance.
 */
extern "C" PLUGIN_API IMenu *CreateMenu(IMenuSystem *pSystem, IMenuProfile *pProfile) {
	return plugify::g_pMenuBinging->MenuSystem_CreateInstance(pSystem, pProfile);
}

/**
 * @brief Displays a menu to a player.
 * 
 * @param pSystem       The menu system instance.
 * @param pMenu         The menu instance to display.
 * @param aSlot         The player slot to display the menu to.
 * @param iStartPostion The starting item position (use MENU_FIRST_ITEM_INDEX for default).
 * @param nManyTimes    Display duration in seconds (use MENU_TIME_FOREVER for infinite).
 * 
 * @return              true if displayed successfully, false otherwise.
 */
extern "C" PLUGIN_API bool DisplayMenu(IMenuSystem *pSystem, IMenu *pMenu, CPlayerSlot aSlot, int iStartPostion, int nManyTimes) {
	return plugify::g_pMenuBinging->MenuSystem_DisplayInstanceToPlayer(pSystem, pMenu, aSlot, iStartPostion, nManyTimes);
}

/**
 * @brief Closes a menu instance.
 * 
 * @param pSystem       The menu system instance.
 * @param pMenu         The menu instance to close.
 * 
 * @return              true if closed successfully, false otherwise.
 */
extern "C" PLUGIN_API bool CloseMenu(IMenuSystem *pSystem, IMenu *pMenu) {
	return plugify::g_pMenuBinging->MenuSystem_CloseInstance(pSystem, pMenu);
}

/**
 * @brief Retrieves a menu profile by name.
 * 
 * @param pProfileSystem The profile system instance.
 * @param sName          The name of the profile to retrieve.
 * 
 * @return              Pointer to the requested menu profile, or nullptr if not found.
 */
extern "C" PLUGIN_API IMenuProfile *GetMenuProfile(IMenuProfileSystem *pProfileSystem, plg::string sName) {
	return plugify::g_pMenuBinging->MenuProfileSystem_Get(pProfileSystem, sName.c_str());
}

/**
 * @brief Gets the title text of a menu.
 * 
 * @param pMenu         The menu instance.
 * 
 * @return              The title text as a string.
 */
extern "C" PLUGIN_API plg::string GetMenuTitle(IMenu *pMenu) {
	return plugify::g_pMenuBinging->Menu_GetTitle(pMenu);
}

/**
 * @brief Sets the title text of a menu.
 * 
 * @param pMenu         The menu instance.
 * @param sName         The new title text.
 */
extern "C" PLUGIN_API void SetMenuTitle(IMenu *pMenu, plg::string sName) {
	return plugify::g_pMenuBinging->Menu_SetTitle(pMenu, sName.c_str());
}

/**
 * @brief Gets the style flags of a menu item.
 * 
 * @param pMenu         The menu instance.
 * @param iItem         The item index to to get styles.
 * 
 * @return              The style flags as a bitmask (IMenu::ItemStyleFlags_t).
 */
extern "C" PLUGIN_API unsigned char GetMenuItemStyles(IMenu *pMenu, int iItem) {
	return plugify::g_pMenuBinging->Menu_GetItemSytles(pMenu, iItem);
}

/**
 * @brief Adds a new item to the menu.
 * 
 * @param pMenu         The menu instance.
 * @param eFlags        Style flags for the item (IMenu::ItemStyleFlags_t).
 * @param sContent      The text content of the item.
 * @param pfnItemHandler The callback function for item interactions.
 * @param pData         User-defined data to pass to the handler.
 */
extern "C" PLUGIN_API void AddMenuItem(IMenu *pMenu, unsigned char eFlags, plg::string sContent, plugify::MenuItemHandlerFn_t pfnItemHandler, void *pData) {
	return plugify::g_pMenuBinging->Menu_AddItem(pMenu, eFlags, sContent.c_str(), pfnItemHandler, pData);
}

/**
 * @brief Removes an item from the menu.
 * 
 * @param pMenu         The menu instance.
 * @param iItem         The index of the item to remove.
 */
extern "C" PLUGIN_API void RemoveMenuItem(IMenu *pMenu, int iItem) {
	return plugify::g_pMenuBinging->Menu_RemoveItem(pMenu, iItem);
}

/**
 * @brief Gets the current control flags of the menu.
 * 
 * @param pMenu         The menu instance.
 * 
 * @return              The control flags as a bitmask (IMenu::ItemControlFlags_t).
 */
extern "C" PLUGIN_API unsigned char GetMenuControls(IMenu *pMenu) {
	return plugify::g_pMenuBinging->Menu_GetItemControls(pMenu);
}

/**
 * @brief Sets the control flags for the menu.
 * 
 * @param pMenu         The menu instance.
 * @param eNewControls  New control flags (IMenu::ItemControlFlags_t).
 */
extern "C" PLUGIN_API void SetMenuControls(IMenu *pMenu, unsigned char eNewControls) {
	return plugify::g_pMenuBinging->Menu_SetItemControls(pMenu, eNewControls);
}

/**
 * @brief Gets the current cursor position for a player in the menu.
 * 
 * @param pMenu         The menu instance.
 * @param aSlot         The player slot to get a position.
 * 
 * @return              The current item position index.
 */
extern "C" PLUGIN_API int GetMenuPosition(IMenu *pMenu, CPlayerSlot aSlot) {
	return plugify::g_pMenuBinging->Menu_GetCurrentPosition(pMenu, aSlot);
}

PLUGIFY_WARN_POP()
