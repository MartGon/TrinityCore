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
            printf("\nHola que pasa man desde la script\n");
            if (AuraEffect *auraEff = GetCaster()->GetAuraEffect(200759, EFFECT_0))
                //SetEffectValue(auraEff->GetAmount());
            
            printf("El valor es %i", GetEffectValue());
            
            
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
            printf("\nHola que pasa man\n");
            printf("Hola el number es %i", aurEff->GetAmount());
            if (AuraEffect *auraEff = GetTarget()->GetAuraEffect(200759, EFFECT_0))
                const_cast<AuraEffect*>(aurEff)->SetAmount(auraEff->GetAmount());
            
        }

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            printf("\nCalculadno\n"); //Funsionó
            amount += GetCaster()->GetAuraEffect(200759, EFFECT_0)->GetAmount();
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


void AddSC_spell_rog_spell_scripts_two()
{
    new spell_rog_shadow_dance_aura();
}