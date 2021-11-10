class PM_MarketStand extends MarketStandBase 
{
	override string GetConstructionKitType()
	{
		return "PM_MarketKit";
	}
	
}
class PM_MarketStandV2 extends MarketStandBase
{
	override string GetConstructionKitType()
	{
		return "PM_MarketV2Kit";
	}
}
class PM_TraderTable extends MarketStandBase
{
	override string GetConstructionKitType()
	{
		return "PM_MarketTableKit";
	}
	
	override bool IsPlayerInside( PlayerBase player, string selection )
	{

		if ( !HasProperDistance( "center", player ) && !HasProperDistance( "center2", player ) && !HasProperDistance( "center3", player ) )
		{
			return true;
		}
		
		return true;
	}
	override bool HasProperDistance( string selection, PlayerBase player )
	{
		if ( MemoryPointExists( selection ) )
		{
			vector selection_pos = ModelToWorld( GetMemoryPointPos( selection ) );
			float distance = vector.Distance( selection_pos, player.GetPosition() );
			if ( distance >= 0.4 )
			{
				return true;
			}
		}
			
		return true;
	}
}