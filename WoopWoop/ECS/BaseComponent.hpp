#ifndef BASE_COMPONENT_HPP
#define BASE_COMPONENT_HPP
namespace wpwp
{
    /**
     * Dont touch this struct
    */
    struct BaseComponent
    {
        virtual void onDrawGUI() = 0;
    };
}

#endif