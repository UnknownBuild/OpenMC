#pragma once
#include "Chunk.h"

class Region final {
private:
    Chunk chunks[32][32];
};
