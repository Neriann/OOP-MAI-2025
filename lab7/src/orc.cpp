#include "druid.h"
#include "orc.h"
#include "slaver.h"

Orc::Orc(int x, int y) :
    NPC(OrcType, x, y) {
}

Orc::Orc(std::istream& is) :
    NPC(OrcType, is) {
}

void Orc::print() const {
    std::cout << *this;
}

void Orc::save(std::ostream& os) {
    os << OrcType << std::endl;
    NPC::save(os);
}

bool Orc::fight(const NPC& other) const {
    return other.fight_with(*this);
}

bool Orc::fight_with(const Druid& other) const {
    fight_notify(other, true);
    return true;
}

bool Orc::fight_with(const Orc& other) const {
    fight_notify(other, true);
    return true;
}

bool Orc::fight_with(const Slaver& other) const {
    fight_notify(other, true);
    return true;
}

std::ostream& operator<<(std::ostream& os, const Orc& orc) {
    os << "Orc[" << orc.get_id() << "] 👹: " << static_cast<const NPC&>(orc) << std::endl;
    return os;
}
