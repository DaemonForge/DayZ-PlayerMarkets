class ActionDestroyPM_MarketPartCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousRepeat( UATimeSpent.DEFAULT_DESTROY/ActionDestroyPart.Cast(m_ActionData.m_Action).CYCLES );
	}
};

class ActionDestroyPM_MarketPart: ActionContinuousBase
{
	static int CYCLES = 4;
	
	void ActionDestroyPM_MarketPart()
	{
		m_CallbackClass = ActionDestroyPM_MarketPartCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DISASSEMBLE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;		
		
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.BASEBUILDING );
	}
		
	override string GetText()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( player )
		{
			ConstructionActionData construction_action_data = player.GetConstructionActionData();
			ConstructionPart constrution_part = construction_action_data.GetTargetPart();
			
			if ( constrution_part )
			{
				return "#destroy" + " " + constrution_part.GetName();
			}
		}
		
		return "";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
		MarketStandBase base_building = MarketStandBase.Cast( target_object );
		if ( base_building )
		{
			if (player.m_BrokenLegState == eBrokenLegs.BROKEN_LEGS)
				return false;
			
			return DestroyCondition( player, target, item, true );

		}
        
		return false;
	}
	
	override bool ActionConditionContinue( ActionData action_data )
	{	
		return DestroyCondition( action_data.m_Player, action_data.m_Target, action_data.m_MainItem , false );
	}	
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		MarketStandBase base_building = MarketStandBase.Cast( action_data.m_Target.GetObject() );
		Construction construction = base_building.GetConstruction();
		ConstructionActionData construction_action_data = action_data.m_Player.GetConstructionActionData();
		ConstructionPart construction_part = construction_action_data.GetTargetPart();

		
		if ( construction.CanDestroyPart( construction_part.GetPartName() ) )
		{
			//build
			construction.DestroyPartServer( action_data.m_Player, construction_part.GetPartName(), AT_DESTROY_PART );
			
			//add damage to tool
			action_data.m_MainItem.DecreaseHealth( UADamageApplied.DESTROY, false );
		}

		action_data.m_Player.GetSoftSkillsManager().AddSpecialty( m_SpecialtyWeight );
	}
	
	protected bool DestroyCondition( PlayerBase player, ActionTarget target, ItemBase item, bool camera_check )
	{	
		if ( player && !player.IsLeaning() )
		{
			Object target_object = target.GetObject();
			if ( target_object && target_object.CanUseConstruction() )
			{
				string part_name = target_object.GetActionComponentName( target.GetComponentIndex() );
				BaseBuildingBase base_building = BaseBuildingBase.Cast( target_object );
				Construction construction = base_building.GetConstruction();		
				ConstructionPart construction_part = construction.GetConstructionPartToDestroy( part_name );
				MarketStandBase NDBase = MarketStandBase.Cast( target.GetObject() );
				if (!NDBase.IsOwner(player)){
					return false;
				}
				if ( construction_part )
				{
					//camera and position checks
					if ( base_building.IsFacingPlayer( player, part_name ) && !player.GetInputController().CameraIsFreeLook() )
					{
						//Camera check (client-only)
						if ( camera_check )
						{
							if ( GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
							{
								if ( !base_building.IsFacingCamera( part_name ) )
								{
									return false;
								}
							}
						}
						ConstructionActionData construction_action_data = player.GetConstructionActionData();
						construction_action_data.SetTargetPart( construction_part );
						return true;
					}
				}
				
			
				
				//this if statement was added
				else if ( base_building)
				{
					if ( Class.CastTo(base_building, target.GetObject()) )
					{
						if ( construction_part )
						{
							if ( NDBase && NDBase.HasProperDistanceDestory( "outside", player ) )
							{
								ConstructionActionData construction_action_data_flat = player.GetConstructionActionData();
								construction_action_data_flat.SetTargetPart( construction_part );
								return true;
							}
							return false;
						}
					}
				}
			}
		}
		
		return false;
	}
	
	override string GetAdminLogMessage(ActionData action_data)
	{
		return " destroyed " + action_data.m_Target.GetObject().GetDisplayName() + " with " + action_data.m_MainItem.GetDisplayName();
	}
}