/*
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "npc_pet_hun_".
 */

#include "ScriptMgr.h"
#include "CellImpl.h"
#include "CombatAI.h"
#include "GridNotifiersImpl.h"
#include "MotionMaster.h"
#include "ScriptedCreature.h"
#include "SpellInfo.h"

enum HunterSpells
{
    SPELL_HUNTER_CRIPPLING_POISON       = 30981, // Viper
    SPELL_HUNTER_DEADLY_POISON_PASSIVE  = 34657, // Venomous Snake
    SPELL_HUNTER_MIND_NUMBING_POISON    = 25810,  // Viper
    SPELL_HUNTER_COBRA_SPIT             = 206685, // Cobra Spit
    SPELL_HUNTER_COBRA_AURA             = 194407 // Focus regn Aura
};

enum HunterCreatures
{
    NPC_HUNTER_VIPER                    = 19921
};

class npc_pet_hunter_snake_trap : public CreatureScript
{
    public:
        npc_pet_hunter_snake_trap() : CreatureScript("npc_pet_hunter_snake_trap") { }

        struct npc_pet_hunter_snake_trapAI : public ScriptedAI
        {
            npc_pet_hunter_snake_trapAI(Creature* creature) : ScriptedAI(creature)
            {
                Initialize();
            }

            void Initialize()
            {
                _spellTimer = 0;
                _isViper = false;
            }

            void EnterCombat(Unit* /*who*/) override { }

            void Reset() override
            {
                Initialize();

                CreatureTemplate const* Info = me->GetCreatureTemplate();

                _isViper = Info->Entry == NPC_HUNTER_VIPER ? true : false;

                me->SetMaxHealth(uint32(107 * (me->getLevel() - 40) * 0.025f));
                // Add delta to make them not all hit the same time
                uint32 delta = (rand32() % 7) * 100;
                me->SetBaseAttackTime(BASE_ATTACK, Info->BaseAttackTime + delta);
                //me->SetStatFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER, float(Info->attackpower));

                // Start attacking attacker of owner on first ai update after spawn - move in line of sight may choose better target
                if (!me->GetVictim() && me->IsSummon())
                    if (Unit* Owner = me->ToTempSummon()->GetSummoner())
                        if (Owner->getAttackerForHelper())
                            AttackStart(Owner->getAttackerForHelper());

                if (!_isViper)
                    DoCast(me, SPELL_HUNTER_DEADLY_POISON_PASSIVE, true);
            }

            // Redefined for random target selection:
            void MoveInLineOfSight(Unit* who) override
            {
                if (!me->GetVictim() && me->CanCreatureAttack(who))
                {
                    if (me->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                        return;

                    float attackRadius = me->GetAttackDistance(who);
                    if (me->IsWithinDistInMap(who, attackRadius) && me->IsWithinLOSInMap(who))
                    {
                        if (!(rand32() % 5))
                        {
                            me->setAttackTimer(BASE_ATTACK, (rand32() % 10) * 100);
                            _spellTimer = (rand32() % 10) * 100;
                            AttackStart(who);
                        }
                    }
                }
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim() || !me->GetVictim())
                    return;

                if (me->EnsureVictim()->HasBreakableByDamageCrowdControlAura(me))
                {
                    me->InterruptNonMeleeSpells(false);
                    return;
                }

                // Viper
                if (_isViper)
                {
                    if (_spellTimer <= diff)
                    {
                        if (urand(0, 2) == 0) // 33% chance to cast
                            DoCastVictim(RAND(SPELL_HUNTER_MIND_NUMBING_POISON, SPELL_HUNTER_CRIPPLING_POISON));

                        _spellTimer = 3000;
                    }
                    else
                        _spellTimer -= diff;
                }

                DoMeleeAttackIfReady();
            }

        private:
            bool _isViper;
            uint32 _spellTimer;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_pet_hunter_snake_trapAI(creature);
        }
};

class npc_pet_hunter_spitting_cobra : public CreatureScript
{
    public:
        npc_pet_hunter_spitting_cobra() : CreatureScript("npc_pet_hunter_spitting_cobra") { }

        struct npc_pet_hunter_spitting_cobraAI : CasterAI
        {
            npc_pet_hunter_spitting_cobraAI(Creature* creature) : CasterAI(creature) { }

            void InitializeAI() override
            {
                CasterAI::InitializeAI();
            
                if(Unit* Owner = me->GetOwner())
                    if(Owner->GetVictim())
                        AttackStart(Owner->GetVictim());

            }

            void JustDied(Unit* /*killer*/) override
            {
                // Stop Feeding Gargoyle when it dies
                if (Unit* owner = me->GetOwner())
                    owner->RemoveAurasDueToSpell(SPELL_HUNTER_COBRA_AURA);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim() || !me->GetVictim()) 
                {
                    if (Unit* Owner = me->GetOwner())
                        if (Owner->GetVictim())
                            AttackStart(Owner->GetVictim());
                        else
                            return;
                }
                

                if (me->EnsureVictim()->HasBreakableByDamageCrowdControlAura(me))
                {
                    me->InterruptNonMeleeSpells(false);
                    return;
                }

                if (_spellTimer <= diff)
                {
                        DoCastVictim(SPELL_HUNTER_COBRA_SPIT);

                    _spellTimer = 2000;
                }
                else
                    _spellTimer -= diff;
            
                DoMeleeAttackIfReady();
            }

        private:
            bool _isViper;
            uint32 _spellTimer;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_pet_hunter_spitting_cobraAI(creature);
        }
};

class spell_hun_cobra_spit : public SpellScriptLoader
{
    public:
        spell_hun_cobra_spit() : SpellScriptLoader("spell_hun_cobra_spit") { }

        class spell_hun_cobra_spit_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_hun_cobra_spit_SpellScript);

            void CalculateDamage(SpellEffIndex /*effIndex*/)
            {
                if (Unit* Owner = GetCaster()->GetOwner())
                    SetHitDamage(Owner->GetTotalAttackPowerValue(BASE_ATTACK) *1.1);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_hun_cobra_spit_SpellScript::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }

        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_hun_cobra_spit_SpellScript();
        }
};

void AddSC_hunter_pet_scripts()
{
    new npc_pet_hunter_snake_trap();
    new npc_pet_hunter_spitting_cobra();
    new spell_hun_cobra_spit();
}
