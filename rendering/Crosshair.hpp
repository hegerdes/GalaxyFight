#ifndef __CROSSHAIR__
#define __CROSSHAIR__

#include "Renderable.hpp"
#include "io/TextureFactory.hpp"

namespace asteroids
{
class Crosshair : public Renderable
{
    public:
        using Ptr = std::shared_ptr<Crosshair>;

        Crosshair(float red, float green, float blue, int width, int height);

        Crosshair() = delete;

        void render();

    
    private:
        float r, g, b, c_dimension1, c_dimension2, offset;        
        int m_width, m_height;
};
}
#endif