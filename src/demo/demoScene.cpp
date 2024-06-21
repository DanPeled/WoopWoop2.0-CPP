#include "demoScene.hpp"
#include "components/mouseController.hpp"
#include "components/BallComponent.hpp"

using namespace wpwp;
namespace wpwp
{
    std::shared_ptr<Entity> e = wpwp::Entity::createEntity("Circle", sf::Vector3f(30, 30, 0));
    std::shared_ptr<Entity> e1 = wpwp::Entity::createEntity("Player", sf::Vector3f(800, 30, 0));

    void MainScene::load()
    {
        // this->m_name = "Demo Scene";

        // e->addComponent<CircleRenderer>();
        // e->transform->setScale(sf::Vector3f(100, 100, 0));
        // e->addComponent<BallComponent>();
        // Entity::instantiate(e);

        // e1->transform->setScale(sf::Vector3f(100, 100, 0));
        // e1->addComponent<SpriteRenderer>();
        // std::shared_ptr<SpriteRenderer> sp = e1->getComponent<SpriteRenderer>();
        // e1->addComponent<MouseController>();
        // sp->loadSprite("assets/test.png");
        // Entity::instantiate(e1);

        // e->transform->addChild(e1);
    }
}