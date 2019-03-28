#ifndef SOUNDFACTORY_H_
#define SOUNDFACTORY_H_

#include <string>
using std::string;

#include <map>
#include <memory>

#include <QSoundEffect>
#include <QUrl>


namespace asteroids
{

/**
 * @brief Contains all sounds used in the game and returns a smart-pointer to them when asked.
 */ 
class SoundFactory
{
protected:

    /**
     * @brief Constructor, writes all used sounds into the map
     */
    SoundFactory();

    /**
     * @brief Destructor
     */
    virtual ~SoundFactory();

    std::map<string, std::shared_ptr<QSoundEffect>> m_loadedSounds;

public:

    /**
     * @brief   Returns a smart-pointer to a QSoundEffect called filename
     */
    std::shared_ptr<QSoundEffect> getSound(const string& filename);

    /**
     * @brief   Returns the singleton instance
     */
    static SoundFactory& instance();

    /**
     * @brief mutes/unmutes all sounds
     */ 
    void muteAll();

private:

    ///Toggle bool for the mute/unmute function
    bool    muteToggle;
};

}
#endif
