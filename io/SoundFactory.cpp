#include "SoundFactory.hpp"

#include <iostream>
#include <memory>

namespace asteroids
{

    //std::map<string, std::shared_ptr<QSoundEffect> > SoundFactory::m_loadedSounds;


SoundFactory::SoundFactory():muteToggle(false)
{
    // TODO Auto-generated constructor stub

      std::shared_ptr<QSoundEffect> soundtrack = std::make_shared<QSoundEffect>();
      soundtrack->setSource(QUrl::fromLocalFile(QString::fromStdString("sounds/soundtrack.wav")));
      m_loadedSounds["soundtrack"] = soundtrack;

      std::shared_ptr<QSoundEffect> pew = std::make_shared<QSoundEffect>();
      pew->setSource(QUrl::fromLocalFile(QString::fromStdString("sounds/pewNew.wav")));
      m_loadedSounds["pew"] = pew;

      std::shared_ptr<QSoundEffect> schneller = std::make_shared<QSoundEffect>();
      schneller->setSource(QUrl::fromLocalFile(QString::fromStdString("sounds/schneller.wav")));
      m_loadedSounds["schneller"] = schneller;

      std::shared_ptr<QSoundEffect> langsamer = std::make_shared<QSoundEffect>();
      langsamer->setSource(QUrl::fromLocalFile(QString::fromStdString("sounds/langsamer.wav")));
      m_loadedSounds["langsamer"] = langsamer;

      std::shared_ptr<QSoundEffect> boom = std::make_shared<QSoundEffect>();
      boom->setSource(QUrl::fromLocalFile(QString::fromStdString("sounds/boom.wav")));
      m_loadedSounds["boom"] = boom;

}

SoundFactory::~SoundFactory()
{
    // TODO Auto-generated destructor stub
}

SoundFactory& SoundFactory::instance()
{
    static SoundFactory instance;
    return instance;
}


std::shared_ptr<QSoundEffect> SoundFactory::getSound(const string& filename)
{   
    

    auto it = m_loadedSounds.find(filename);
    
    if(it == m_loadedSounds.end())
    {
        std::cerr << "FILE NICHT IN MAP" << std::endl;
        return 0;
        
    }
    else
    {
        return it->second;        
    }
}

void SoundFactory::muteAll()
{
    std::map<string, std::shared_ptr<QSoundEffect>>::iterator it;
    if(!muteToggle){
        for( it = m_loadedSounds.begin(); it != m_loadedSounds.end(); it++ )
        {
            it->second->setMuted(true);
        }
        muteToggle = true;
    }
    else
    {
        for( it = m_loadedSounds.begin(); it != m_loadedSounds.end(); it++ )
        {
            it->second->setMuted(false);
        }
        muteToggle = false;
    }
    
}

}
