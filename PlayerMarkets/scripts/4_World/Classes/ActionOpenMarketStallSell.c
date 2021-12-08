class ActionOpenMarketStallSell: ActionInteractBase
{
	void ActionMapLinkOpenOnAny()
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
		return "Sell";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !target || !player) 
			return false;
		
		MarketStandBase stall;
		if ((Class.CastTo(stall, target.GetObject()) || Class.CastTo(stall, target.GetParent())) && stall.CanOpenSellMenu(player)){
			return true;
		}
		return false;
	}
	
	override void OnExecuteServer( ActionData action_data ){
		super.OnExecuteServer( action_data );
		
	}
	
	override void OnExecuteClient( ActionData action_data ){
		super.OnExecuteClient( action_data );

		if ( !action_data )
			return;
		
		MarketStandBase stall;
		PlayerBase thePlayer;
		if ((Class.CastTo(stall, action_data.m_Target.GetObject()) || Class.CastTo(stall, action_data.m_Target.GetParent())) && Class.CastTo(thePlayer, action_data.m_Player)){
			
			if (stall.CanOpenSellMenu(thePlayer)){
				m_MarketStallSellerMenu = MarketStallSellerMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(PLAYER_MARKET_MENU_SELL, NULL));
				m_MarketStallSellerMenu.SetStall(stall);
			}
		}
	}
	
}