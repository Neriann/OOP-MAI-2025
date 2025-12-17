#pragma once
#include "npc.h"


class Druid : public NPC
{
public:
    Druid(int x, int y);
    explicit Druid(std::istream& is);

    void print(std::ostream& os) const override;

    bool fight(const NPC& other) const override;

    bool fight_with(const Druid& other) const override;
    bool fight_with(const Orc& other) const override;
    bool fight_with(const Slaver& other) const override;

    void save(std::ostream& os) override;

    friend std::ostream& operator<<(std::ostream& os, const Druid& druid);
};
