#include "npc.h"

#include <cmath>

int NPC::next_save_id{1};
int NPC::next_load_id{1};

NPC::NPC(NpcType t, int _x, int _y) :
    type(t), id(next_save_id++), x(_x), y(_y) {
}

NPC::NPC(NpcType t, std::istream& is) :
    type(t), id(next_load_id++) {
    is >> x >> y;
}

void NPC::subscribe(const std::shared_ptr<IFightObserver>& observer) {
    observers.push_back(observer);
}

void NPC::fight_notify(const NPC& defender, bool win) const {
    for (auto& o : observers)
        o->on_fight(*this, defender, win);
}

bool NPC::is_close(const NPC& other, size_t distance) const {
    if (std::pow(x - other.x, 2) + std::pow(y - other.y, 2) <= std::pow(distance, 2))
        return true;
    return false;
}

int NPC::get_id() const {
    return id;
}

void NPC::save(std::ostream& os) {
    os << x << ' ' << y << std::endl;
}

std::ostream& operator<<(std::ostream& os, const NPC& npc) {
    os << "{ x:" << npc.x << ", y:" << npc.y << "} ";
    return os;
}
