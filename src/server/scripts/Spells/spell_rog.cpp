//Una script chupiguay
#include "ScriptMgr.h"
#include "Containers.h"
#include "DB2Stores.h"
#include "Item.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include <iostream>

enum rog_spells
{
    SPELL_ROGUE_MASTER_OF_SUBTLETY_DAMAGE_PERCENT = 31665,
    SPELL_ROGUE_SHADOW_DANCE_AURA2                = 185422,
    SPELL_ROGUE_SOOTHING_DARKNESS                 = 200759,
    SPELL_ROGUE_STEALTH                           = 1784
};

// 185313 - Shadow Dance
class spell_rog_shadow_dance_aura : public SpellScriptLoader
{
    public:
        spell_rog_shadow_dance_aura() : SpellScriptLoader("spell_rog_shadow_dance_aura") { }
    
        class spell_rog_shadow_dance_aura_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_shadow_dance_aura_SpellScript);

            void HandleLaunch(SpellEffIndex effIndex)
            { 
            }

            void Register() override
            {
                OnEffectHit += SpellEffectFn(spell_rog_shadow_dance_aura_SpellScript::HandleLaunch, EFFECT_2, SPELL_EFFECT_APPLY_AURA);
            }
        };

        class spell_rog_shadow_dance_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_shadow_dance_aura_AuraScript);

            void HandleOnApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {     
                GetTarget()->CastSpell(GetCaster(), SPELL_ROGUE_SHADOW_DANCE_AURA2, true);
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
            {
                if(AuraEffect *aurEffe = GetCaster()->GetAuraEffect(SPELL_ROGUE_SOOTHING_DARKNESS, EFFECT_0))
                    amount += aurEffe->GetAmount();
            }

            void Register() override
            {     
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_shadow_dance_aura_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_OBS_MOD_HEALTH);
                OnEffectApply += AuraEffectApplyFn(spell_rog_shadow_dance_aura_AuraScript::HandleOnApply, EFFECT_2, SPELL_AURA_OBS_MOD_HEALTH, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_rog_shadow_dance_aura_AuraScript();
        }

        SpellScript* GetSpellScript() const override
        {
            return new spell_rog_shadow_dance_aura_SpellScript();
        }

};

// 31223 - Master of Subtlety passive
class spell_rog_master_of_subtlety_passive : public SpellScriptLoader
{
public:
    spell_rog_master_of_subtlety_passive() : SpellScriptLoader("spell_rog_master_of_subtlety_passive") { }

    class spell_rog_master_of_subtlety_passive_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_master_of_subtlety_passive_AuraScript);

        void HandleOnApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            if (GetTarget()->HasAura(SPELL_ROGUE_STEALTH))
                GetTarget()->CastSpell(GetTarget(), SPELL_ROGUE_MASTER_OF_SUBTLETY_DAMAGE_PERCENT, true);
        }

        void HandleOnRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            printf("Hey");
            if (GetTarget()->HasAura(SPELL_ROGUE_MASTER_OF_SUBTLETY_DAMAGE_PERCENT))
                GetTarget()->RemoveAura(SPELL_ROGUE_MASTER_OF_SUBTLETY_DAMAGE_PERCENT);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_rog_master_of_subtlety_passive_AuraScript::HandleOnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectApplyFn(spell_rog_master_of_subtlety_passive_AuraScript::HandleOnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_rog_master_of_subtlety_passive_AuraScript();
    }


};



void AddSC_spell_rog_spell_scripts_two()
{
    new spell_rog_shadow_dance_aura();
    new spell_rog_master_of_subtlety_passive();
}