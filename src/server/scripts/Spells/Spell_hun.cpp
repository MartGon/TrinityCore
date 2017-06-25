//Una script chupiguay
#include "PlayerAI.h"
#include "Pet.h"
#include "AreaTrigger.h"
#include "AreaTriggerDataStore.h"
#include "AreaTriggerPackets.h"
#include "AreaTriggerTemplate.h"
#include "CellImpl.h"
#include "Chat.h"
#include "DB2Stores.h"
#include "GridNotifiersImpl.h"
#include "Language.h"
#include "Log.h"
#include "Object.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellInfo.h"
#include "Transport.h"
#include "Unit.h"
#include "UpdateData.h"
#include "AreaTriggerAI.h"
#include <stdio.h>
#include <iostream>
#include "SpellAuraEffects.h"
#include "SpellMgr.cpp"
#include "ScriptMgr.h"
#include "CellImpl.h"
#include "GridNotifiersImpl.h"
#include "Pet.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "SpellMgr.h"
#include "SpellScript.h"

enum HunterSpells
{
    SPELL_HUNTER_ANIMAL_INSTINCTS               = 204315,
    SPELL_HUNTER_ARCANE_SHOT                    = 185358,
    SPELL_HUNTER_ASPECT_EAGLE                   = 186289,
    SPELL_HUNTER_ASPECT_EAGLE_PET               = 231555,
    SPELL_HUNTER_ASPECT_BEAST                   = 191384,
    SPELL_HUNTER_AUTO_SHOT                      = 75,
    SPELL_HUNTER_BESTIAL_CUNNING                = 191397,
    SPELL_HUNTER_BESTIAL_TENACITY               = 191414,
    SPELL_HUNTER_BESTIAL_FEROCITY               = 191413,
    SPELL_HUNTER_BOMBARDMENT_TALENT             = 35110,
    SPELL_HUNTER_BOMBARDMENT_SPELL              = 82921,
    SPELL_HUNTER_CAMOUFLAGE_AURA                = 199483,
    SPELL_HUNTER_CALTROPS_DAMAGE                = 194279,
    SPELL_HUNTER_HUNTING_COMPANION_MASTERY      = 191334,
    SPELL_HUNTER_HUNTING_COMPANION_GAIN_CHARGE  = 191335,
    SPELL_HUNTER_HUNTING_COMPANION_AURA         = 191336,
    SPELL_HUNTER_FLANKING_STRIKE                = 202800,
    SPELL_HUNTER_FLANKING_STRIKE_MASTER         = 205434,
    SPELL_HUNTER_FLANKING_STRIKE_R2             = 237327,
    SPELL_HUNTER_FLANKING_STRIKE_PET            = 204740,
    SPELL_HUNTER_FLARE                          = 1543,
    SPELL_HUNTER_FLARE_AREA_TRIGGER             = 132950,
    SPELL_HUNTER_FLARE_AURA                     = 140691,
    SPELL_HUNTER_FREEZING_TRAP                  = 187650,
    SPELL_HUNTER_FREEZING_TRAP_AREA_TRIGGER     = 187651,
    SPELL_HUNTER_FREEZING_TRAP_AURA             = 3355,/*55041*/
    SPELL_HUNTER_EXPLOSIVE_TRAP_DMG             = 13812,
    SPELL_HUNTER_EXPLOSIVE_TRAP_BLIND           = 237338,
    SPELL_HUNTER_HARPOON_JUMP                   = 57604,
    SPELL_HUNTER_HARPOON                        = 190925,
    SPELL_HUNTER_HARPOON_ROOT                   = 190927,
    SPELL_HUNTER_MOKNATHAL_TALENT               = 201082,
    SPELL_HUNTER_MOKNATHAL_AURA                 = 201081,
    SPELL_HUNTER_MONGOOSE_BITE                  = 190928,
    SPELL_HUNTER_MONGOOSE_FURY                  = 190931,
    SPELL_HUNTER_MORTAL_WOUNDS_AURA             = 201075,
    SPELL_HUNTER_MULTISHOT                      = 2643,
    SPELL_HUNTER_MURDER_CROWS_DAMAGE            = 131900,
    SPELL_HUNTER_POSTHASTE                      = 109215,
    SPELL_HUNTER_POSTHASTE_BUFF                 = 118922,
    SPELL_HUNTER_RAPTOR_STRIKE                  = 186270,
    SPELL_HUNTER_SERPENT_STING                  = 87935,
    SPELL_HUNTER_SERPENT_STING_AURA             = 118253,
    SPELL_HUNTER_STEADY_FOCUS                   = 193534,
    SPELL_HUNTER_STICKY_BOMB_EXPLOSION          = 191244,
    SPELL_HUNTER_TAR_TRAP_AREA2                 = 187700,
    SPELL_HUNTER_TAR_TRAP_SLOW                  = 135299,
    SPELL_HUNTER_TAR_TRAP_SLOW_WL               = 236699, // WL - WayLay
    SPELL_HUNTER_THRILL_OF_THE_HUNT             = 34720,
    SPELL_HUNTER_THROWING_AXES                  = 200167,
    SPELL_HUNTER_WAYLAY                         = 234955
};

    // Aspect of the Eagle 186289
