#include "ScriptMgr.h"
#include "Group.h"
#include "Player.h"
#include "Random.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "SpellScript.h"

enum PalaSpells
{
    SPELL_PALA_RETRIBUTION_MASTERY = 76672

};

// 205273 Wake of Ashes

class spell_pal_wake_ashes : public SpellScriptLoader
{
public:
    spell_pal_wake_ashes() : SpellScriptLoader("spell_pal_wake_ashes") { }

    class spell_pal_wake_ashes_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_wake_ashes_SpellScript);

        bool Validate(SpellInfo const* /*spellEntry*/) override
        {
            return true;
        }

        bool Load() override
        {
            if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                return false;

            if (GetCaster()->ToPlayer()->getClass() != CLASS_PALADIN)
                return false;

            return true;
        }

        void ChangeDamage(SpellEffIndex /*effIndex*/)
        {
            int32 damage;

            damage = GetCaster()->ToPlayer()->GetInt32Value(UNIT_FIELD_ATTACK_POWER) * 6.5;

            printf("Holy power %i", GetCaster()->GetPower(POWER_HOLY_POWER));

            SetHitDamage(damage);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_pal_wake_ashes_SpellScript::ChangeDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_pal_wake_ashes_SpellScript();
    }
};

// 184662 Shield of vengeance
class spell_pal_shield_vengeance : public SpellScriptLoader
{
public:
    spell_pal_shield_vengeance() : SpellScriptLoader("spell_pal_shield_vengeance") { }

    class spell_pal_shield_vengeance_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_shield_vengeance_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return true;
        }

        void CalculateAmount(AuraEffect const* aurEff, int32 & amount, bool & canBeRecalculated)
        {
            amount = GetCaster()->ToPlayer()->GetInt32Value(UNIT_FIELD_ATTACK_POWER) * 20;
        }

        void HandleAfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_ENEMY_SPELL)
            {
                int32 damage = GetCaster()->ToPlayer()->GetInt32Value(UNIT_FIELD_ATTACK_POWER) * 20;
                GetTarget()->CastCustomSpell(184689, SPELLVALUE_BASE_POINT0, damage, (Unit*)nullptr, true, NULL);
            }
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_shield_vengeance_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            AfterEffectRemove += AuraEffectRemoveFn(spell_pal_shield_vengeance_AuraScript::HandleAfterRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_pal_shield_vengeance_AuraScript();
    }
};


// 213757 Execution Sentence
class spell_pal_execution_sentence : public SpellScriptLoader
{
public:
    spell_pal_execution_sentence() : SpellScriptLoader("spell_pal_execution_sentence") { }

    class spell_pal_execution_sentence_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_pal_execution_sentence_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return true;
        }

        void CalculateAmount(AuraEffect const* aurEff, int32 & amount, bool & canBeRecalculated)
        {
            amount = GetCaster()->ToPlayer()->GetInt32Value(UNIT_FIELD_ATTACK_POWER) * 14.5f;
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_execution_sentence_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_pal_execution_sentence_AuraScript();
    }
};


void AddSC_pala_spell_scripts()
{
    new spell_pal_wake_ashes();
    new spell_pal_shield_vengeance();
    new spell_pal_execution_sentence();
}