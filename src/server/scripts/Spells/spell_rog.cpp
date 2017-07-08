//Una script chupiguay
#include "AreaTriggerAI.h"
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
    SPELL_ROGUE_MAIN_GAUCHE_ATTACK                = 86392,
    SPELL_ROGUE_MASTER_OF_SUBTLETY_DAMAGE_PERCENT = 31665,
    SPELL_ROGUE_MASTER_OF_SUBTLETY_PASSIVE        = 31223,
    SPELL_ROGUE_MASTERY_MAIN_GAUCHE               = 76806,
    SPELL_ROGUE_SHADOW_DANCE_AURA2                = 185422,
    SPELL_ROGUE_SOOTHING_DARKNESS                 = 200759,
    SPELL_ROGUE_STEALTH                           = 1784,
    SPELL_ROGUE_SUBTERFUGE_AURA                   = 115192,

};


// 193539 Alacrity
class spell_rog_alacrity : public SpellScriptLoader
{
public:
    spell_rog_alacrity() : SpellScriptLoader("spell_rog_alacrity") { }

    class spell_rog_alacrity_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_alacrity_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return false;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_rog_alacrity_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_rog_alacrity_AuraScript();
    }
};

// 185767 Cannonball Barrage
class spell_rog_cannonball_barrage : public SpellScriptLoader
{
public:
    static char constexpr const ScriptName[] = "spell_rog_cannonball_barrage";
    spell_rog_cannonball_barrage() : SpellScriptLoader("spell_rog_cannonball_barrage") { }
    class spell_rog_cannonball_barrage_AuraScript;
    class spell_rog_cannonball_barrage_SpellScript;

    class spell_rog_cannonball_barrage_SpellScript : public SpellScript
    {
        Position pos;
        PrepareSpellScript(spell_rog_cannonball_barrage_SpellScript);

        using CannonAura = spell_rog_cannonball_barrage::spell_rog_cannonball_barrage_AuraScript;
        template<void(CannonAura::*func)(ObjectGuid const&,Position)>

        Position HelperFunction(Position desPos)
        {
            if (Unit* caster = GetCaster())
                if (Aura* atonement = caster->GetAura(185767))
                    if (CannonAura* script = atonement->GetScript<CannonAura>(spell_rog_cannonball_barrage::ScriptName))
                        (script->*func)(GetCaster()->GetGUID(),desPos);

            
        }

        void HandleOnCast()
        {
            pos = GetExplTargetDest()->GetPosition();

            GetCaster()->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), 185779, true); // CanonBall Barrage damage
            HelperFunction<&CannonAura::SetPosition>(pos);
        }

        void Register() override
        {
            AfterHit += SpellHitFn(spell_rog_cannonball_barrage_SpellScript::HandleOnCast);
        }
    public:
        
    };

    class spell_rog_cannonball_barrage_AuraScript : public AuraScript
    {
        Position pos;

        PrepareAuraScript(spell_rog_cannonball_barrage_AuraScript);

        void HandlePeroidic(AuraEffect const* aurEff)
        {
            printf("x %f y %f z %f", pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ());
            GetCaster()->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), 185779, true); // CanonBall Barrage damage
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_rog_cannonball_barrage_AuraScript::HandlePeroidic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    public:
       
        void SetPosition(ObjectGuid const&,Position newPos)
        {
            pos = newPos;
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_rog_cannonball_barrage_AuraScript();
    }

    SpellScript* GetSpellScript() const override
    {
        return new spell_rog_cannonball_barrage_SpellScript();
    }   
};

// 196819 - Eviscerate
class spell_rog_eviscerate : public SpellScriptLoader
{
public:
    spell_rog_eviscerate() : SpellScriptLoader("spell_rog_eviscerate") { }

    class spell_rog_eviscerate_SpellScript : public SpellScript
    {
        bool first = false;
        PrepareSpellScript(spell_rog_eviscerate_SpellScript);

        void HandleOnHit(SpellEffIndex effIndex)
        {
            float extra = 1;
            if (GetCaster()->HasAura(245640)) // SHuriken Combo
            {
                extra += GetCaster()->GetAura(245640)->GetStackAmount() * 0.1f;
                GetCaster()->RemoveAura(245640);
            }

            int8 combo = GetCaster()->ToPlayer()->GetComboPoints();
            SetHitDamage(GetEffectValue() + GetCaster()->ToPlayer()->GetInt32Value(UNIT_FIELD_ATTACK_POWER) * 0.98f * 2 * 1.375f * combo * extra);
        }