class spell_hun_aspect_eagle : public SpellScriptLoader
{
public:
    spell_hun_aspect_eagle() : SpellScriptLoader("spell_hun_aspect_eagle") { }

    class spell_hun_aspect_eagle_SpellScript : public SpellScript
    {

        PrepareSpellScript(spell_hun_aspect_eagle_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_HUNTING_COMPANION_AURA))
                return false;
            return true;
        }

        void HandleAfterCast()
        {
            if (Unit *pet = GetCaster()->GetGuardianPet())
                if (AuraEffect const* aurEff = GetCaster()->GetAuraEffectOfRankedSpell(SPELL_HUNTER_ASPECT_EAGLE_PET, EFFECT_0))
                {
                    int32 basepoints = aurEff->GetAmount();
                    pet->CastCustomSpell(pet, SPELL_HUNTER_ASPECT_EAGLE, &basepoints, NULL, NULL, true, NULL, aurEff);
                }
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_hun_aspect_eagle_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_aspect_eagle_SpellScript();
    }
};

    // Bombardment 35110
class spell_hun_bombardment : public SpellScriptLoader
{
public:
    spell_hun_bombardment() : SpellScriptLoader("spell_hun_bombardment") { }

    class spell_hun_bombardment_AuraScript : public AuraScript
    {

        PrepareAuraScript(spell_hun_bombardment_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_BOMBARDMENT_TALENT))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetActor()->HasAura(SPELL_HUNTER_BOMBARDMENT_TALENT))
                if (eventInfo.GetHitMask() & PROC_HIT_CRITICAL && eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_MULTISHOT)
                {
                    return true;
                }
            return false;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_bombardment_AuraScript::CheckProc);

        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_bombardment_AuraScript();
    }
};

    // Caltrops 194278
class areatrigger_hun_caltrops : public AreaTriggerEntityScript
{
public:
    areatrigger_hun_caltrops() : AreaTriggerEntityScript("areatrigger_hun_caltrops") { }

    struct areatrigger_hun_caltropsAI : AreaTriggerAI
    {
        areatrigger_hun_caltropsAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

        // Called when the AreaTrigger has just been initialized, just before added to map

        void OnUnitEnter(Unit* unit) override
        {
            if (Unit* caster = at->GetCaster())
            {
                if (caster->IsValidAttackTarget(unit))
                {
                    caster->CastSpell(unit, SPELL_HUNTER_CALTROPS_DAMAGE, true);
                }
            }
        }

        void OnUpdate(uint32 /*diff*/) override
        {
            for (ObjectGuid const& exitUnitGuid : at->GetInsideUnits())
            {
                if (Unit* unit = ObjectAccessor::GetUnit(*at, exitUnitGuid))
                    if (!unit->HasAura(SPELL_HUNTER_CALTROPS_DAMAGE) && at->GetCaster()->IsValidAttackTarget(unit))
                        at->GetCaster()->CastSpell(unit, SPELL_HUNTER_CALTROPS_DAMAGE, true);
            }
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new areatrigger_hun_caltropsAI(areatrigger);
    }
};

    // Camouflage 199483
class spell_hun_camouflage : public SpellScriptLoader
{
    public:
        spell_hun_camouflage() : SpellScriptLoader("spell_hun_camouflage") { }

        class spell_hun_camouflage_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_camouflage_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                if (Unit* pet = GetCaster()->GetGuardianPet())
                    pet->CastSpell(pet, SPELL_HUNTER_CAMOUFLAGE_AURA, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/) {
                if (Unit* pet = GetCaster()->GetGuardianPet())
                    pet->RemoveAura(SPELL_HUNTER_CAMOUFLAGE_AURA);
                else
                    GetCaster()->GetOwner()->RemoveAura(SPELL_HUNTER_CAMOUFLAGE_AURA);

            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_hun_camouflage_AuraScript::OnApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_hun_camouflage_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_hun_camouflage_AuraScript();
        }
};

    // Carve 187708
class spell_hun_carve : public SpellScriptLoader
{
public:
    spell_hun_carve() : SpellScriptLoader("spell_hun_carve") { }

    class spell_hun_carve_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_carve_SpellScript);

        void HandleAfterHit()
        {
            Unit *caster = GetCaster();
            if (caster->HasSpell(SPELL_HUNTER_SERPENT_STING))
                caster->CastSpell(GetHitUnit(), SPELL_HUNTER_SERPENT_STING_AURA, true);
        }

        void Register() override
        {
            AfterHit += SpellHitFn(spell_hun_carve_SpellScript::HandleAfterHit);
        }

    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_carve_SpellScript();
    }
};

    // .learn pet 240152
    // 202800 - Flanking Strike
class spell_hun_flanking_strike : public SpellScriptLoader
{
public:
    spell_hun_flanking_strike() : SpellScriptLoader("spell_hun_flanking_strike") { }

