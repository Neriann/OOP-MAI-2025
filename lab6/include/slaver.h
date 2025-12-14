#pragma once
#include "npc.h"

class Slaver : public NPC
{
public:
    Slaver(int x, int y);
    explicit Slaver(std::istream& is);

    void print() const override;
    void save(std::ostream& os) override;

    bool fight(const NPC& other) const override;

    bool fight_with(const Druid& other) const override;
    bool fight_with(const Orc& other) const override;
    bool fight_with(const Slaver& other) const override;

    friend std::ostream& operator<<(std::ostream& os, const Slaver& slaver);
};
