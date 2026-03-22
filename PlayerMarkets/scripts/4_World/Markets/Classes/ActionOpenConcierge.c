class ActionOpenConcierge: ActionInteractBase
{
	void ActionOpenConcierge()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}
	
	override string GetText()
	{
		return "Browse Market";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (!target || !player) 
			return false;
		
		PM_ConciergeNPC concierge;
		if (Class.CastTo(concierge, target.GetObject())){
			PMMarketArea area = concierge.GetLinkedArea();
			return (area != NULL);
		}
		
		return false;
	}
	
	override void OnExecuteClient( ActionData action_data ){
		super.OnExecuteClient( action_data );

		if (!action_data)
			return;
		
		PM_ConciergeNPC concierge;
		if (Class.CastTo(concierge, action_data.m_Target.GetObject())){
			m_ConciergeMenu = ConciergeMenu.Cast(g_Game.GetUIManager().EnterScriptedMenu(PLAYER_MARKET_MENU_CONCIERGE, NULL));
			if (m_ConciergeMenu){
				m_ConciergeMenu.SetConcierge(concierge);
			}
		}
	}
}
