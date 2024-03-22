#include "H2O.hpp"
#include <iostream>
#include <pthread.h>
#include <string>

using namespace ns_water_molecules;

int main()
{
    WaterMolecules water;

    pthread_t H;
    pthread_t O;

    pthread_create(&H, nullptr, WaterMolecules::hydrogen, &water);
    pthread_create(&O, nullptr, WaterMolecules::oxygen, &water);

    std::string molecules;
    std::cin >> molecules;

    int h = 0, o = 0;
    for (int i = 0; i < molecules.size(); ++i)
    {
        if (molecules[i] == 'H')
            ++h;
        else if (molecules[i] == 'O')
            ++o;

        water.Push(h, o);
    }
    
    return 0;
}