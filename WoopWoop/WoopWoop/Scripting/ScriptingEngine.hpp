#ifndef SCRIPTING_ENGINE_HPP
#define SCRIPTING_ENGINE_HPP

#include <angelscript.h>

#include "Util/Subsystem.hpp"

namespace wpwp
{
	class ScriptingEngine : Subsystem
	{
	public:
		void init() override;

		void onStop() override;

	private:
		void configureEngine();
		static void messageCallback(const asSMessageInfo *msg, void *param);

	private:
		asIScriptEngine *m_scriptEngine = nullptr;
	};
} // namespace wpwp

#endif