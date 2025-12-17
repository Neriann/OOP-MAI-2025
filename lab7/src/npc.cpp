#include "npc.h"

#include <cmath>
#include <random>
#include <bits/random.h>

int NPC::next_save_id{1};
int NPC::next_load_id{1};

NPC::NPC(NpcType t, int x, int y) :
    type(t), _id(next_save_id++), _x(x), _y(y), _step(NPC_STEPS[t]), _distance(NPC_DISTANCES[t]) {
}

NPC::NPC(NpcType t, std::istream& is) :
    type(t), _id(next_load_id++), _step(NPC_STEPS[t]), _distance(NPC_DISTANCES[t]) {
    if (!is.eof()) is >> _x;
    else throw std::invalid_argument("NPC::NPC: bad input");

    if (!is.eof()) is >> _y;
    else throw std::invalid_argument("NPC::NPC: bad input");
}

void NPC::subscribe(const std::shared_ptr<IFightObserver>& observer) {
    _observers.push_back(observer);
}

void NPC::fight_notify(const NPC& defender, bool win) const {
    for (auto& o : _observers)
        o->on_fight(*this, defender, win);
}

bool NPC::is_close(const NPC& other) const {
    if (std::pow(_x - other._x, 2) + std::pow(_y - other._y, 2) <= std::pow(_distance, 2))
        return true;
    return false;
}

void NPC::save(std::ostream& os) {
    os << _x << ' ' << _y << std::endl;
}


int NPC::get_id() const {
    return _id;
}

int NPC::get_x() const {
    return _x;
}

int NPC::get_y() const {
    return _y;
}

std::pair<int, int> NPC::get_position() const {
    return {_x, _y};
}

int NPC::get_step() const {
    return _step;
}

NpcType NPC::get_type() const {
    return type;
}

bool NPC::is_alive() const {
    return _alive;
}

void NPC::must_die() {
    _alive = false;

}

void NPC::move(int shift_x, int shift_y, int max_x, int max_y) {
    if (_x + shift_x >= 0 && _x + shift_x < max_x) {
        _x += shift_x;
    }
    if (_y + shift_y >= 0 && _y + shift_y < max_y) {
        _y += shift_y;
    }
}


std::ostream& operator<<(std::ostream& os, const NPC& npc) {
    os << "{ x:" << npc._x << ", y:" << npc._y << "} ";
    return os;
}
