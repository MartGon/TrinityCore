#include <iostream>
#include "FindSpellID.h"

        
// new stupid function

bool IsThisIdOnMap(int id)
{
    if (id == 185438 || //Shadowstrike
        id == 53 || // Backstab 
        id == 196819 || // Eviscerate
        id == 114014 || // Shuriken Tossd
        id == 197835 || // Shuriken Storm
        id == 209782 || // Goremaw's Bite (Artifact)
        id == 51723 || // Fan of Knives
        id == 1329 || // Mutilate
        id == 16511 || // Hemorrhage
        id == 245389 || // Toxic Blade
        id == 32645 || // Envenom
        id == 185565 || // Poisoned Knife
        id == 222062 || // Kingsbane (Artifact)
        id == 200758    // GloomBlade
        )
        return true;

    return false;
}
	

