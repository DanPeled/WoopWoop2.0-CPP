#ifndef MOUSE_CONTROLLER_HPP
#define MOUSE_CONTROLLER_HPP
#include "WoopWoop.hpp"

namespace wpwp
{
    struct MouseController : wpwp::Component
    {
    public:
        ~MouseController() override;
        void update() override;
        void start() override;

        std::string getName() const override { return "MouseController"; }
    };

    WREGISTER(MouseController)

}
#endif