        void Register() override
        {
            OnEffectLaunchTarget += SpellEffectFn(spell_rog_eviscerate_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };



    SpellScript* GetSpellScript() const override
    {
        return new spell_rog_eviscerate_SpellScript();
    }
};

// 76806 - Main Gauche (OutLaw Mastery)
class spell_rog_main_gauche : public SpellScriptLoader
{
public:
    spell_rog_main_gauche() : SpellScriptLoader("spell_rog_main_gauche") { }

    class spell_rog_main_gauche_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_main_gauche_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo
            ({
                SPELL_ROGUE_MASTERY_MAIN_GAUCHE,
                SPELL_ROGUE_MAIN_GAUCHE_ATTACK
            });
        }

        void HandleOnProc(ProcEventInfo& eventInfo)
        {
            Unit* target = eventInfo.GetProcTarget();
            GetTarget()->CastSpell(target, SPELL_ROGUE_MAIN_GAUCHE_ATTACK, true);
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return roll_chance_f(GetEffect(EFFECT_0)->GetAmount());
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_rog_main_gauche_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_rog_main_gauche_AuraScript::HandleOnProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_rog_main_gauche_AuraScript();
    }
};

// 137619 - marked-for-death
class spell_rog_marked_for_death : public SpellScriptLoader
{
public:
    spell_rog_marked_for_death() : SpellScriptLoader("spell_rog_marked_for_death") { }

    class spell_rog_marked_for_death_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_marked_for_death_AuraScript);

        void HandleOnRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            if (Unit* caster = GetCaster())
                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
                    GetCaster()->GetSpellHistory()->ResetCooldown(GetSpellInfo()->Id, true); // Marked for death
        }

        void HandlePeroidic(AuraEffect const* aurEff)
        {
            if (Unit* caster = GetCaster())
                if (GetCaster()->GetAppliedAuras().find(137619)->second)
                    if (GetCaster()->GetAppliedAuras().find(137619)->second->GetTarget()->isDead())
                    {
                        caster->GetSpellHistory()->ResetCooldown(GetSpellInfo()->Id); // Marked for death
                        caster->RemoveAura(GetSpellInfo()->Id);
                    }
                        
        }
        
        void Register() override
        {
            AfterEffectRemove += AuraEffectApplyFn(spell_rog_marked_for_death_AuraScript::HandleOnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_rog_marked_for_death_AuraScript();
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

// 195452 Nightblade
class spell_rog_nightblade_aura : public SpellScriptLoader
{
public:
    spell_rog_nightblade_aura() : SpellScriptLoader("spell_rog_nightblade_aura") { }

    class spell_rog_nightblade_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_nightblade_aura_AuraScript);

        void HandleOnProc(ProcEventInfo& eventInfo)
        {
            GetCaster()->CastSpell(GetTarget(), 206760, true); // Nightblade slow
        }

        void Register() override
        {
            OnProc += AuraProcFn(spell_rog_nightblade_aura_AuraScript::HandleOnProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_rog_nightblade_aura_AuraScript();
    }
};

// 222018 - Nightblade Proc
class spell_rog_nightblade : public SpellScriptLoader
{
public:
    spell_rog_nightblade() : SpellScriptLoader("spell_rog_nightblade") { }

    class spell_rog_nightblade_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_nightblade_SpellScript);

        void HandleOnCast()
        {
            GetCaster()->CastSpell(GetExplTargetUnit(), 206760, true); // Nightblade slow
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_rog_nightblade_SpellScript::HandleOnCast);
        }
    };


    SpellScript* GetSpellScript() const override
    {
        return new spell_rog_nightblade_SpellScript();
    }
};

// 185438 - Shadowstrike
class spell_rog_shadowstrike : public SpellScriptLoader
{
public:
    spell_rog_shadowstrike() : SpellScriptLoader("spell_rog_shadowstrike") { }

