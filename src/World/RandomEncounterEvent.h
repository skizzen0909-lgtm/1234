// World/RandomEncounterEvent.h
#ifndef RANDOMENCOUNTEREVENT_H
#define RANDOMENCOUNTEREVENT_H

#include "WorldEvent.h"
#include <memory>
#include <string>

class RandomEncounterEvent : public WorldEvent {
private:
    int enemyLevel;
    std::string enemyType;
    bool encounterResolved;
    
public:
    RandomEncounterEvent(const std::string& id,
                        const std::string& enemyType,
                        int enemyLevel,
                        float duration = 120.0f);
    
    ~RandomEncounterEvent() override = default;
    
    void handleEvent(Entity* player) override;
    
    void resolveEncounter(bool won);
    bool isResolved() const;
    int getEnemyLevel() const;
    const std::string& getEnemyType() const;
};

#endif // RANDOMENCOUNTEREVENT_H