    class spell_hun_flanking_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_flanking_strike_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo
            ({
                SPELL_HUNTER_FLANKING_STRIKE_PET
            });
        }

        void HandleOnCast()
        {
            if (Unit *target = GetExplTargetUnit())
                if (Unit* guardian = GetCaster()->GetGuardianPet())
                    guardian->CastSpell(target, SPELL_HUNTER_FLANKING_STRIKE_PET, true);
        }

        void CalculateDamage(SpellEffIndex /*effIndex*/)
        {
            if (Unit *target = GetHitUnit())
                if (target->GetVictim() != GetCaster())
                    SetHitDamage(GetHitDamage()*1.5);
        }

        SpellCastResult CheckCast()
        {
            if (Unit* guardian = GetCaster()->GetGuardianPet())
            {
                if (Unit *target = GetExplTargetUnit())
                {
                    if (target->GetDistance(guardian->GetPosition()) > 8)
                    {
                        SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_PET_OUT_OF_RANGE);
                        return SPELL_FAILED_CUSTOM_ERROR;
                    }
                    else if (!guardian->IsAlive())
                    {
                        SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_PET_IS_DEAD);
                        return SPELL_FAILED_CUSTOM_ERROR;
                    }
                    return SPELL_CAST_OK;
                }
            }
            return SPELL_FAILED_SUCCESS;
        }

        void HandleCooldowns()
        {
            if (AuraEffect *aura = GetCaster()->GetAuraEffect(SPELL_HUNTER_ANIMAL_INSTINCTS, EFFECT_0))
            {
                std::vector<int32> spellCds;
                std::vector<int32> spellIDs;

                spellIDs.push_back(SPELL_HUNTER_ASPECT_EAGLE);
                spellIDs.push_back(SPELL_HUNTER_FLANKING_STRIKE);
                spellIDs.push_back(SPELL_HUNTER_HARPOON);
                spellIDs.push_back(SPELL_HUNTER_MONGOOSE_BITE);

                for (std::vector<int32>::iterator itr = spellIDs.begin(); itr != spellIDs.end(); ++itr)
                    if (GetCaster()->GetSpellHistory()->HasCooldown(sSpellMgr->GetSpellInfo(*itr))
                        || !GetCaster()->GetSpellHistory()->HasCharge(sSpellMgr->GetSpellInfo(*itr)->ChargeCategoryId))
                        spellCds.push_back(*itr);

                int32 selector = irand(0, spellCds.size() - 1);
                int32 value = 0;
                value = -GetCaster()->GetFloatValue(UNIT_MOD_CAST_HASTE) * sSpellMgr->GetSpellInfo(SPELL_HUNTER_FLANKING_STRIKE)->RecoveryTime * 0.5f;

                if (GetCaster()->GetSpellHistory()->HasCooldown(sSpellMgr->GetSpellInfo(spellCds.at(selector))))
                    GetCaster()->GetSpellHistory()->ModifyCooldown(spellCds.at(selector), value);
                else
                    GetCaster()->GetSpellHistory()->ModifyCharge(sSpellMgr->GetSpellInfo(spellCds.at(selector))->ChargeCategoryId, value);
            }

            if (AuraEffect *aura = GetCaster()->GetAuraEffect(SPELL_HUNTER_ASPECT_BEAST, EFFECT_0))
            {
                Unit *target = GetHitUnit();
                Pet *pet = GetCaster()->ToPlayer()->GetPet();

                uint16 spec = GetCaster()->GetGuardianPet()->ToPet()->GetSpecialization();
                // tenacity = 81
                // cunning = 79
                // ferocity = 74

                switch (spec)
                {
                case 81:
                    pet->CastSpell(target, SPELL_HUNTER_BESTIAL_TENACITY, true);
                    break;
                case 79:
                    pet->CastSpell(target, SPELL_HUNTER_BESTIAL_CUNNING, true);
                    break;
                case 74:
                    pet->CastSpell(target, SPELL_HUNTER_BESTIAL_FEROCITY, true);
                    break;
                default:
                    break;
                }


            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_hun_flanking_strike_SpellScript::CalculateDamage, EFFECT_0, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            OnCast += SpellCastFn(spell_hun_flanking_strike_SpellScript::HandleOnCast);
            OnCheckCast += SpellCheckCastFn(spell_hun_flanking_strike_SpellScript::CheckCast);
            AfterHit += SpellHitFn(spell_hun_flanking_strike_SpellScript::HandleCooldowns);
        }

    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_flanking_strike_SpellScript();
    }
};

    // 206933 - Flanking Strike pet's version
class spell_hun_flanking_strike_pet : public SpellScriptLoader
{
public:
    spell_hun_flanking_strike_pet() : SpellScriptLoader("spell_hun_flanking_strike_pet") { }

