#pragma once

#include <memory>

#include "npc.h"

std::shared_ptr<NPC> factory(std::istream& is);

std::shared_ptr<NPC> factory(NpcType type, int x, int y);
