#include "factory.h"

#include "druid.h"
#include "orc.h"
#include "slaver.h"

#include "observer.h"

// Фабрики -----------------------------------
std::shared_ptr<NPC> factory(std::istream& is) {
    std::shared_ptr<NPC> result = nullptr;
    int type{0};
    if (is >> type) {
        switch (type) {
        case SlaverType:
            result = std::make_shared<Slaver>(is);
            break;
        case OrcType:
            result = std::make_shared<Orc>(is);
            break;
        case DruidType:
            result = std::make_shared<Druid>(is);
            break;

        default: ;
        }
    }
    else
        std::cerr << "Unexpected NPC type:" << type << std::endl;

    if (result)
        result->subscribe(TextObserver::get());

    return result;
}

std::shared_ptr<NPC> factory(NpcType type, int x, int y) {
    std::cout << "Type:" << type << std::endl;
    std::shared_ptr<NPC> result;
    switch (type) {
    case SlaverType:
        result = std::make_shared<Slaver>(x, y);
        break;
    case OrcType:
        result = std::make_shared<Orc>(x, y);
        break;
    case DruidType:
        result = std::make_shared<Druid>(x, y);
        break;
    default: ;
    }
    if (result)
        result->subscribe(TextObserver::get());

    return result;
}