    class spell_hun_flanking_strike_pet_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_flanking_strike_pet_SpellScript);


        SpellCastResult CheckCast()
        {
            if (!GetCaster()->IsPet())
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

            return SPELL_CAST_OK;
        }

        void CalculateDamage(SpellEffIndex effIndex)
        {

            if (Unit *target = GetHitUnit())
            {
                int32 ap = GetCaster()->GetOwner()->ToPlayer()->GetInt32Value(UNIT_FIELD_ATTACK_POWER);
                float vers = GetCaster()->GetOwner()->ToPlayer()->GetFloatValue(PLAYER_VERSATILITY);
                int32 level = GetCaster()->getLevel();
                //formula taken from wowhead
                float result = 1.5*(ap*2.152)*(1 + vers)*(std::min(20, level)*0.05f);

                if (target->GetVictim() == GetCaster()->GetOwner() && GetCaster()->IsPet())
                {
                    SetHitDamage(result*1.5);
                    GetCaster()->getThreatManager().modifyThreatPercent(GetCaster()->GetVictim(), 400);
                }
                else
                    SetHitDamage(result);
            }
        }


        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_hun_flanking_strike_pet_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_hun_flanking_strike_pet_SpellScript::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }

    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_flanking_strike_pet_SpellScript();
    }


};

    // Flare 1543
class spell_hun_flare_trigger : public SpellScriptLoader
{
public:
    spell_hun_flare_trigger() : SpellScriptLoader("spell_hun_flare_trigger") { }

    class spell_hun_flare_trigger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_flare_trigger_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_FLARE))
                return false;
            return true;
        }

        void HandleEffectDummy(SpellEffIndex /*effIndex*/)
        {
            Position destPos = GetHitDest()->GetPosition();
            SpellCastTargets targets;
            targets.SetDst(destPos);
            GetCaster()->CastSpell(targets, sSpellMgr->GetSpellInfo(SPELL_HUNTER_FLARE_AREA_TRIGGER), nullptr);
        
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_hun_flare_trigger_SpellScript::HandleEffectDummy, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_flare_trigger_SpellScript();
    }
};
    
    // Flare Areatrigger  1543
class areatrigger_hun_flare : public AreaTriggerEntityScript
{
public:
    areatrigger_hun_flare() : AreaTriggerEntityScript("areatrigger_hun_flare") { }

    struct areatrigger_hun_flareAI : AreaTriggerAI
    {
        areatrigger_hun_flareAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

        // Called when the AreaTrigger has just been initialized, just before added to map

        void OnUnitEnter(Unit* unit) override
        {
            if (Unit* caster = at->GetCaster())
            {
                if (caster->IsHostileTo(unit))
                {
                    unit->RemoveAurasByType(SPELL_AURA_MOD_STEALTH);
                }
            }
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new areatrigger_hun_flareAI(areatrigger);
    }
};

    // Freezing trap 187561
class areatrigger_hun_freezing_trap : public AreaTriggerEntityScript
{
public:
    areatrigger_hun_freezing_trap() : AreaTriggerEntityScript("areatrigger_hun_freezing_trap") { }

    struct areatrigger_hun_freezing_trapAI : AreaTriggerAI
    {
        areatrigger_hun_freezing_trapAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

        // Called when the AreaTrigger has just been initialized, just before added to map
        void OnInitialize() override 
        {
            if (Unit* caster = at->GetCaster())
            {
                caster->RemoveAreaTrigger(SPELL_HUNTER_FREEZING_TRAP_AREA_TRIGGER);
            }
        }

        void OnUnitEnter(Unit* unit) override
        {
            if (Unit* caster = at->GetCaster())
            {
                if (caster->IsValidAttackTarget(unit))
                {
                    caster->CastSpell(unit, SPELL_HUNTER_FREEZING_TRAP_AURA, true);
                    at->SetDuration(0);
                }
            }
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new areatrigger_hun_freezing_trapAI(areatrigger);
    }
};

    // Harpoon 190925
class spell_hun_harpoon : public SpellScriptLoader
{
public:
    spell_hun_harpoon() : SpellScriptLoader("spell_hun_harpoon") { }

    class spell_hun_harpoon_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_harpoon_SpellScript);

        float castedOrientation = 0;

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            const_cast<SpellEffectInfo*>(sSpellMgr->GetSpellInfo(SPELL_HUNTER_HARPOON_JUMP)->GetEffect(EFFECT_0))->MiscValue = 10;
            const_cast<SpellEffectInfo*>(sSpellMgr->GetSpellInfo(SPELL_HUNTER_HARPOON_JUMP)->GetEffect(EFFECT_0))->MiscValueB = 150;

            Position  pos = GetHitUnit()->GetFirstCollisionPosition(GetHitUnit()->GetObjectSize(), GetHitUnit()->GetRelativeAngle(GetCaster()));

