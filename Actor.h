#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>
/*
 *
 *
 * GraphObject
    Actor
        Agent
            IceMan
            Protester
                RegularProtester
                HardCoreProtester
        Ice
        Boulder
        Squirt
        ActivatingObject
            OilBarrel
            GoldNugget
            SonarKit
            WaterPool
 *
 *
 *
*/

const double SIZE_NORMAL = 1.0;
const double SIZE_SMALL = 0.25;
const double FALLING = 10;
const double WAITING = 9;
const double DEAD = 0;

class StudentWorld;
class Point
{
public:

    Point(int x = -1, int y = -1);

    Point(const Point& p);
    int getX() const;
    int getY() const;

    Point getAdjLeft() const;
    Point getAdjUp() const;
    Point getAdjRight() const;
    Point getAdjDown() const;
    Point& operator=(Point& p);
    bool operator==(Point& p);
    bool operator!=(Point& p);
    bool isValid() const;
    bool isInBounds() const;

    int m_x;
    int m_y;
};

class BoundingBox
{
public:
    BoundingBox(int x = 0, int y = 0);
    BoundingBox(int x, int y, int size);
    void updateBB(int x, int y);

    Point* getBB();
    Point getXY();

    bool intersects(BoundingBox BB);
    bool operator==(BoundingBox& BB);
    bool operator!=(BoundingBox& BB);

    ~BoundingBox();
private:

    int m_size;
    Point* m_box;
};

class Actor : public GraphObject
{
public:

    enum DamageSource { waterSpray, rockFall, protest };

    Actor(int imageID,
        int startX,
        int startY,
        double size,
        unsigned int depth,
        int health,
        bool isDamageable,
        bool isPassable);
    bool isFacing(Actor* other) const;
    void faceTowards(Actor* other);
    virtual void move();
    virtual void takeDamage(DamageSource src) = 0;
    StudentWorld* getWorld();
    BoundingBox getBB();
    bool isDamageable() const;
    bool isPassable() const;
    bool isAlive() const;
    void setDead();
    int getHealth() const;
    void incHealth();
    void decHealth();
    void decHealth(int i);
    int getTicksAlive() const;
    virtual ~Actor();

private:
    virtual void doSomething() = 0;
    BoundingBox m_BB;
    int m_ticksAlive;
    int m_health;
    int m_iFrames;
    bool m_isAlive;
    bool m_isDamageable;
    bool m_isPassable;

};

class HardcoreProtester : public Protester
{
public:

    HardcoreProtester(int x = 60, int y = 60);
    virtual ~HardcoreProtester();

private:

    virtual int getGiveUpPoints();
    virtual void pathTowardsPlayer();
    virtual void foundGold();
    std::string m_pathToPlayer;
    std::size_t m_maxPathSize;
};

class Iceman : public Actor
{
private:
    int m_numSonarKits;
    int m_numGoldNuggets;
    int m_numSquirts;
public:
    Iceman(int imageID = 0,
        int startX = 30,
        int startY = 60,
        double size = 1.0,
        unsigned int depth = 0,
        int health = 10,
        bool isDamageable = true,
        bool isPassable = true);

    void doSomething();
    void takeDamage(DamageSource src);
    void gotSonarKit();
    void gotGoldNugget();
    void gotWater();
    int getNumSquirts();
    int getNumSonarKits();
    int getNumGoldNuggets();
    virtual ~Iceman();

};

class Protester : public Actor
{
private:
    virtual int getGiveUpPoints() = 0;
    virtual void pathTowardsPlayer() = 0;

public:
    enum States { InOilField, LeaveOilField };

    Protester(int imageID, int health, int x, int y);
    States getState() const;
    virtual void doSomething();
    virtual void takeDamage(DamageSource src);
    virtual void foundGold() = 0;
    virtual ~Protester();

protected:
    int m_stepsInCurrDir;
    int m_ticksSinceAxisSwap;
    int m_nonShoutingActions;
    int m_restingTickCount;
    int m_stunTicksLeft;

    std::string m_pathOut;
    States m_state;

    bool m_isBribed;
};

class RegularProtester : public Protester
{
private:
    virtual int getGiveUpPoints();
    virtual void pathTowardsPlayer();
    virtual void foundGold();

public:
    RegularProtester(int x = 60, int y = 60);
    virtual ~RegularProtester();

};

class Ice : public Actor
{
public:

    Ice(int startX, int startY);
    virtual void doSomething();
    virtual void takeDamage(DamageSource src);
    virtual ~Ice();
};

class Boulder : public Actor
{
private:
    int m_ticksUnstable;
    bool m_isStable;
    bool m_isFalling;
public:
    Boulder(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool canActorsPassThroughMe() const;
    virtual void doSomething();
    virtual ~Boulder();
};

class Squirt : public Actor
{
private:
    int m_movesLeft;
    bool firstRun;
public:
    Squirt(StudentWorld* world, int startX, int startY, Direction startDir);

    virtual void move();
    virtual void takeDamage(DamageSource src);
    virtual void doSomething();
    virtual ~Squirt();
};


class Item : public Actor
{
    enum States { Permanent, Temporary };
private:
    virtual void ItemDoSomething() = 0;
    int m_tempLifetime;
    States m_state;
    bool m_hasBeenPickedUp;
public:

    Item(int imageID,
        int startX,
        int startY,
        Direction dir,
        double size,
        unsigned int depth,
        States state);
    virtual ~Item();
    virtual void takeDamage(DamageSource src);
    virtual void doSomething();
    void setPickedUp();
    bool hasBeenPickedUp() const;
    States getState() const;
    void setTempLifetime(int ticks = 100);
    int getTempTicksLeft();
};

class OilBarrel : public Item
{
private:
    virtual void ItemDoSomething();

public:
    OilBarrel(int x, int y);
    ~OilBarrel();
};

class GoldNugget : public Item
{
private:
    virtual void ItemDoSomething();
public:
    GoldNugget(int x, int y, States state);
    ~GoldNugget();
};

class WaterPool : public Item
{
private:
    virtual void ItemDoSomething();

public:
    WaterPool(int x, int y);
    WaterPool(StudentWorld* world, int startX, int startY);
    virtual void move();
    ~WaterPool();
};

#endif // ACTOR_H_