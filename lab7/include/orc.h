#pragma once
#include "npc.h"


class Orc : public NPC
{
public:
    Orc(int x, int y);
    explicit Orc(std::istream& is);

    void print(std::ostream& os) const override;
    void save(std::ostream& os) override;
    bool fight(const NPC& other) const override;

    bool fight_with(const Druid& other) const override;
    bool fight_with(const Orc& other) const override;
    bool fight_with(const Slaver& other) const override;
    friend std::ostream& operator<<(std::ostream& os, const Orc& orc);
};
