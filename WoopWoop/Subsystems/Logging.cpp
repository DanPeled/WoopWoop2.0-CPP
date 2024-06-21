#include "Logging.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

std::vector<wpwp::Log> wpwp::Logging::s_logs{};
std::map<wpwp::Log, int> wpwp::Logging::s_logMapCount{};

void wpwp::Logging::init()
{
    std::ofstream ofs;
    ofs.open("logs.log", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

void wpwp::Logging::update()
{
}

void wpwp::Logging::renderLogs()
{
    const int maxLogsToRender = 1000;
    int logsCount = s_logMapCount.size();
    int start = std::max(0, logsCount - maxLogsToRender);

    auto it = s_logMapCount.begin();
    std::advance(it, start);

    for (; it != s_logMapCount.end(); ++it)
    {
        const auto &[log, count] = *it;

        ImVec4 col{1, 1, 1, 255};
        switch (log.type)
        {
        case wpwp::LogType::Warning:
            col = {1, 1, 0, 255}; // Yellow
            break;
        case wpwp::LogType::Error:
            col = {1, 0, 0, 255}; // Red
            break;
        default:
            break;
        }
        ImGui::PushStyleColor(ImGuiCol_Text, col);

        ImGui::Text("%s (%d) %s", log.prefix.c_str(), count, log.message.c_str());

        ImGui::PopStyleColor();
    }
}

void wpwp::Logging::clear()
{
    s_logMapCount.clear();
    s_logs.clear();
}

void wpwp::Logging::onStop()
{
}

bool wpwp::Log::operator<(const Log &other) const
{
    return std::tie(message, type) < std::tie(other.message, other.type);
}
