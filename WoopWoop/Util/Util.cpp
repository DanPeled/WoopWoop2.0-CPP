#include "Util.hpp"
namespace wpwp
{
    std::random_device Util::rd;
    std::mt19937 Util::gen(rd());
    std::uniform_int_distribution<> Util::dis(0, 15);
    std::uniform_int_distribution<> Util::dis2(8, 11);
    float Util::m_deltaTime;

    std::string Util::generate_uuid_v4()
    {
        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++)
        {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 4; i++)
        {
            ss << dis(gen);
        }
        ss << "-4";
        for (i = 0; i < 3; i++)
        {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++)
        {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 12; i++)
        {
            ss << dis(gen);
        };
        return ss.str();
    }
}