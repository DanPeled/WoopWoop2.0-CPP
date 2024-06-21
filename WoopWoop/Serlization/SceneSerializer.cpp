#include "SceneSerializer.hpp"

namespace wpwp
{
    YAML::Emitter &operator<<(YAML::Emitter &out, const sf::Vector3f v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter &operator<<(YAML::Emitter &out, const sf::Vector2f v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter &operator<<(YAML::Emitter &out, const sf::Color c)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << (int)c.r << (int)c.g << (int)c.b << (int)c.a << YAML::EndSeq;
        return out;
    }

    static void serializeEntity(YAML::Emitter &out, std::shared_ptr<Entity> entity)
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Entity";
        out << YAML::Value << entity->getName();

        for (auto c : entity->getComponents())
        {
            out << YAML::Key << c->getName();
            out << YAML::BeginMap;
            if (auto transform = std::dynamic_pointer_cast<Transform>(c))
            {
                out << YAML::Key << "Translation";
                out << YAML::BeginMap;
                out << YAML::Key << "Position" << YAML::Value << *transform->getPosition();
                out << YAML::Key << "Rotation" << YAML::Value << *transform->getRotation();
                out << YAML::Key << "Scale" << YAML::Value << *transform->getScale();
                out << YAML::EndMap;

                out << YAML::Key << "Children";
                out << YAML::Flow << YAML::BeginSeq;
                for (auto child : transform->getChildren())
                {
                    out << child->getName();
                }
                out << YAML::EndSeq;
            }

            else if (auto spriteRenderer = std::dynamic_pointer_cast<SpriteRenderer>(c))
            {
                out << YAML::Key << "SpritePath" << YAML::Value << spriteRenderer->getFilePath();
            }

            else if (auto physicsBody = std::dynamic_pointer_cast<PhysicsBody2D>(c))
            {
                out << YAML::Key << "Body Type" << YAML::Value << (int)physicsBody->body->GetType();

                float density = physicsBody->body->GetFixtureList()[0].GetDensity();
                float friction = physicsBody->body->GetFixtureList()[0].GetFriction();
                float restitution = physicsBody->body->GetFixtureList()[0].GetRestitution();

                out << YAML::Key << "Density" << YAML::Value << density;
                out << YAML::Key << "Friction" << YAML::Value << friction;
                out << YAML::Key << "Restitution" << YAML::Value << restitution;
                out << YAML::Key << "Rotation Fixed" << YAML::Value << physicsBody->getRotationFixed();
            }

            else if (auto camera = std::dynamic_pointer_cast<Camera2D>(c))
            {
                out << YAML::Key << "IsMain" << YAML::Value << camera->isMain;
            }

            out << YAML::EndMap; // Transform

            if (auto renderer = std::dynamic_pointer_cast<Renderer>(c))
            {
                out << YAML::Key << "Renderer";
                out << YAML::BeginMap;
                out << YAML::Key << "Material";
                out << YAML::BeginMap;
                out << YAML::Key << "Color" << YAML::Value << renderer->material.color;
                out << YAML::EndMap;
                out << YAML::EndMap; // Renderer
            }
        }

        out << YAML::EndMap; // Entity
    }

    SceneSerializer::SceneSerializer(const Scene &scene) : m_scene(scene)
    {
    }

    void SceneSerializer::serialize(const std::filesystem::path &filepath)
    {
        std::filesystem::path path = generatePath(filepath);
        std::filesystem::create_directories(path.parent_path());

        std::ofstream ofs(path);
        if (!ofs)
        {
            ERROR("Error creating file: ", path);
            return;
        }

        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene";
        out << YAML::Value << (m_scene.getName().empty() ? "Unnamed" : m_scene.getName());

        out << YAML::Key << "Entities";
        out << YAML::Value << YAML::BeginSeq;
        for (auto entity : Entity::getAllEntities())
        {
            if (entity)
            {
                serializeEntity(out, entity);
            }
        }
        out << YAML::EndSeq;
        out << YAML::EndMap;

        ofs << out.c_str();
    }

    void SceneSerializer::serializeRuntime(const std::filesystem::path &filepath)
    {
    }

    bool SceneSerializer::deserialize(const std::filesystem::path &filepath)
    {
        if (!Engine::getInstance())
        {
            ERROR("NO ENGINE INSTANCE");
            return false;
        }

        std::filesystem::path path = generatePath(filepath);
        LOG("Loading scene from: ", path);

        LOG("\nSTARTED DESERIALIZATION\n");

        std::ifstream stream(path);
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());

        if (!data["Scene"])
        {
            return false;
        }

        std::string sceneName = data["Scene"].as<std::string>();

        Engine::getInstance()->getCurrentScene()->m_name = sceneName;

