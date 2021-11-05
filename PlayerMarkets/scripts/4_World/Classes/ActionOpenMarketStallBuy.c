modded class ActionConstructor
{
	override void RegisterActions( TTypenameArray actions )
	{
		super.RegisterActions( actions );	
		actions.Insert( ActionOpenMarketStallBuy );
		actions.Insert( ActionOpenMarketStallSell );

	}
};

class ActionOpenMarketStallBuy: ActionInteractBase
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
		return "Buy";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !target || !player) 
			return false;
		
		
		return false;
	}
	
	override void OnExecuteServer( ActionData action_data ){
		super.OnExecuteServer( action_data );
		
	}
	
	override void OnExecuteClient( ActionData action_data ){
		super.OnExecuteClient( action_data );

		if ( !action_data )
			return;
		
	}
	
}

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
		return "Buy";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !target || !player) 
			return false;
		
		
		return false;
	}
	
	override void OnExecuteServer( ActionData action_data ){
		super.OnExecuteServer( action_data );
		
	}
	
	override void OnExecuteClient( ActionData action_data ){
		super.OnExecuteClient( action_data );

		if ( !action_data )
			return;
		
	}
	
}