    class spell_rog_shadowstrike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_shadowstrike_SpellScript);

        void HandleOnCast()
        {
            if (GetCaster()->HasAura(245623)) // Shadowstrike rank 2
            {
                Position  pos = GetExplTargetUnit()->GetFirstCollisionPosition(GetExplTargetUnit()->GetObjectSize(),3.1416f);
                GetCaster()->SendTeleportPacket(pos); // Teleport to back
            }  
        }

        void HandleOnHit(SpellEffIndex effIndex)
        {
            if (GetCaster()->HasAura(196951)) // Strike from shadows passive
                if (Player *player = GetExplTargetUnit()->ToPlayer())
                    GetCaster()->CastSpell(GetExplTargetUnit(), 222775, true); // Strike from shadows daze
                else
                    GetCaster()->CastSpell(GetExplTargetUnit(), 196958, true); // Strike from shadows stun
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_rog_shadowstrike_SpellScript::HandleOnCast);
            OnEffectHit += SpellEffectFn(spell_rog_shadowstrike_SpellScript::HandleOnHit, EFFECT_1, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
        }
    };


    SpellScript* GetSpellScript() const override
    {
        return new spell_rog_shadowstrike_SpellScript();
    }
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

                // Master of Subtlety
                if (AuraEffect const* aurEff = GetTarget()->GetAuraEffect(SPELL_ROGUE_MASTER_OF_SUBTLETY_PASSIVE, EFFECT_0))
                    GetTarget()->CastCustomSpell(SPELL_ROGUE_MASTER_OF_SUBTLETY_DAMAGE_PERCENT, SPELLVALUE_BASE_POINT0, aurEff->GetAmount(), GetTarget(), true);

                if (GetTarget()->HasAura(108209)) // shadow focus passive
                    GetTarget()->CastSpell(GetTarget(), 112942, true); // shadow focus aura

                if (GetTarget()->HasAura(196976)) //Master of Shadows passive
                    GetTarget()->CastSpell(GetTarget(), 196980, true); // Master of Shadows regen aura
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
            {
                if(AuraEffect *aurEffe = GetCaster()->GetAuraEffect(SPELL_ROGUE_SOOTHING_DARKNESS, EFFECT_0))
                    amount += aurEffe->GetAmount();
            }

            void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                //if (GetTarget()->HasAura(108209)) // shadow focus passive
                    GetTarget()->RemoveAura(112942); // shadow focus aura

                    // Master of subtlety
                    if (GetTarget()->HasAura(SPELL_ROGUE_MASTER_OF_SUBTLETY_DAMAGE_PERCENT))
                    {
                        GetTarget()->GetAura(SPELL_ROGUE_MASTER_OF_SUBTLETY_DAMAGE_PERCENT)->SetMaxDuration(GetTarget()->GetAuraEffect(SPELL_ROGUE_MASTER_OF_SUBTLETY_PASSIVE, EFFECT_0)->GetAmount() * 1000);
                        GetTarget()->GetAura(SPELL_ROGUE_MASTER_OF_SUBTLETY_DAMAGE_PERCENT)->SetDuration(GetTarget()->GetAuraEffect(SPELL_ROGUE_MASTER_OF_SUBTLETY_PASSIVE, EFFECT_0)->GetAmount() * 1000);
                    }
            }

            void Register() override
            {     
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_shadow_dance_aura_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_OBS_MOD_HEALTH);
                AfterEffectApply += AuraEffectApplyFn(spell_rog_shadow_dance_aura_AuraScript::HandleOnApply, EFFECT_2, SPELL_AURA_OBS_MOD_HEALTH, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_rog_shadow_dance_aura_AuraScript::HandleEffectRemove, EFFECT_2, SPELL_AURA_OBS_MOD_HEALTH, AURA_EFFECT_HANDLE_REAL);

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

// 196912 - Shadow Techniques
class spell_rog_shadow_techniques : public SpellScriptLoader
{
public:
    spell_rog_shadow_techniques() : SpellScriptLoader("spell_rog_shadow_techniques") { }

    class spell_rog_shadow_techniques_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_shadow_techniques_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo
            ({
                SPELL_ROGUE_MASTERY_MAIN_GAUCHE,
                SPELL_ROGUE_MAIN_GAUCHE_ATTACK
            });
        }

        void HandleOnProc(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            uint32 stacks = irand(3, 5);
            if (GetEffect(EFFECT_0)->GetAmount() == 0)
            {
                GetEffect(EFFECT_0)->SetAmount(stacks);
            }
            else
            {
                GetEffect(EFFECT_0)->SetAmount(GetEffect(EFFECT_0)->GetAmount() - 1);
                if (GetEffect(EFFECT_0)->GetAmount() == 0)
                {
                    GetTarget()->CastSpell((Unit*)nullptr, 196911, true);
                    GetEffect(EFFECT_0)->SetAmount(stacks);
                }
            }   
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            return true;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_rog_shadow_techniques_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_rog_shadow_techniques_AuraScript::HandleOnProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_rog_shadow_techniques_AuraScript();
    }
};

