#ifndef BALL_COMP_HPP
#define BALL_COMP_HPP
#include "WoopWoop.hpp"
namespace wpwp
{
    struct BallComponent : wpwp::Component
    {
    public:
        void start() override;
        void update() override;
        std::string getName() const override { return "BallComponent"; }
        void onDrawGUI() override;

    private:
        void changeColor();

    private:
        sf::Vector2f m_velocity;
        std::shared_ptr<wpwp::CircleRenderer> m_renderer;
    };

    WREGISTER(BallComponent)

} // namespace demo

#endif