            if (Unit* target = GetHitUnit())
            {
                if (!target->HasAuraType(SPELL_AURA_DEFLECT_SPELLS)) // Deterrence
                    GetCaster()->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_HUNTER_HARPOON_JUMP, true);
                if (GetCaster()->HasAura(SPELL_HUNTER_POSTHASTE)) 
                {
                    GetCaster()->CastSpell(GetCaster(), SPELL_HUNTER_POSTHASTE_BUFF, true);
                    GetCaster()->RemoveAurasWithMechanic((1 << MECHANIC_ROOT) | (1 << MECHANIC_SNARE));
                }
            }
        }

        void OnCaste()
        {	
            castedOrientation = GetCaster()->GetOrientation();
            if(Unit *target = GetExplTargetUnit())
                GetCaster()->CastSpell(target, SPELL_HUNTER_HARPOON_ROOT, true);   
        }

        SpellCastResult CheckCast()
        {
            if(GetCaster()->HasUnitState(UNIT_STATE_ROOT))
            { 
                if (GetCaster()->HasAura(SPELL_HUNTER_POSTHASTE))
                {
                    
                    return SPELL_CAST_OK;
                }
                
                return SPELL_FAILED_ROOTED;
            }
            
            return SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_hun_harpoon_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_hun_harpoon_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnCast += SpellCastFn(spell_hun_harpoon_SpellScript::OnCaste);
        }

    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_harpoon_SpellScript();
    }
};

    // Explosive Trap 13813
class areatrigger_hun_explosive_trap : public AreaTriggerEntityScript
{
public:
    areatrigger_hun_explosive_trap() : AreaTriggerEntityScript("areatrigger_hun_explosive_trap") { }

    struct areatrigger_hun_explosive_trapAI : AreaTriggerAI
    {
        areatrigger_hun_explosive_trapAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

        // Called when the AreaTrigger has just been initialized, just before added to map

        void OnUnitEnter(Unit* unit) override
        {
            if (Unit* caster = at->GetCaster())
            {
                if (caster->IsValidAttackTarget(unit))
                {
                    Position pos = at->GetPosition();
                    caster->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), SPELL_HUNTER_EXPLOSIVE_TRAP_DMG, true); //daño
                    if (caster->HasAura(SPELL_HUNTER_WAYLAY) && at->GetTimeSinceCreated()>2000)
                        caster->CastSpell(unit, SPELL_HUNTER_EXPLOSIVE_TRAP_BLIND, true); //cegar
                    at->SetDuration(0);
                }
            }
        }

        void OnUpdate(uint32 /*diff*/) override
        {
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new areatrigger_hun_explosive_trapAI(areatrigger);
    }
};

// 191335 - Hunting Companion (gain charge)
class spell_hun_hunting_companion_gain_charge : public SpellScriptLoader
{
public:
    spell_hun_hunting_companion_gain_charge() : SpellScriptLoader("spell_hun_hunting_companion_gain_charge") { }

    class spell_hun_hunting_companion_gain_charge_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_hunting_companion_gain_charge_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo
            ({
                SPELL_HUNTER_HUNTING_COMPANION_AURA,
                SPELL_HUNTER_MONGOOSE_BITE
            });
        }

        void OnCaste()
        {
            Unit *caster = GetCaster();
            if (Unit *owner = GetCaster()->GetOwner())
                if (caster->HasAura(SPELL_HUNTER_HUNTING_COMPANION_AURA))
                    owner->GetSpellHistory()->RestoreCharge(sSpellMgr->GetSpellInfo(SPELL_HUNTER_MONGOOSE_BITE)->ChargeCategoryId);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_hun_hunting_companion_gain_charge_SpellScript::OnCaste);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_hunting_companion_gain_charge_SpellScript();
    }
};

// 191336 - Hunting Companion (Pet Aura)
class spell_hun_hunting_companion : public SpellScriptLoader
{
public:
    spell_hun_hunting_companion() : SpellScriptLoader("spell_hun_hunting_companion") { }

    class spell_hun_hunting_companion_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_hunting_companion_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo
            ({
                SPELL_HUNTER_ASPECT_EAGLE,
                SPELL_HUNTER_FLANKING_STRIKE_PET,
                SPELL_HUNTER_FLANKING_STRIKE_R2,
                SPELL_HUNTER_HUNTING_COMPANION_MASTERY
            });
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (Unit *owner = GetTarget()->GetOwner())
                if (Aura *mast = owner->GetAura(SPELL_HUNTER_HUNTING_COMPANION_MASTERY))
                {
                    if (SpellInfo const* spellInfo = eventInfo.GetSpellInfo())
                    {
                        if (spellInfo->Id == SPELL_HUNTER_FLANKING_STRIKE_PET)
                        {
                            if (GetTarget()->HasAura(SPELL_HUNTER_ASPECT_EAGLE))
                            {
                                // Aspect of the Eagle + Flanking strike;
                                if (owner->HasAura(SPELL_HUNTER_FLANKING_STRIKE_R2))
                                    return roll_chance_f(mast->GetEffect(EFFECT_0)->GetAmount() * 2 + owner->GetAura(SPELL_HUNTER_ASPECT_EAGLE)->GetEffect(EFFECT_0)->GetAmount());
                                else
                                    return roll_chance_f(mast->GetEffect(EFFECT_0)->GetAmount() + owner->GetAura(SPELL_HUNTER_ASPECT_EAGLE)->GetEffect(EFFECT_0)->GetAmount());
                            }
                            // Flanking Strike
                            if (owner->HasAura(SPELL_HUNTER_FLANKING_STRIKE_R2))
                                return roll_chance_f(mast->GetEffect(EFFECT_0)->GetAmount() * 2);
                            else
                                return roll_chance_f(mast->GetEffect(EFFECT_0)->GetAmount());
                        }
                        // Basic Attack
                        return roll_chance_f(mast->GetEffect(EFFECT_0)->GetAmount());
                    }
                    if (GetTarget()->HasAura(SPELL_HUNTER_ASPECT_EAGLE))   // Aspect of the Eagle 
                        return roll_chance_f(mast->GetEffect(EFFECT_0)->GetAmount() + owner->GetAura(SPELL_HUNTER_ASPECT_EAGLE)->GetEffect(EFFECT_0)->GetAmount());
                }
            return false;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_hunting_companion_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_hunting_companion_AuraScript();
    }
};

    // Moknathal 201082
