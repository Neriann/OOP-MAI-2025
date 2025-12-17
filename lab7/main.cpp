#include <chrono>

#include "npc.h"
#include "druid.h"
#include "orc.h"
#include "slaver.h"
#include "factory.h"


#include <cstring>
#include <queue>
#include <random>
#include <thread>
#include <optional>

using namespace std::chrono_literals;
extern std::mutex print_mutex;

constexpr int MAX_X_FIELD_SIZE = 100;
constexpr int MAX_Y_FIELD_SIZE = 100;
constexpr int QUANTITY_OF_ENTITIES = 50;
constexpr auto PROGRAM_TIME = 30s;


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
void print_entities(const set_t& array, auto func) {
    for (auto& npc : array)
        if (npc->is_alive() && func(npc)) npc->print(std::cout);
}

// Singleton
struct RandomGenerator {
    static std::mt19937& get() {
        static std::mt19937 instance({std::random_device{}()});
        return instance;
    }
private:
    RandomGenerator() = default;
};

struct FightEvent {
    std::shared_ptr<NPC> attacker;
    std::shared_ptr<NPC> defender;
};


// Singleton
class FightManager {
public:
    static FightManager& get() {
        static FightManager instance;
        return instance;
    }

    void add_event(FightEvent&& event) {
        // Синхронизируем запись
        std::lock_guard lock(mutex);
        events.emplace(event.attacker, event.defender);
    }

    std::optional<FightEvent> get_event() {
        // Синхронизируем чтение
        std::lock_guard lock(mutex);
        if (events.empty()) {
            return std::nullopt;
        }
        auto event = events.front();
        events.pop();
        return event;
    }

private:
    FightManager() = default;
    std::queue<FightEvent> events;
    std::mutex mutex;
};


void count_entities(const set_t& array, int& count_druids, int& count_orcs, int& count_slavers, int& count_not_alive) {
    for (auto& npc : array) {
        if (npc->is_alive()) {
            switch (npc->get_type()) {
            case DruidType:
                ++count_druids;
                break;
            case OrcType:
                ++count_orcs;
                break;
            case SlaverType:
                ++count_slavers;
                break;
            default:
                std::cerr << "Error: " << std::strerror(errno) << std::endl;
            }
        } else {
            ++count_not_alive;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    const char* filename = argv[1];

    set_t array; // монстры

    // Генерируем начальное распределение монстров
    std::cout << "Generating ..." << std::endl;
    for (size_t i = 0; i < QUANTITY_OF_ENTITIES; ++i)
        array.insert(factory(static_cast<NpcType>(RandomGenerator::get()() % 3),
                             RandomGenerator::get()() % MAX_X_FIELD_SIZE,
                             RandomGenerator::get()() % MAX_X_FIELD_SIZE));
    std::cout << "Saving ..." << std::endl;

    save(array, filename);

    std::cout << "Loading ..." << std::endl;
    array = load(filename);

    int count_not_alive_before_fight = 0, count_druids_before_fight = 0, count_orcs_before_fight = 0, count_slavers_before_fight = 0;
    count_entities(array, count_druids_before_fight, count_orcs_before_fight, count_slavers_before_fight, count_not_alive_before_fight);

    std::cout << "Starting list:" << std::endl;

    print_entities(array, [](auto& npc) {return true;});

    bool program_running = true;

    std::thread fight_thread([&program_running] {
        FightManager& fm = FightManager::get();
        while (program_running) {
            if (auto event = fm.get_event()) {
                if (!event->attacker->is_alive() || !event->defender->is_alive()) continue;

                unsigned int power_attacker = RandomGenerator::get()() % 6 + 1;
                unsigned int power_defender = RandomGenerator::get()() % 6 + 1;

                if (power_attacker > power_defender && event->defender->fight(*event->attacker)) {
                    event->defender->must_die();
                }
            }
        }
        std::this_thread::sleep_for(100ms);
    });

    std::thread move_thread([&array, &program_running] {
        while (program_running) {
            for (auto& npc : array) {
                if (npc->is_alive())
                    npc->move(RandomGenerator::get()() % (2 * npc->get_step() + 1) - npc->get_step(),
                        RandomGenerator::get()() % (2 * npc->get_step() + 1) - npc->get_step(),
                        MAX_X_FIELD_SIZE, MAX_Y_FIELD_SIZE);
            }
            for (auto& attacker : array) {
                for (auto& defender : array) {
                    if (attacker != defender && attacker->is_alive() && defender->is_alive() && attacker->is_close(*defender)) {
                        FightManager::get().add_event({attacker, defender});
                    }
                }
            }
            std::this_thread::sleep_for(1s);
        }
    });
    auto start_time = std::chrono::steady_clock::now();
    std::array<std::array<char, MAX_Y_FIELD_SIZE>, MAX_X_FIELD_SIZE> field{};
    while (std::chrono::steady_clock::now() - start_time < PROGRAM_TIME) {
        for (auto& row : field) {
            row.fill(' ');
        }
        for (auto& npc: array) {
            auto [i, j] = npc->get_position();

            if (npc->is_alive()) {

                switch (npc->get_type()) {
                case DruidType:
                    field[i][j] = 'D';
                    break;
                case OrcType:
                    field[i][j] = 'O';
                    break;
                case SlaverType:
                    field[i][j] = 'S';
                    break;
                default:
                    std::cerr << "Error: " << std::strerror(errno) << std::endl;
                }
            } else {
                field[i][j] = '.';
            }
        }
        {
            std::lock_guard lock(print_mutex);
            for (auto& row : field) {
                for (char col : row) {
                    std::cout << '[' << col << ']' << ' ';
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }


        std::this_thread::sleep_for(1s);
    }
    program_running = false;

    fight_thread.join();
    move_thread.join();


    int count_not_alive_after_fight = 0, count_druids_after_fight = 0, count_orcs_after_fight = 0, count_slavers_after_fight = 0;
    count_entities(array, count_druids_after_fight, count_orcs_after_fight, count_slavers_after_fight, count_not_alive_after_fight);

    std::lock_guard lock(print_mutex);
    std::cout << "Statistics:" << std::endl;
    std::cout << "Before fight:" << std::endl;
    std::cout << "The number of NPCs: " << QUANTITY_OF_ENTITIES - count_not_alive_before_fight << std::endl;
    std::cout << "The number of druids: " << count_druids_before_fight << std::endl;
    std::cout << "The number of orcs: " << count_orcs_before_fight << std::endl;
    std::cout << "The number of slavers: " << count_slavers_before_fight << std::endl;

    std::cout << std::endl;

    std::cout << "After fight:" << std::endl;
    std::cout << "The number of NPCs: " << QUANTITY_OF_ENTITIES - count_not_alive_after_fight << std::endl;
    std::cout << "The number of druids: " << count_druids_after_fight << std::endl;
    print_entities(array, [](auto& npc) { return npc->get_type() == DruidType; });

    std::cout << std::endl;

    std::cout << "The number of orcs: " << count_orcs_after_fight << std::endl;
    print_entities(array, [](auto& npc) { return npc->get_type() == OrcType; });

    std::cout << std::endl;

    std::cout << "The number of slavers: " << count_slavers_after_fight << std::endl;
    print_entities(array, [](auto& npc) { return npc->get_type() == SlaverType; });

    std::cout << std::endl;



    return 0;
}
