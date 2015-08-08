#define CATCH_CONFIG_RUNNER
#include "../src/length.h"
#include "../src/angle.h"
#include "../lib/catch.hpp"

using namespace dewalls;

int main(int argc, char* const argv[])
{
    Length::init();
    Angle::init();

    return Catch::Session().run(argc, argv);
}
