#ifndef __HEALTHBAR__
#define __HEALTHBAR__

#include "Renderable.hpp"

#include "io/TextureFactory.hpp"

namespace asteroids
{
class HealthBar : public Renderable
{
    public:
        using Ptr = std::shared_ptr<HealthBar>;

        enum Player{PLAYER_1, PLAYER_2};

        HealthBar(Player p, int width, int height);

        HealthBar() = delete;

        void render();

    private:
    Player m_player;

    int m_width;

    int m_height;
};

}
#endif
