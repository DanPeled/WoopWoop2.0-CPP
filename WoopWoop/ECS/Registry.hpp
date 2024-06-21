#pragma once

#include <memory>
#include <vector>
#include <map>
#include <typeinfo>
#include <iostream>

namespace wpwp
{
    /// Forward declaration of Component struct.
    struct Component;

    /// @brief Registry class for managing component types.
    class Registry
    {
    public:
        // Function pointer type for creating instances of Component.
        using FactoryFunc = std::shared_ptr<Component> (*)();

        /** @brief Get the singleton instance of Registry.
         *
         * @return Reference to the singleton instance of Registry.
         */
        static Registry &getInstance();

        /** @brief Register a component type with its factory function.
         *
         * @param typeName The name of the component type.
         * @param factory The factory function that creates instances of the component type.
         */
        void registerType(const std::string &typeName, FactoryFunc factory);

        /** @brief Create an instance of a component by its type name.
         *
         * @param typeName The name of the component type to create.
         * @return A shared pointer to the created component instance, or nullptr if not found.
         */
        std::shared_ptr<Component> createInstance(const std::string &typeName) const;

        /** @brief Get a list of all registered component type names.
         *
         * @return A vector of strings containing the names of all registered component types.
         */
        std::vector<std::string> getAllRegistered() const;

        /** @brief Get the type_info of a component by its name.
         *
         * @param typeName The name of the component type.
         * @return The type_info of the component type, or nullptr if not found.
         */
        const std::type_info *getType(const std::string &typeName) const;

        /** @brief Get the type_info of a component by its name and convert it to a template argument.
         *
         * @param typeName The name of the component type.
         * @return The type_info of the component type as a template argument, or nullptr if not found.
         */
        template <typename T>
        static constexpr const std::type_info *getTypeAsTemplateArgument()
        {
            return &typeid(T);
        }

        /** @brief Get the type_info of a component by its name and convert it to a template argument.
         *
         * @param typeName The name of the component type.
         * @return The type_info of the component type as a template argument, or nullptr if not found.
         */
        template <typename T>
        static constexpr const std::type_info *getTypeAsTemplateArgument(const std::string &typeName)
        {
            return getInstance().getType(typeName);
        }

    private:
        /// @brief Private constructor to enforce singleton pattern.
        Registry() = default;

        /// Map of component type names to their factory functions.
        std::map<std::string, FactoryFunc> factoryMap;
    };
}