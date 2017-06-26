#include "Spell.h"
#include "Unit.h"
#include "SpellHistory.h"

class ComboPointManager
{
public:

    enum rogue_spells
    {
        ADRENALINE_RUSH = 13750,
        ALACRITY = 193539,
        ALACRITY_AURA = 193538,
        BETWEEN_EYES = 199804,
        CANNONBALL_BARRAGE = 185767,
        DEATH_FROM_ABOVE = 152150,
        GRAPPLING_HOOK = 195457,
        KILLING_SPREE = 51690,
        MARKED_DEATH = 137619,
        RESTLESS_BLADE = 79096,
        SPRINT = 2983,
        VANISH = 1856
    };

    bool HandleRuhtlessness(Unit* m_caster)
    {
        bool yes = false;
        if (m_caster->HasAura(14161)) // Ruthlessness
            yes = roll_chance_i((m_caster->GetPower(POWER_COMBO_POINTS) + 1) * 20);

        return yes;
    }

    void HandleRestlessBlades(Unit* m_caster)
    {
        if (m_caster->HasAura(RESTLESS_BLADE))
        {
            int32 spent = m_caster->GetPower(POWER_COMBO_POINTS) + 1;
            float seconds = spent * 0.5 * 1000;
            std::vector<int32> vector;

            vector.push_back(ADRENALINE_RUSH);
            vector.push_back(BETWEEN_EYES);
            vector.push_back(CANNONBALL_BARRAGE);
            vector.push_back(DEATH_FROM_ABOVE);
            vector.push_back(GRAPPLING_HOOK);
            vector.push_back(KILLING_SPREE);
            vector.push_back(MARKED_DEATH);
            vector.push_back(SPRINT);
            vector.push_back(VANISH);

            for (int i = 0; i < vector.size(); i++)
            {
                if(m_caster->HasSpell(vector.at(i)) && m_caster->GetSpellHistory()->HasCooldown(vector.at(i)))
                    m_caster->GetSpellHistory()->ModifyCooldown(vector.at(i), seconds);
            }            
        }
    }

    void HandleAlacrity(Unit* m_caster)
    {
        if (m_caster->HasAura(ALACRITY))
            if (roll_chance_i((m_caster->GetPower(POWER_COMBO_POINTS) + 1) * 20))
                m_caster->CastSpell(m_caster, 193538, true);
    }



};