#include <iostream>

#include "burst/Engine.h"

int main()
{
    auto engine = burst::Engine( 800, 600, "Burst example" );
    engine.Run();
}