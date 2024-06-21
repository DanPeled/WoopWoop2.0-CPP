#ifndef SUBSYSTEM_HPP
#define SUBSYSTEM_HPP

namespace wpwp
{
    /**
     * @brief Base class for subsystems in the game engine.
     */
    class Subsystem
    {
    public:
        /**
         * @brief Initializes the subsystem.
         */
        virtual void init() { return; };

        /**
         * @brief Updates the subsystem.
         */
        virtual void update() { return; };

        /**
         * @brief Cleans up the subsystem.
         */
        virtual void onStop() { return; };

        bool isEnabled = true; // Flag indicating whether the subsystem is enabled.
    };
} // namespace wpwp

#endif // SUBSYSTEM_HPP
