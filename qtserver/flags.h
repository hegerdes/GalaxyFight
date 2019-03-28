#ifndef FLAGS_H
#define FLAGS_H

/**
 * @brief The Bullet_shot enum stores information if the player shot or not
 */
enum Bullet_shot{
    shot, not_shot
};

/**
 * @brief The Living enum stores information about the player
 * @param alive the player is still alive and can player
 * @param dead the player is dead and the 3D mode should end
 */
enum Living{
    alive, dead
};

/**
 * @brief The Hit enum stores information if a bullet hit the player or if it did not
 */
enum Hit{
    hit, no_hit
};

/**
 * @brief The player_no enum tells the player if the is the first or second one depending on the socket
 */
enum player_no {
    first, second, winner, loser, unassigned
};

#endif // FLAGS_H
