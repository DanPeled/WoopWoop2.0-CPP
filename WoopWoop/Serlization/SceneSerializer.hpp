#pragma once
#include "WoopWoop.hpp"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <filesystem>
#include "ECS/Components/Transform.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Components/Transform.hpp"
#include "ECS/Components/Graphics/Renderer.hpp"
#include "ECS/Components/Graphics/SpriteRenderer.hpp"
#include "ECS/Components/Box2D/PhysicsBody2D.hpp"
#include <memory>

#include <yaml-cpp/yaml.h>
#include "Scene.hpp"
#include <filesystem>

namespace wpwp
{
    /**
     * @brief Class for serializing and deserializing Scene objects to and from YAML files.
     */
    class SceneSerializer
    {
    public:
        /**
         * @brief Constructs a SceneSerializer object.
         * @param scene The Scene object to serialize or deserialize.
         */
        SceneSerializer(const Scene &scene);

        /**
         * @brief Serializes the scene to a YAML file.
         * @param filepath The path to the YAML file.
         */
        void serialize(const std::filesystem::path &filepath);

        void serializeRuntime(const std::filesystem::path &filepath);

        /**
         * @brief Deserializes a YAML file into the scene.
         * @param filepath The path to the YAML file.
         * @return True if deserialization is successful, false otherwise.
         */
        bool deserialize(const std::filesystem::path &filepath);

        bool deserializeRuntime(const std::filesystem::path &filepath);

    private:
        std::filesystem::path generatePath(const std::filesystem::path &filepath);

    private:
        const Scene &m_scene; // Reference to the Scene object.
    };
} // namespace wpwp

namespace YAML
{

    template <>
    struct convert<sf::Vector3f>
    {
        static Node encode(const sf::Vector3f &v)
        {
            Node node;
            node.push_back(v.x);
            node.push_back(v.y);
            node.push_back(v.z);
            return node;
        }

        static bool decode(const Node &node, sf::Vector3f &v)
        {
            if (!node.IsSequence() || node.size() < 3)
            {
                return false;
            }

            v.x = node[0].as<float>();
            v.y = node[1].as<float>();
            v.z = node[2].as<float>();

            return true;
        }
    };

    template <>
    struct convert<sf::Vector2f>
    {
        static Node encode(const sf::Vector2f &v)
        {
            Node node;
            node.push_back(v.x);
            node.push_back(v.y);
            return node;
        }

        static bool decode(const Node &node, sf::Vector2f &v)
        {
            if (!node.IsSequence() || node.size() < 2)
            {
                return false;
            }

            v.x = node[0].as<float>();
            v.y = node[1].as<float>();

            return true;
        }
    };

    template <>
    struct convert<sf::Color>
    {
        static Node encode(const sf::Color &c)
        {
            Node node;
            node.push_back(c.r);
            node.push_back(c.g);
            node.push_back(c.b);
            node.push_back(c.a);
            return node;
        }

        static bool decode(const Node &node, sf::Color &c)
        {
            if (!node.IsSequence() || node.size() < 4)
            {
                return false;
            }

            c.r = node[0].as<int>();
            c.g = node[1].as<int>();
            c.b = node[2].as<int>();
            c.a = node[3].as<int>();
            return true;
        }
    };
};