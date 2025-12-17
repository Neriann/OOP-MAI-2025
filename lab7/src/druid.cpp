#include "druid.h"
#include "orc.h"
#include "slaver.h"


Druid::Druid(int x, int y) :
    NPC(DruidType, x, y) {
}

Druid::Druid(std::istream& is) :
    NPC(DruidType, is) {
}

void Druid::print(std::ostream& os) const {
    os << *this;
}

bool Druid::fight(const NPC& other) const {
    return other.fight_with(*this);
}

bool Druid::fight_with(const Druid& other) const {
    fight_notify(other, false);
    return false;
}

bool Druid::fight_with(const Orc& other) const {
    fight_notify(other, false);
    return false;
}

bool Druid::fight_with(const Slaver& other) const {
    fight_notify(other, false);
    return false;
}

void Druid::save(std::ostream& os) {
    os << DruidType << std::endl;
    NPC::save(os);
}

std::ostream& operator<<(std::ostream& os, const Druid& druid) {
    os << "Druid[" << druid.get_id() << "] 🤖: " << static_cast<const NPC&>(druid) << std::endl;
    return os;
}