class spell_hun_moknathal_talent : public SpellScriptLoader
{
public:
    spell_hun_moknathal_talent() : SpellScriptLoader("spell_hun_moknathal_talent") { }

    class spell_hun_moknathal_talent_AuraScript : public AuraScript
    {

        PrepareAuraScript(spell_hun_moknathal_talent_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_MOKNATHAL_TALENT))
                return false;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            if (eventInfo.GetActor()->HasAura(SPELL_HUNTER_MOKNATHAL_TALENT))
                if (eventInfo.GetSpellInfo()->Id == SPELL_HUNTER_RAPTOR_STRIKE)
                {
                    return true;
                }
            return false;
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(spell_hun_moknathal_talent_AuraScript::CheckProc);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_moknathal_talent_AuraScript();
    }
};

    // Mongoose Bite 190928
class spell_hun_mongoose_bite : public SpellScriptLoader
{
    public:
        spell_hun_mongoose_bite() : SpellScriptLoader("spell_hun_mongoose_bite") { }

        class spell_hun_mongoose_bite_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_mongoose_bite_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo
                ({
                    SPELL_HUNTER_MONGOOSE_FURY
                });
            }

            void OnCaste()
            {
                Unit *caster = GetCaster();
                if (Aura *aura = caster->GetAura(SPELL_HUNTER_MONGOOSE_FURY))
                {
                    int32 time_left = aura->GetDuration();
                    caster->CastSpell(caster, SPELL_HUNTER_MONGOOSE_FURY, true);
                    caster->GetAura(SPELL_HUNTER_MONGOOSE_FURY)->SetDuration(time_left);
                }
                else
                    caster->CastSpell(caster, SPELL_HUNTER_MONGOOSE_FURY, true);
            }

            void Register() override
            {
                AfterHit += SpellHitFn(spell_hun_mongoose_bite_SpellScript::OnCaste);
            }

        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_hun_mongoose_bite_SpellScript();
        }
};

    // Mortal Wounds 201091
class spell_hun_mortal_wounds : public SpellScriptLoader
{
    public:
        spell_hun_mortal_wounds() : SpellScriptLoader("spell_hun_mortal_wounds") { }

        class spell_hun_mortal_wounds_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_mortal_wounds_SpellScript);

            void OnCaste()
            {
                Unit *caster = GetCaster();
                    if (Aura *aura = caster->GetAura(SPELL_HUNTER_MORTAL_WOUNDS_AURA))
                        caster->GetSpellHistory()->RestoreCharge(sSpellMgr->GetSpellInfo(SPELL_HUNTER_MONGOOSE_BITE)->ChargeCategoryId);
            }

            void Register() override
            {
                OnCast += SpellCastFn(spell_hun_mortal_wounds_SpellScript::OnCaste);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_hun_mortal_wounds_SpellScript();
        }
};

    // Murder of crows 206505
class spell_hun_murder_crows : public SpellScriptLoader
{
    public:
        spell_hun_murder_crows() : SpellScriptLoader("spell_hun_murder_crows") { }

        class spell_hun_murder_crows_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_murder_crows_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_HUNTER_MURDER_CROWS_DAMAGE))
                    return false;
                return true;
            }

            void HandlePeriodic(AuraEffect const* aurEff)
            {
                if(Unit* caster = GetCaster())
                    caster->CastSpell(GetTarget(), SPELL_HUNTER_MURDER_CROWS_DAMAGE, true);
            }

            void HandleOnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
                    GetCaster()->GetSpellHistory()->ResetCooldown(GetSpellInfo()->Id, true);
            }


            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_hun_murder_crows_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                AfterEffectRemove += AuraEffectRemoveFn(spell_hun_murder_crows_AuraScript::HandleOnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_hun_murder_crows_AuraScript();
        }
};

    // Raptor Strike 186270
