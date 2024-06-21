#include "ScriptingEngine.hpp"
#include "WoopWoop.hpp"

// TODO: finish all of this

void wpwp::ScriptingEngine::init()
{
	m_scriptEngine = asCreateScriptEngine();

	if (!m_scriptEngine)
	{
		ERROR("Failed to create script engine");

		this->isEnabled = false;

		return;
	}

	configureEngine();
}

void wpwp::ScriptingEngine::onStop()
{
	m_scriptEngine->ShutDownAndRelease();
}

void wpwp::ScriptingEngine::configureEngine()
{
	int r = m_scriptEngine->SetMessageCallback(asFunctionPtr(messageCallback), 0, asCALL_CDECL);
}

void wpwp::ScriptingEngine::messageCallback(const asSMessageInfo *msg, void *param)
{
	const char *type = "ERROR";
	if (msg->type == asMSGTYPE_WARNING)
		type = "WARNING";
	else if (msg->type == asMSGTYPE_INFORMATION)
		type = "INFO";
	printf("%s (%d, %d) : %s\n", type, msg->row, msg->col, msg->message);
}
