#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>

#include "kibir.h"

class HealthComponent : public KibirECS::Component<HealthComponent> {
public:
    float percentage = .2;
    float healthCap = 1;
};

class RegenComponent : public KibirECS::Component<RegenComponent> {
public:
    float percentageRegenPerFrame = 0.001;
};

class RandomDamageComponent : public KibirECS::Component<RandomDamageComponent> {
public:
    float chanceToSufferDamage = 1./10.;
    float damagePercentage = 0.05;
};

class HealingSystem : public KibirECS::System<HealingSystem> {
public: 
    HealingSystem() : KibirECS::System<HealingSystem>() {
        RequireComponents<HealthComponent, RegenComponent>();
    }

    virtual void Update(float dt) override {
        auto healthComponents = m_components->Get<HealthComponent>();
        auto regenComponents = m_components->Get<RegenComponent>();

        for(auto it = healthComponents->begin(); it != healthComponents->end(); it++) {
            HealthComponent* healthComponent = static_cast<HealthComponent*>(it->second);
            RegenComponent* regenComponent = m_components->Get<RegenComponent>(it->first);

            healthComponent->percentage = std::min((float)healthComponent->healthCap, healthComponent->percentage + regenComponent->percentageRegenPerFrame);
        }
    }
};

class RandomDamageSystem : public KibirECS::System<RandomDamageSystem> {
public: 
    RandomDamageSystem() : KibirECS::System<RandomDamageSystem>() {
        RequireComponents<HealthComponent, RandomDamageComponent>();

        srand(time(NULL));
    }

    virtual void Update(float dt) override {
        auto healthComponents = m_components->Get<HealthComponent>();
        auto damageComponents = m_components->Get<RandomDamageComponent>();

        for(auto it = healthComponents->begin(); it != healthComponents->end(); it++) {
            HealthComponent* healthComponent = static_cast<HealthComponent*>(it->second);
            RandomDamageComponent* damageComponent = m_components->Get<RandomDamageComponent>(it->first);

            bool doDamage = rand() % (int)(1. / damageComponent->chanceToSufferDamage) == 0;

            if(doDamage) {
                healthComponent->percentage = std::max((float)0, healthComponent->percentage - damageComponent->damagePercentage);
            }
        }
    }
};

class DeathSystem : public KibirECS::System<DeathSystem> {
public: 
    DeathSystem() : KibirECS::System<DeathSystem>() {
        RequireComponents<HealthComponent>();
    }

    virtual void Update(float dt) override {
        auto healthComponents = m_components->Get<HealthComponent>();

        for(auto it = healthComponents->begin(); it != healthComponents->end(); it++) {
            if(static_cast<HealthComponent*>(it->second)->percentage <= 0) {
                // This currently causes a seg fault. Entity/component creation/deletionn 
                // should probably be deferred until updates finish
                //EntityId entityId = m_world->CreateEntity();
                //m_world->AddComponent<HealthComponent>(entityId);
                //m_world->AddComponent<RegenComponent>(entityId);
                //m_world->AddComponent<RandomDamageComponent>(entityId);

                m_world->RemoveEntity(it->first);
            }
        }
    }
};

class HealthReportSystem : public KibirECS::System<HealthReportSystem> {
public: 
    HealthReportSystem() : KibirECS::System<HealthReportSystem>() {
        RequireComponents<HealthComponent>();
    }

    virtual void Update(float dt) override {
        auto healthComponents = m_components->Get<HealthComponent>();

        printf("\033[2J\033[1;1H");
        for(auto it = healthComponents->begin(); it != healthComponents->end(); it++) {
            printf("Health of %d: %f\n", it->first, static_cast<HealthComponent*>(it->second)->percentage);
        }
    }
};

int main() {
    KibirECS::World world;
    
    world.AddSystem<HealingSystem>();
    world.AddSystem<RandomDamageSystem>();
    world.AddSystem<HealthReportSystem>();
    world.AddSystem<DeathSystem>();

    printf("registered component count: %d\n", KibirECS::Register<KibirECS::InternalComponent>::value.size());

    KibirECS::EntityId playerId = world.CreateEntity();
    world.AddComponent<HealthComponent>(playerId);
    world.AddComponent<RegenComponent>(playerId);
    world.AddComponent<RandomDamageComponent>(playerId);

    KibirECS::EntityId anotherPlayerId = world.CreateEntity();
    world.AddComponent<HealthComponent>(anotherPlayerId);
    world.AddComponent<RegenComponent>(anotherPlayerId);
    world.AddComponent<RandomDamageComponent>(anotherPlayerId);

    KibirECS::EntityId yetAnotherPlayerId = world.CreateEntity();
    world.AddComponent<HealthComponent>(yetAnotherPlayerId);
    world.AddComponent<RegenComponent>(yetAnotherPlayerId);
    world.AddComponent<RandomDamageComponent>(yetAnotherPlayerId);

    KibirECS::EntityId randomRegeningEntityId = world.CreateEntity();
    auto healthComp = world.AddComponent<HealthComponent>(randomRegeningEntityId);
    healthComp->healthCap = 100;
    auto regenComp = world.AddComponent<RegenComponent>(randomRegeningEntityId);
    regenComp->percentageRegenPerFrame = 1;
    world.AddComponent<RandomDamageComponent>(randomRegeningEntityId);

    int stepsTaken = 0;
    while(stepsTaken++ < 100) {
        world.Update(stepsTaken);

        usleep(100000);
    }

    return 0;
}
