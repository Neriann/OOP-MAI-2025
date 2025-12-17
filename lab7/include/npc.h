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
    SlaverType = 0,
    OrcType = 1,
    DruidType = 2,
};

constexpr int NPC_STEPS[] = {
    [SlaverType] = 10,
    [OrcType] = 20,
    [DruidType] = 10
};

constexpr int NPC_DISTANCES[] = {
    [SlaverType] = 10,
    [OrcType] = 10,
    [DruidType] = 10
};

class NPC
{
public:
    virtual ~NPC() = default;

    NPC(NpcType t, int x, int y);
    NPC(NpcType t, std::istream& is);

    void subscribe(const std::shared_ptr<IFightObserver>& observer);
    void fight_notify(const NPC& defender, bool win) const;
    virtual bool is_close(const NPC& other) const;

    virtual bool fight(const NPC& other) const = 0;

    virtual bool fight_with(const Druid& other) const = 0;
    virtual bool fight_with(const Orc& other) const = 0;
    virtual bool fight_with(const Slaver& other) const = 0;
    virtual void print(std::ostream& os) const = 0;

    virtual void save(std::ostream& os);

    int get_id() const;
    int get_x() const;
    int get_y() const;

    std::pair<int, int> get_position() const;
    int get_step() const;
    NpcType get_type() const;

    bool is_alive() const;

    void must_die();

    void move(int shift_x, int shift_y, int max_x, int max_y);

    friend std::ostream& operator<<(std::ostream& os, const NPC& npc);

private:
    static int next_save_id;
    static int next_load_id;

    NpcType type;

    const int _id;
    int _x{0};
    int _y{0};
    int _step{0};
    int _distance{0};
    bool _alive{true};

    std::vector<std::shared_ptr<IFightObserver>> _observers;
};
