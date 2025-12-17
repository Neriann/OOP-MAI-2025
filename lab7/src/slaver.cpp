#include "druid.h"
#include "orc.h"
#include "slaver.h"

Slaver::Slaver(int x, int y) :
    NPC(SlaverType, x, y) {
}

Slaver::Slaver(std::istream& is) :
    NPC(SlaverType, is) {
}

void Slaver::print(std::ostream& os) const {
    os << *this;
}

void Slaver::save(std::ostream& os) {
    os << SlaverType << std::endl;
    NPC::save(os);
}

bool Slaver::fight(const NPC& other) const {
    return other.fight_with(*this);
}

bool Slaver::fight_with(const Druid& other) const {
    fight_notify(other, true);
    return true;
}

bool Slaver::fight_with(const Orc& other) const {
    fight_notify(other, false);
    return false;
}

bool Slaver::fight_with(const Slaver& other) const {
    fight_notify(other, false);
    return false;
}

std::ostream& operator<<(std::ostream& os, const Slaver& slaver) {
    os << "Slaver[" << slaver.get_id() << "] 🤡: " << static_cast<const NPC&>(slaver) << std::endl;
    return os;
}