class spell_hun_raptor_strike : public SpellScriptLoader
{
    public:
        spell_hun_raptor_strike() : SpellScriptLoader("spell_hun_raptor_strike") { }

        class spell_hun_raptor_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_raptor_strike_SpellScript);

            void HandleAfterHit()
            {
                std::cout << "raptor\n";
                Unit *caster = GetCaster();
            
                if (caster->HasSpell(87935))
                    caster->CastSpell(GetHitUnit(), 118253, true);
            }

            void Register() override
            {
                AfterHit += SpellHitFn(spell_hun_raptor_strike_SpellScript::HandleAfterHit);
            }

        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_hun_raptor_strike_SpellScript();
        }
};

    // Tar trap 1 areatrigger 187699
class areatrigger_hun_tar_trap : public AreaTriggerEntityScript
{
public:
    areatrigger_hun_tar_trap() : AreaTriggerEntityScript("areatrigger_hun_tar_trap") { }

    struct areatrigger_hun_tar_trapAI : AreaTriggerAI
    {
        areatrigger_hun_tar_trapAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

        // Called when the AreaTrigger has just been initialized, just before added to map

        void OnUnitEnter(Unit* unit) override
        {
            if (Unit* caster = at->GetCaster())
            {
                if (caster->IsValidAttackTarget(unit))
                {
                    caster->CastSpell(at->GetPositionX(),at->GetPositionY(),at->GetPositionZ(), SPELL_HUNTER_TAR_TRAP_AREA2, true);
                    if (caster->IsValidAttackTarget(unit))
                    {
                        if (caster->HasAura(SPELL_HUNTER_WAYLAY) && at->GetTimeSinceCreated()>2000)
                            caster->CastSpell(unit, SPELL_HUNTER_TAR_TRAP_SLOW_WL, true); //spuer pegajoso 
                    }
                    at->SetDuration(0);
                }
            }
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new areatrigger_hun_tar_trapAI(areatrigger);
    }
};

    // Tar trap 2 areatrigger 187700
class areatrigger_hun_tar_trap_pool : public AreaTriggerEntityScript
{
public:
    areatrigger_hun_tar_trap_pool() : AreaTriggerEntityScript("areatrigger_hun_tar_trap_pool") { }

    struct areatrigger_hun_tar_trap_poolAI : AreaTriggerAI
    {
        areatrigger_hun_tar_trap_poolAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

        // Called when the AreaTrigger has just been initialized, just before added to map

        void OnUnitEnter(Unit* unit) override
        {
            if (Unit* caster = at->GetCaster())
            {
                if (caster->IsValidAttackTarget(unit))
                {
                    caster->CastSpell(unit, SPELL_HUNTER_TAR_TRAP_SLOW, true);
                }
            }
        }

        void OnUnitExit(Unit* unit) override
        {
            if (Unit* caster = at->GetCaster())
            {
                if (caster->IsValidAttackTarget(unit))
                {
                    unit->RemoveAurasDueToSpell(SPELL_HUNTER_TAR_TRAP_SLOW);
                }
            }
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new areatrigger_hun_tar_trap_poolAI(areatrigger);
    }
};

    // Throwing Axes 200163
class spell_hun_throwing_axes : public SpellScriptLoader
{
public:
    spell_hun_throwing_axes() : SpellScriptLoader("spell_hun_throwing_axes") { }

    class spell_hun_throwing_axes_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_throwing_axes_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
        }

        void OnCaste()
        {
            GetCaster()->CastSpell(GetExplTargetUnit(), SPELL_HUNTER_THROWING_AXES);
            GetCaster()->CastSpell(GetExplTargetUnit(), SPELL_HUNTER_THROWING_AXES);
            GetCaster()->CastSpell(GetExplTargetUnit(), SPELL_HUNTER_THROWING_AXES);
        }

        SpellCastResult CheckCast()
        {
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_hun_throwing_axes_SpellScript::OnCaste);
        }

    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_throwing_axes_SpellScript();
    }
};

    // Snake hunter 201078
class spell_hun_snake_hunter : public SpellScriptLoader
{
public:
    spell_hun_snake_hunter() : SpellScriptLoader("spell_hun_snake_hunter") { }

    class spell_hun_snake_hunter_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_hun_snake_hunter_SpellScript);

        void HandleAfterCast()
        {
            std::cout << "sanke hunter\n";
            Unit *caster = GetCaster();

            caster->GetSpellHistory()->ResetCharges(sSpellMgr->GetSpellInfo(SPELL_HUNTER_MONGOOSE_BITE)->ChargeCategoryId);
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_hun_snake_hunter_SpellScript::HandleAfterCast);
        }

    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_hun_snake_hunter_SpellScript();
    }
};

    // Steady Focus 193553
class spell_hun_steady_focus : public PlayerScript
{

public:

    spell_hun_steady_focus() : PlayerScript("script_nuevesita") {	}

    int32 lastSpell_id = 0;

