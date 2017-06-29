#include "Spell.h"
#include "Unit.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "SpellAuraEffects.h"
#include "Player.h"
#include "ComboPointManager.h"


    bool ComboPointManager::HandleRuhtlessness(Unit* m_caster)
    {
        bool yes = false;
        if (m_caster->HasAura(14161)) // Ruthlessness
            yes = roll_chance_i((m_caster->GetPower(POWER_COMBO_POINTS) + 1) * 20);

        return yes;
    }

    void ComboPointManager::HandleRestlessBlades(Unit* m_caster)
    {
        if (m_caster->HasAura(RESTLESS_BLADE))
        {
            int32 spent = m_caster->GetPower(POWER_COMBO_POINTS) + 1;
            float seconds = -1 * spent * 0.5 * 1000;
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

    void ComboPointManager::HandleAlacrity(Unit* m_caster)
    {
        if (m_caster->HasAura(ALACRITY))
            if (roll_chance_i((m_caster->GetPower(POWER_COMBO_POINTS) + 1) * 20))
                m_caster->CastSpell(m_caster, ALACRITY_AURA, true);
    }

    void ComboPointManager::HandleDeepingShadows(Unit* m_caster)
    {
        if (m_caster->HasAura(DEPEENING_SHADOWS))
        {
            int32 spent = m_caster->GetPower(POWER_COMBO_POINTS);
            float ms = -1 * spent * m_caster->GetAuraEffect(DEPEENING_SHADOWS, EFFECT_0)->CalculateAmount(m_caster) * 100;

            m_caster->GetSpellHistory()->ModifyCharge(sSpellMgr->GetSpellInfo(SHADOW_DANCE)->ChargeCategoryId, ms);
        }
    }

    void ComboPointManager::HandleRelentlessStrikes(Unit* m_caster)
    {
        if (m_caster->HasAura(RELENTLESS_STRIKES))
        {
            int32 combo = m_caster->GetPower(POWER_COMBO_POINTS);
            int32 energy = combo * 6;

            m_caster->ModifyPower(POWER_ENERGY, energy);
        }
    }

    bool ComboPointManager::HandleOutLaw(Unit* m_caster)
    {
        if(Player *player = m_caster->ToPlayer())
            if (player->GetUInt32Value(PLAYER_FIELD_CURRENT_SPEC_ID) == TALENT_SPEC_ROGUE_COMBAT)
            {
                HandleRestlessBlades(m_caster);
                HandleAlacrity(m_caster);
                return HandleRuhtlessness(m_caster);
            }
        return false;
    }

    void ComboPointManager::HandleSubtlety(Unit* m_caster)
    {
        if (Player *player = m_caster->ToPlayer())
            if (player->GetUInt32Value(PLAYER_FIELD_CURRENT_SPEC_ID) == TALENT_SPEC_ROGUE_SUBTLETY)
            {
                HandleAlacrity(m_caster);
                HandleDeepingShadows(m_caster);
                HandleRelentlessStrikes(m_caster);
            }
    }

    bool ComboPointManager::HandleComboPoints(Unit *m_caster)
    {
        HandleSubtlety(m_caster);
        return HandleOutLaw(m_caster);
    }

    //TODO cambiar cooldowns por valores negativos
