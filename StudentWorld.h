#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include <string>
#include <algorithm>
#include <vector>

class StudentWorld
{
public:

    ~StudentWorld()
    {
        cleanUp();
    }

    virtual int init();

    bool removeIce(int x, int y);

    virtual int move();

    virtual void cleanUp();

    void setPositions(int x, int y, char actortype);

    char getPositions(int x, int y)
    {
        return ActorPositions[x][y];
    }


    bool canFall(int x, int y);
    bool NearIceman(int x, int y, int amount);

    Actor* findProtester(int x, int y);
    void boulderAnnoyActors(int x, int y);
    bool annoyProtester(int x, int y, int amt);
    int generateNewDir(int x, int y);

    void pickupBarrel(int x, int y);
    bool playerDied();

    bool finishedLevel();
    bool noOverlap(int x, int y);
    void updateDisplayText();
    void removeDeadGameObjects();

    bool canAddProtester();
    bool canAddWater(int x, int y);


private:
    int nIce;
    Ice* IcePointers[3616];
    std::vector<Actor*> Actors;
    int ticksSinceLastProtester;
    int ticksToWaitToAddProtester;
    int nBoulders;
    int nGold;
    int pickedBarrels;
    int nProtesters;
    int nBarrels;
    char ActorPositions[64][64];
};

#endif // STUDENTWORLD_H_