    void OnSpellCast(Player* player, Spell* spell, bool skipCheck)
    {

        if ((lastSpell_id == spell->GetSpellInfo()->Id && spell->GetSpellInfo()->Id == 185358) || (lastSpell_id == spell->GetSpellInfo()->Id && spell->GetSpellInfo()->Id == SPELL_HUNTER_MULTISHOT))
        {
            if (AuraEffect const* aurEff = player->GetAuraEffectOfRankedSpell(193533, EFFECT_0))
            {

                int32 basepoints = aurEff->GetAmount() + 25;
                player->CastCustomSpell(player, 193534, &basepoints, NULL, NULL, true, NULL, aurEff);
                lastSpell_id = 0;
                return;
            }
        }
        if (spell->GetSpellInfo()->Id != 75 && !spell->IsTriggered())
        {
            lastSpell_id = spell->GetSpellInfo()->Id;
        }

    }

};

    // Steel Trap 162496
class areatrigger_hun_steel_trap : public AreaTriggerEntityScript
{
public:
    areatrigger_hun_steel_trap() : AreaTriggerEntityScript("areatrigger_hun_steel_trap") { }

    struct areatrigger_hun_steel_trapAI : AreaTriggerAI
    {
        areatrigger_hun_steel_trapAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

        // Called when the AreaTrigger has just been initialized, just before added to map

        void OnUnitEnter(Unit* unit) override
        {
            at->SetDuration(0);
            if (Unit* caster = at->GetCaster())
            {
                if (caster->IsValidAttackTarget(unit))
                {
                    if (caster->HasAura(199543)) // Expert trapper
                        caster->CastSpell(unit, 201199, true); // Expert Damage's spell
                    
                    if (caster->HasAura(234955) && at->GetTimeSinceCreated() > 2000)
                    {
                        int32 basepoints = caster->GetTotalAttackPowerValue(BASE_ATTACK)*5;
                        caster->CastCustomSpell(unit, 162487, &basepoints, NULL, NULL, true, NULL);
                    }
                    else
                        caster->CastSpell(unit, 162487, true); //Bleed
                    caster->CastSpell(unit, 162480, true); //root
                    
                    
                   
                }
            }
        }

    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new areatrigger_hun_steel_trapAI(areatrigger);
    }
};

    // Sticky Bomb 191241
class spell_hun_sticky_bomb : public SpellScriptLoader
{
public:
    spell_hun_sticky_bomb() : SpellScriptLoader("spell_hun_sticky_bomb") { }

    class spell_hun_sticky_bomb_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_hun_sticky_bomb_AuraScript);

        void HandleOnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit *target = GetTarget();
            if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
                GetCaster()->CastSpell(GetTarget()->GetPosition().GetPositionX(), GetTarget()->GetPosition().GetPositionY(), GetTarget()->GetPosition().GetPositionZ(), SPELL_HUNTER_STICKY_BOMB_EXPLOSION, true);
        }


        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_hun_sticky_bomb_AuraScript::HandleOnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_hun_sticky_bomb_AuraScript();
    }
};

    // TrailBlazer 199921
class spell_hun_trail_blazer : public SpellScriptLoader
{
    public:
        spell_hun_trail_blazer() : SpellScriptLoader("spell_hun_trail_blazer") { }

        class spell_hun_trail_blazer_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hun_trail_blazer_AuraScript);

            void HandlePeriodic(AuraEffect* aurEff)
            {
                if (GetTarget()->IsInCombat())
                    GetTarget()->RemoveAura(231390);
                
                
            }

            void Register() override
            {
                OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_hun_trail_blazer_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_MOD_SPEED_ALWAYS);
            }

        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_hun_trail_blazer_AuraScript();
        }
};

    // TrailBlazer 199921
class spell_hun_trailbazer : public PlayerScript
{
public:
    spell_hun_trailbazer() : PlayerScript("npc_pet_hunter_spitting_cobra") { }

    struct spell_hun_trailbazerAI : PlayerAI
    {
        spell_hun_trailbazerAI(Player* player) : PlayerAI(player) { }

        void UpdateAI(uint32 diff) override
        {
        }
        
    };

};

void AddSC_spell_hun_steady_focus()
{
    
    new spell_hun_steady_focus();
    new spell_hun_bombardment();
    new spell_hun_flare_trigger();
    new areatrigger_hun_flare();
    new areatrigger_hun_freezing_trap();
    new spell_hun_camouflage();
    new spell_hun_harpoon();
    new spell_hun_mongoose_bite();
    new spell_hun_moknathal_talent();
    new spell_hun_throwing_axes();
    new spell_hun_mortal_wounds();
    new spell_hun_murder_crows();
    new spell_hun_carve();
    new spell_hun_raptor_strike();
    new spell_hun_snake_hunter();
    new areatrigger_hun_caltrops();
    new spell_hun_sticky_bomb();
    new areatrigger_hun_explosive_trap();
    new areatrigger_hun_tar_trap_pool();
    new areatrigger_hun_tar_trap();
    new areatrigger_hun_steel_trap();
    new spell_hun_trail_blazer();
    new spell_hun_trailbazer();
};