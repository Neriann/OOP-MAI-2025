#pragma once

#include <iostream>
#include <fstream>

#include <memory>
#include <vector>
#include <set>

#include "observer.h"

// type for npcs
class NPC;
class Druid;
class Orc;
class Slaver;
using set_t = std::set<std::shared_ptr<NPC>>;

enum NpcType
{
    Unknown = 0,
    SlaverType = 1,
    OrcType = 2,
    DruidType = 3
};


class NPC
{
public:
    virtual ~NPC() = default;

    NPC(NpcType t, int _x, int _y);
    NPC(NpcType t, std::istream& is);

    void subscribe(const std::shared_ptr<IFightObserver>& observer);
    void fight_notify(const NPC& defender, bool win) const;
    virtual bool is_close(const NPC& other, size_t distance) const;

    virtual bool fight(const NPC& other) const = 0;

    virtual bool fight_with(const Druid& other) const = 0;
    virtual bool fight_with(const Orc& other) const = 0;
    virtual bool fight_with(const Slaver& other) const = 0;
    virtual void print(std::ostream& os) const = 0;

    int get_id() const;

    int get_x() const;
    int get_y() const;

    NpcType get_type() const;

    virtual void save(std::ostream& os);

    friend std::ostream& operator<<(std::ostream& os, const NPC& npc);

private:
    static int next_save_id;
    static int next_load_id;

    NpcType type;
    const int id;
    int x{0};
    int y{0};
    std::vector<std::shared_ptr<IFightObserver>> observers;
};
