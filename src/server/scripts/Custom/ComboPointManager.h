#include "Spell.h"
#include "Unit.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "SpellAuraEffects.h"
#include "Player.h"

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
        DEPEENING_SHADOWS = 185314,
        GRAPPLING_HOOK = 195457,
        KILLING_SPREE = 51690,
        MARKED_DEATH = 137619,
        RELENTLESS_STRIKES = 58423,
        RESTLESS_BLADE = 79096,
        SHADOW_DANCE = 185313,
        SPRINT = 2983,
        VANISH = 1856
    };

    static bool HandleRuhtlessness(Unit* m_caster);

    static void HandleRestlessBlades(Unit* m_caster);

    static void HandleAlacrity(Unit* m_caster);

    static void HandleDeepingShadows(Unit* m_caster);

    static void HandleRelentlessStrikes(Unit* m_caster);

    static bool HandleOutLaw(Unit* m_caster);

    static void HandleSubtlety(Unit* m_caster);

    static bool HandleComboPoints(Unit *m_caster);
    
};