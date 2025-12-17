#include <sstream>

#include "npc.h"

#include "observer.h"

// Text Observer
TextObserver::TextObserver(const std::string& filename): console_stream(std::cout), file_stream(filename, std::ios::out | std::ios::app) {
    if (!file_stream.is_open()) {
        std::cerr << "Error: Could not open log file: " << filename << std::endl;
    }
}
std::shared_ptr<IFightObserver> TextObserver::get(const std::string& filename) {
    static TextObserver instance(filename);
    // Constructor with custom deleter
    return {&instance, [](IFightObserver*) {}};
}
void TextObserver::on_fight(const NPC& attacker, const NPC& defender, bool win) {
    if (win) {
        static int count = 0;
        std::stringstream oss;
        oss << std::endl
            << '[' << ++count << ']' << " Murder --------" << std::endl;
        attacker.print(oss);
        defender.print(oss);
        std::string message = oss.str();
        console_stream << message;
        console_stream.flush();
        if (file_stream.is_open()) {
            file_stream << message;
            file_stream.flush();
        }
    }
}
TextObserver::~TextObserver() {
    if (file_stream.is_open()) {
        file_stream.close();
    }
}
