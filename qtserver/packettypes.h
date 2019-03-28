#ifndef PACKETTYPES_H
#define PACKETTYPES_H
/**
 * @brief The PacketType enum contains each packageType so that the Client and the server can interpret every package
 */
enum PacketType { init_3D, update_3D_C, update_3D_S, end_3D, start_2D, ready_T, game_start, con_lost};

#endif // PACKETTYPES_H
