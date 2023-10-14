#include <iostream>

#include "Org.h"
#include "emp/base/vector.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"
#include "emp/config/ArgManager.hpp"

#include "World.h"

EMP_BUILD_CONFIG(MyConfigType,
    VALUE(SEED, int, 2, "What value should the random seed be?"), 
    VALUE(MUT_PROB, double, 0.005, "What should the percent probability that each bit in the binary representation of the genome is flipped be?"),
    VALUE(SR, int, 0, "Should pseudo-sexual reproduction be included? 0 for no 1 for yes."),
    VALUE(FILE_NAME, std::string, "_data.dat", "Root output file name"),
    VALUE(FILE_PATH, std::string, "Example/", "Location of the data"),
    VALUE(INITIAL_ORG_NUM, int, 8, "How many organisms should the world start with?"),
    VALUE(WORLD_WIDTH, int, 60, "What should be the width of the square-shaped, toroidal structure world?"),
    VALUE(WORLD_HEIGHT, int, 60, "What should be the height of the square-shaped, toroidal structure world?")
)
