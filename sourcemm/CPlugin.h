/* ======== SourceMM ========
* Copyright (C) 2004-2005 Metamod:Source Development Team
* No warranties of any kind
*
* License: zlib/libpng
*
* Author(s): David "BAILOPAN" Anderson
* ============================
*/

#ifndef _INCLUDE_CPLUGIN_H
#define _INCLUDE_CPLUGIN_H

/**
 * @brief Implementation of Plugin API and Management routines
 * @file CPlugin.cpp
 */

#include <list>
#include <string>
#include "IPluginManager.h"
#include "oslink.h"

/**
 * History of plugin versions: (M=min, C=current)
 *    1: Initial interface
 *    2: Added GetLogTag()
 *    3: Moved GetApiVersion() to top  (2005-04-16)
 *    4: Added AllPluginsLoaded() callback (2005-04-18)
 * MC 5: Bumped version for SourceHook V4 (2005-05-01)
 */

#define PLAPI_MIN_VERSION	5

namespace SourceMM
{
	/**
	 * @brief Implements Plugin Manager API
	 */
	class CPluginManager : public ISmmPluginManager
	{	
	public:
		/**
		 * @brief Internal structure for holding plugin data
		 */
		class CPlugin
		{
		public:
			CPlugin();
		public:
			PluginId m_Id;
			std::string m_File;
			Pl_Status m_Status;
			PluginId m_Source;
			ISmmPlugin *m_API;
			HINSTANCE m_Lib;
			factories fac_list;
		};
	public:
		CPluginManager();
		~CPluginManager();
		void SetAllLoaded();
	public:
		PluginId Load(const char *file, PluginId source, bool &already, char *error, size_t maxlen);
		bool Unload(PluginId id, bool force, char *error, size_t maxlen);
		bool Pause(PluginId id, char *error, size_t maxlen);
		bool Unpause(PluginId id, char *error, size_t maxlen);
		bool UnloadAll();
	public:
		bool Query(PluginId id, const char *&file, factories *&list, Pl_Status &status, PluginId &source);

		/**
		 * @brief Finds a plugin by Id
		 *
		 * @param id Id of plugin
		 * @return CPlugin on success, NULL otherwise
		 */
		CPlugin *FindById(PluginId id);

		/**
		 * @brief Attempts to reload a failed plugin
		 *
		 * @param id Id of plugin
		 * @param error Error message buffer
		 * @param len Maximum length of buffer
		 * @return True on success, false otherwise
		 */
		bool Retry(PluginId id, char *error, size_t len);

		//Internal iterators
		std::list<SourceMM::CPluginManager::CPlugin *>::iterator _begin();
		std::list<SourceMM::CPluginManager::CPlugin *>::iterator _end();
	private:
		//These are identical internal functions for the wrappers above.
		CPlugin *_Load(const char *file, PluginId source, char *error, size_t maxlen);
		bool _Unload(CPlugin *pl, bool force, char *error, size_t maxlen);
		bool _Pause(CPlugin *pl, char *error, size_t maxlen);
		bool _Unpause(CPlugin *pl, char *error, size_t maxlen);
	private:
		PluginId m_LastId;
		std::list<CPlugin *> m_Plugins;
		bool m_AllLoaded;
	};
};

typedef std::list<SourceMM::CPluginManager::CPlugin *>::iterator PluginIter;

/** @brief Singleton for plugin manager */
extern SourceMM::CPluginManager g_PluginMngr;

#endif //_INCLUDE_CPLUGIN_H