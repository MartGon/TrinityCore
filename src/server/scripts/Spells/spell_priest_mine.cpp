//Una script chupiguay
#include "PlayerAI.h"
#include "AreaTriggerTemplate.h"
#include "AreaTriggerAI.h"


enum PriestSpells
{
	SPELL_DIVINE_STAR_CAST= 110744,
	SPELL_DIVINE_STAR_HEAL= 110745,
	SPELL_DIVINE_STAR_DMG= 122128
};

class areatrigger_divine_start : public AreaTriggerEntityScript
{
public:
	areatrigger_divine_start() : AreaTriggerEntityScript("areatrigger_priest_divine_star") { }

	struct areatrigger_divine_startAI : AreaTriggerAI
	{
		areatrigger_divine_startAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) { }

		// Called when the AreaTrigger has just been initialized, just before added to map
		void OnCreate()
		{ 
			
            if (Unit* caster = at->GetCaster())
            {
                Position t_pos;
                Position c_pos = at->GetCaster()->GetPosition();

                std::vector<G3D::Vector3> SplinePoints;
                G3D::Vector3 vector;
                G3D::Vector3 vector2;
                for (int i = 0; i < 40; i++)
                {
                    vector.x = c_pos.GetPositionX()+i;
                    vector.y = c_pos.GetPositionY()+i;
                    vector.z = c_pos.GetPositionZ();
                    SplinePoints.push_back(vector);
                }

                /*vector.x = c_pos.GetPositionX();
                vector.y = c_pos.GetPositionY();
                vector.z = c_pos.GetPositionZ();

                vector2.x = c_pos.GetPositionX() + 5;
                vector2.y = c_pos.GetPositionY() + 5;
                vector2.z = c_pos.GetPositionZ();

                SplinePoints.push_back(vector);
                SplinePoints.push_back(vector2);*/

               // at->InitSplineOffsets(SplinePoints, 1364);
                

                TempSummon* creature = at->SummonCreature(16221, c_pos, TEMPSUMMON_MANUAL_DESPAWN, 0, 0);
                creature->MonsterMoveWithSpeed(c_pos.GetPositionX() + 5, c_pos.GetPositionY() + 5, c_pos.GetPositionZ(), 4, false, false);


            }
		}

		void OnUpdate(uint32 /*diff*/) 
		{ 

			//at->Relocate(t_pos);
			//std::cout << "Realocated a las siguientes coordenadas x:"<<t_pos.GetPositionX()<<" y:"<<t_pos.GetPositionY()<<" z:"<<t_pos.GetPositionZ()<<"\n";
            
		
		
		}

        void OnDestinationReached() 
        { 
            std::cout << "\n\nLlegue a mi destino\n\n\n";
            Position t_pos = at->GetPosition();
            std::vector<G3D::Vector3> SplinePoints;
            G3D::Vector3 vector;

            vector.x = t_pos.GetPositionX();
            vector.y = t_pos.GetPositionY();
            vector.z = t_pos.GetPositionZ();
            SplinePoints.push_back(vector);

            vector.x = at->GetCaster()->GetPosition().GetPositionX();
            vector.y = at->GetCaster()->GetPosition().GetPositionY();
            vector.z = at->GetCaster()->GetPosition().GetPositionZ();
            SplinePoints.push_back(vector);

            at->SetDuration(at->GetTotalDuration());
            at->InitSplineOffsets(SplinePoints, 1000);
        }

        void OnRemove() 
        { 
            std::cout << "\n\nDesapareci\n\n\n";
        }

		void OnUnitEnter(Unit* unit) override
		{
			if (Unit* caster = at->GetCaster())
			{
				if (caster->IsValidAttackTarget(unit))
				{
					caster->CastSpell(unit, SPELL_DIVINE_STAR_DMG, true);
				}
				else if(caster->IsFriendlyTo(unit))
				{
					caster->CastSpell(unit, SPELL_DIVINE_STAR_HEAL, true);
				}
			}
		}
	};

	AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
	{
		return new areatrigger_divine_startAI(areatrigger);
	}
};



void AddSC_spell_priest_mine()
{
	
	new areatrigger_divine_start();


};