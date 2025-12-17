#include <chrono>
#include <cstring>
#include <random>

#include "npc.h"

#include "druid.h"
#include "orc.h"
#include "slaver.h"

#include "factory.h"
#include "observer.h"


constexpr int FIELD_SIZE = 500;
constexpr int MAX_DISTANCE = 500;
constexpr int QUANTITY_OF_ENTITIES = 100;


// save array to file
void save(const set_t& array, const std::string& filename) {
    std::ofstream fs(filename);
    fs << array.size() << std::endl;
    for (auto& n : array)
        n->save(fs);
    fs.flush();
    fs.close();
}

set_t load(const std::string& filename) {
    set_t result;
    std::ifstream is(filename);
    if (is.good() && is.is_open()) {
        int count;
        is >> count;
        for (int i = 0; i < count; ++i)
            result.insert(factory(is));
        is.close();
    }
    else
        std::cerr << "Error: " << std::strerror(errno) << std::endl;
    return result;
}

// print to screen
std::ostream& operator<<(std::ostream& os, const set_t& array) {
    for (auto& n : array)
        n->print(os);
    return os;
}


set_t fight(const set_t& array, size_t distance) {
    set_t dead_list;

    for (const auto& attacker : array)
        for (const auto& defender : array)
            if ((attacker != defender) && !dead_list.count(defender) && !dead_list.count(attacker) && (attacker->is_close(*defender, distance)) && (defender->fight(*attacker))) {
                dead_list.insert(defender);
            }

    return dead_list;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    const char* filename = argv[1];

    set_t array; // монстры

    std::mt19937 gen(std::random_device{}());

    // Генерируем начальное распределение монстров
    std::cout << "Generating ..." << std::endl;
    for (size_t i = 0; i < QUANTITY_OF_ENTITIES; ++i)
        array.insert(factory(static_cast<NpcType>(gen() % 3 + 1),
                             gen() % FIELD_SIZE,
                             gen() % FIELD_SIZE));
    std::cout << "Saving ..." << std::endl;

    save(array, filename);

    std::cout << "Loading ..." << std::endl;
    array = load(filename);

    std::cout << "Fighting ..." << std::endl << array;

    for (size_t distance = 20; (distance <= MAX_DISTANCE) && !array.empty(); distance += 10) {
        auto dead_list = fight(array, distance);
        for (auto& d : dead_list)
            array.erase(d);

        std::cout << "Fight stats ----------" << std::endl
            << "distance: " << distance << std::endl
            << "killed: " << dead_list.size() << std::endl
            << std::endl << std::endl;
    }
    if (array.size() == 1) {
        std::cout << "The winner of the fight 👑:\n" << array;
    } else if (!array.empty()) {
        std::cout << "Survivors 🔥:\n" << array;
    } else {
        std::cout << "No survivors ¯\\_(ツ)_/¯" << std::endl;
    }

    return 0;
}