        auto entities = data["Entities"];
        if (entities)
        {
            for (auto entity : entities)
            {
                std::string name = entity["Entity"].as<std::string>();
                std::shared_ptr<Entity> deserializedEntity = Entity::getEntityWithName(name);

                if (!deserializedEntity)
                {
                    LOG("No entity named: ", name, ", generating...");
                    deserializedEntity = Entity::createEntity(name, sf::Vector3f(0, 0, 0));

                    if (deserializedEntity)
                    {
                        LOG("Entity created: ", deserializedEntity->getName());
                        Entity::instantiate(deserializedEntity);
                    }
                    else
                    {
                        ERROR("Failed to create entity: ", name);
                        continue; // Skip to the next entity
                    }
                }

                // More detailed logging
                LOG("Processing entity: ");

                {
                    auto transformComponent = entity["Transform"];
                    if (transformComponent)
                    {
                        auto transform = deserializedEntity->getComponent<Transform>();
                        if (transform)
                        {
                            transform->setPosition(transformComponent["Translation"]["Position"].as<sf::Vector3f>());
                            transform->setScale(transformComponent["Translation"]["Scale"].as<sf::Vector3f>());
                            transform->setRotation(transformComponent["Translation"]["Rotation"].as<sf::Vector3f>());

                            auto children = transformComponent["Children"];
                            for (int i = 0; i < children.size(); i++)
                            {
                                transform->addChild(children[i].as<std::string>());
                            }
                        }
                        else
                        {
                            ERROR("FAILED CONFIGURING TRANSFORM FOR: ", deserializedEntity->getName());
                        }
                    } // transform
                }
                {
                    if (auto spriteRendererComponent = entity["SpriteRenderer"])
                    {
                        auto spriteRenderer = deserializedEntity->getOrAddComponent<SpriteRenderer>().get();
                        if (spriteRenderer)
                        {
                            std::string spritePath = spriteRendererComponent["SpritePath"].as<std::string>();
                            spriteRenderer->loadSprite(spritePath);
                        }
                    } // Sprite renderer

                    else if (auto circleRendererComponent = entity["CircleRenderer"])
                    {
                        auto circleRenderer = deserializedEntity->getOrAddComponent<CircleRenderer>();
                    } // Circle renderer
                }

                {
                    if (auto physicsBodyComponent = entity["PhysicsBody2D"])
                    {
                        auto physicsBody = deserializedEntity->getOrAddComponent<PhysicsBody2D>();
                        if (physicsBody)
                        {
                            physicsBody->body->SetType(static_cast<b2BodyType>(physicsBodyComponent["Body Type"].as<int>()));

                            physicsBody->body->GetFixtureList()[0].SetRestitution(physicsBodyComponent["Restitution"].as<float>());
                            physicsBody->body->GetFixtureList()[0].SetDensity(physicsBodyComponent["Density"].as<float>());
                            physicsBody->body->GetFixtureList()[0].SetFriction(physicsBodyComponent["Friction"].as<float>());

                            physicsBody->setRotationFixed(physicsBodyComponent["Rotation Fixed"].as<bool>());
                        }
                    }
                }

                {
                    if (auto cameraComponentData = entity["Camera2D"])
                    {
                        auto camera = deserializedEntity->getOrAddComponent<Camera2D>();

                        if (camera)
                        {
                            if (auto isMain = cameraComponentData["IsMain"])
                            {
                                camera->isMain = isMain.as<bool>();
                            }
                        }
                    }
                }

                auto rendererComponent = entity["Renderer"];
                if (rendererComponent)
                {
                    auto renderer = deserializedEntity->getOrAddComponent<Renderer>();
                    if (renderer)
                    {
                        renderer->material.color = rendererComponent["Material"]["Color"].as<sf::Color>();
                    }
                } // renderer (put after all other renderers)

                for (auto it = entity.begin(); it != entity.end(); ++it)
                {
                    std::string compTypeName = it->first.as<std::string>();
                    if (compTypeName == "Entity")
                        continue;

                    LOG("Added ", compTypeName);

                    if (deserializedEntity->getComponent(compTypeName))
                    {
                        continue; // prevent the same component to be added twice if it already exists
                    }

                    else
                    {
                        auto comp = Registry::getInstance().createInstance(compTypeName);
                        if (comp)
                        {
                            deserializedEntity->addComponent(std::move(comp));
                        }
                    }
                }

                LOG("Finished processing entity: ");
            }
            LOG("ENDED DESERIALIZATION SUCCESSFULLY");
            return true;
        }
        return false;
    }

    bool SceneSerializer::deserializeRuntime(const std::filesystem::path &filepath)
    {
        return true;
    }

    std::filesystem::path SceneSerializer::generatePath(const std::filesystem::path &filepath)
    {
        return std::filesystem::path(std::string("./data/scenes/") + std::string(filepath.c_str()) + std::string(".scene"));
    }
}