// 197835 Shuriken Storm
class spell_rog_shuriken_storm : public SpellScriptLoader
{
public:
    spell_rog_shuriken_storm() : SpellScriptLoader("spell_rog_shuriken_storm") { }

    class spell_rog_shuriken_storm_SpellScript : public SpellScript
    {
        bool first = false;
        PrepareSpellScript(spell_rog_shuriken_storm_SpellScript);

        void HandleOnHit(SpellEffIndex effIndex)
        {
            if (GetCaster()->HasAuraType(SPELL_AURA_MOD_STEALTH))
                SetHitDamage(GetHitDamage() * 2);
            GetCaster()->CastSpell((Unit*)nullptr, 212743, true);   // Shuriken Storm energize

            if (GetCaster()->HasAura(245639) && first) //Shuriken Combo Passive
            {
                GetCaster()->CastSpell((Unit*)nullptr, 245640, true); // Shuriken Combo Aura
            }

            first = true;
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_rog_shuriken_storm_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };



    SpellScript* GetSpellScript() const override
    {
        return new spell_rog_shuriken_storm_SpellScript();
    }
};

// 115192 - Subterfuge effect
class spell_rog_subterfuge_aura : public SpellScriptLoader
{
public:
    spell_rog_subterfuge_aura() : SpellScriptLoader("spell_rog_subterfuge_aura") { }

    class spell_rog_subterfuge_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_subterfuge_aura_AuraScript);

        void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            if (!GetCaster())
                return;

            // We need to call Subterfuge aura, if stealth is removed by AOE damage (AURA_REMOVE_BY_DEFAULT) or other spells
            if (Aura* subterfuge = GetCaster()->GetAura(115191))
                GetCaster()->RemoveAura(subterfuge, AURA_REMOVE_BY_CANCEL);
        }

        void CorrectEffect(AuraEffect const* aurEff, AuraEffectHandleModes mode)
        {
            PreventDefaultAction();
            printf("hey");
        }

        void Register()
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_rog_subterfuge_aura_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectApply += AuraEffectApplyFn(spell_rog_subterfuge_aura_AuraScript::CorrectEffect, EFFECT_0, SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_rog_subterfuge_aura_AuraScript();
    }
};

// 193537 - WeaponMaster
class spell_rog_weaponmaster : public SpellScriptLoader
{
public:
    spell_rog_weaponmaster() : SpellScriptLoader("spell_rog_weaponmaster") { }

    class spell_rog_weaponmaster_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_weaponmaster_AuraScript);

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (roll_chance_i(GetEffect(EFFECT_0)->GetAmount()))
            {
                uint32 damage = eventInfo.GetDamageInfo()->GetDamage();

                if (Player *play = GetTarget()->ToPlayer())
                    play->ApplySpellMod(193536, SPELLMOD_DAMAGE, damage);

                GetTarget()->CastCustomSpell(193536, SPELLVALUE_BASE_POINT0, damage, eventInfo.GetProcTarget(), true, NULL);
                
            }
          return true;
        }
        void HandlePrepProc(ProcEventInfo& eventInfo)
        {
        }

        void HandleOnProc(ProcEventInfo& eventInfo)
        {
        }


        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_rog_weaponmaster_AuraScript::CheckProc);
            OnProc += AuraProcFn(spell_rog_weaponmaster_AuraScript::HandleOnProc);
            DoPrepareProc += AuraProcFn(spell_rog_weaponmaster_AuraScript::HandlePrepProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_rog_weaponmaster_AuraScript();
    }
};

void AddSC_spell_rog_spell_scripts_two()
{
    new spell_rog_alacrity();
    new spell_rog_cannonball_barrage();
    new spell_rog_eviscerate();
    new spell_rog_main_gauche();
    new spell_rog_marked_for_death();
    new spell_rog_master_of_subtlety_passive();
    new spell_rog_nightblade();
    new spell_rog_nightblade_aura();
    new spell_rog_shadow_dance_aura();
    new spell_rog_shadowstrike();
    new spell_rog_shadow_techniques();
    new spell_rog_shuriken_storm();
    new spell_rog_subterfuge_aura();
    new spell_rog_weaponmaster();
}