# GalaxyFight

This is a round based 2D/3D strategy game. Two players fight over map domination. In the case of direct attacks, the player must win a real time 3D space shooter fight.
Window management, views, Networking and synchronization are done in QT-Framework. Boost is used for IO, graph algorithms and way finding.

### The Map:
![Alt Text](https://github.com/hegerdes/GalaxyFight/blob/master/docs/examples/galaxy.png?raw=true)


### The Fight:

![Alt Text](https://github.com/hegerdes/GalaxyFight/blob/master/docs/examples/spaceqt.gif?raw=true)


## Build

### Server:

In praktikum1/qtserver/build/:
```bash
qmake .. && make
```

### Client:
In praktikum1/build/:

```bash
cmake .. && make
```
## Run

### Server:
In praktikum1/qtserver/build/

### Client:
In praktikum1/build/asteroids


### Other Notes:

- Im Spiel erst auf den "Spielen" Knopf drücken wenn beide Spiele gestartet sind!
- In der [Main.cpp](https://gitlab.informatik.uni-osnabrueck.de/cpp2018/praktikum1/blob/master/Main.cpp) den Host auf die Adresse ändern wo der Server gehostet ist