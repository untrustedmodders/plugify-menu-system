#pragma once

namespace DynLibUtils {
	class CModule;
} // namespace DynLibUtils

class CUtlString;

class IMenuSystem;

namespace plugify {
	class IMenuBinging {
		public:
			virtual bool Init(DynLibUtils::CModule &pModule, CUtlString &sError) = 0;

		public:
			virtual IMenuSystem *MenuSystem() = 0;
	}; // class IMenuBinging

	extern IMenuBinging *g_pMenuBinging;
} // namespace plugify
