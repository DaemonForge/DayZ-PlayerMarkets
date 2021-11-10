class PM_MarketKit extends PM_MarketKitBase
{
	
	override string GetMarketStandClass(){
		return "PM_MarketStand";
	}
};
class PM_MarketV2Kit extends PM_MarketKitBase
{	
	override string GetMarketStandClass(){
		return "PM_MarketStandV2";
	}
};
class PM_MarketTableKit extends PM_MarketKitBase
{
	override string GetMarketStandClass()
	{
		return "PM_TraderTable";
	}
}

class PM_MarketKitBase extends ItemBase
{
	
	string GetMarketStandClass(){
		return "PM_MarketStand";
	}
	
	//================================================================
	// ADVANCED PLACEMENT
	//================================================================	
	
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0"  )
	{
		super.OnPlacementComplete( player, position, orientation );
		
		if ( GetGame().IsServer() )
		{
			PlayerBase player_base = PlayerBase.Cast( player );

			ItemBase MarketStall = ItemBase.Cast( GetGame().CreateObjectEx(GetMarketStandClass(), GetPosition(), ECE_PLACE_ON_SURFACE ) );
			
			MarketStall.SetPosition( position);
			MarketStall.SetOrientation( orientation );
			
			//make the kit invisible, so it can be destroyed from deploy UA when action ends
			HideAllSelections();
			
			this.Delete();
			SetIsDeploySound( true );
		}	
	}
	
	override string GetPlaceSoundset()
	{
		return "seachest_drop_SoundSet";
	}
	
	override bool IsDeployable()
	{
		return true;
	}
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionDeployObject);
	}
};
