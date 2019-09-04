#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>

#include "kibir.h"

class HealthComponent {
public:
    float percentage = 1;
    float healthCap = 1;

    GEN_COMPONENT_ID
};

class RegenComponent {
public:
    float percentageRegenPerFrame = 0.001;

    GEN_COMPONENT_ID
};

class RandomDamageComponent {
public:
    float chanceToSufferDamage = 1./10.;
    float damagePercentage = 0.05;

    GEN_COMPONENT_ID
};

class HealingSystem : public KibirECS::System {
public: 
    HealingSystem() {
        RequireComponents<HealthComponent, RegenComponent>();
    }

    virtual void Update(float dt) override {
        auto healthComponents = m_components.Get<HealthComponent>();
        auto regenComponents = m_components.Get<RegenComponent>();

        for(auto it = healthComponents.begin(); it != healthComponents.end(); it++) {
            HealthComponent* healthComponent = static_cast<HealthComponent*>(it->second);
            RegenComponent* regenComponent = static_cast<RegenComponent*>(regenComponents[it->first]);

            healthComponent->percentage = std::min((float)healthComponent->healthCap, healthComponent->percentage + regenComponent->percentageRegenPerFrame);
        }
    }

    GEN_SYSTEM_ID
};

class RandomDamageSystem : public KibirECS::System {
public: 
    RandomDamageSystem() {
        RequireComponents<HealthComponent, RandomDamageComponent>();

        srand(time(NULL));
    }

    virtual void Update(float dt) override {
        auto healthComponents = m_components.Get<HealthComponent>();
        auto damageComponents = m_components.Get<RandomDamageComponent>();

        for(auto it = healthComponents.begin(); it != healthComponents.end(); it++) {
            HealthComponent* healthComponent = static_cast<HealthComponent*>(it->second);
            RandomDamageComponent* damageComponent = static_cast<RandomDamageComponent*>(damageComponents[it->first]);

            bool doDamage = rand() % (int)(1. / damageComponent->chanceToSufferDamage) == 0;

            if(doDamage) {
                healthComponent->percentage = std::max((float)0, healthComponent->percentage - damageComponent->damagePercentage);
            }
        }
    }

    GEN_SYSTEM_ID
};

class HealthReportSystem : public KibirECS::System {
public: 
    HealthReportSystem() {
        RequireComponents<HealthComponent>();
    }

    virtual void Update(float dt) override {
        auto healthComponents = m_components.Get<HealthComponent>();

        for(auto it = healthComponents.begin(); it != healthComponents.end(); it++) {
            printf("Health of %d: %f\n", it->first, static_cast<HealthComponent*>(it->second)->percentage);
        }

        printf("\033[2J\033[1;1H");
    }

    GEN_SYSTEM_ID
};

// 
int main() {
    KibirECS::World world;

    KibirECS::EntityId playerId = world.CreateEntity();
    world.AddComponent<HealthComponent>(playerId);
    world.AddComponent<RegenComponent>(playerId);
    world.AddComponent<RandomDamageComponent>(playerId);

    KibirECS::EntityId randomRegeningEntityId = world.CreateEntity();
    auto healthComp = world.AddComponent<HealthComponent>(randomRegeningEntityId);
    healthComp->healthCap = 100;
    world.AddComponent<RegenComponent>(randomRegeningEntityId);

    world.AddSystem<HealingSystem>();
    world.AddSystem<RandomDamageSystem>();
    world.AddSystem<HealthReportSystem>();

    int stepsTaken = 0;
    while(stepsTaken++ < 100) {
        world.Update(stepsTaken);

        usleep(100000);
    }

    return 0;
}
