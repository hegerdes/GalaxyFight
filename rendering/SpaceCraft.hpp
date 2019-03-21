/*
 *  SpaceCraft.hpp
 *
 *  Created on: Nov. 24 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef __SpaceCraft_HPP__
#define __SpaceCraft_HPP__

#include "Renderable.hpp"
#include "TriangleMesh.hpp"
#include "physics/Transformable.hpp"
#include "math/Vector.hpp"
#include "physics/ParticleEngine.hpp"

#include <memory>

namespace asteroids
{
/**
 * @brief A class to render a cicle to the screen
 * 
 */
class SpaceCraft : public Renderable, public Transformable
{
public:

    using Ptr = std::shared_ptr<SpaceCraft>;

    /**
     * @brief Construct a new SpaceCraft object
     * 
     * @param filename the filename of the Trianglemesh
     * @param position the start position
     * @param movespeed movespeed for the key handling
     * @param rotatespeed rotatespeed for the key handling
     */
    SpaceCraft(const std::string& filename, const Vector3f& position, float movespeed, float rotatespeed);

    /// dtor
    virtual ~SpaceCraft();

    /**
     * @brief moves and rotates the mesh
     * 
     * @param keyStates the SDL Keyinput
     */
   // void handleKeyInput(const Uint8* keyStates);

    /// renders the SpaceCraft
    virtual void render() override;

    /**
     * @brief check if the SpaceCraft has a valid mesh
     * 
     * @return true if mesh is valid
     * @return false if mesh is not valid
     */
    bool hasMesh() const;

    /**
     * @brief Setzt den Status des Raumschiffes auf zerstört.
     * 
     * Das Raumschiff wird nichtmehr gereendert.
     * Setzt m_status = 1
     */ 
    void destroySpaceCraft();

    /**
     * @brief Gibt den Status des Raumschiffes wieder
     * 
     * @return 0 wenn das Raumschiff nicht zerstört wurde
     * @return 1 wenn das Raumschiff gerade zerstört wurde
     * @return 2 wenn das Raumschiff zerstört ist 
     */ 
    int spaceCraftStatus();

    /**
     * @brief Beendet die Zerstörtung des Raumschiffes.
     * 
     * Setzt m_status = 2
     */ 
    void endDestruction();
    
    float getMaxSpeed();
    
    float getCurrentSpeed();

    void accelerate();

    void deccelerate();

private:
    TriangleMesh::Ptr m_mesh;
    
    //Gibt an ob das Raumschiff gerendert werden soll
    int m_status;

    ParticleEngine m_explosion;

    float m_maxSpeed;

    float m_currentSpeed;

};

} // namespace asteroids

#endif