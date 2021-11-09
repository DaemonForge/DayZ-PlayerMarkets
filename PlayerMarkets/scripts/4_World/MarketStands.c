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
	
	override bool HasProperDistance( string selection, PlayerBase player )
	{
		if ( MemoryPointExists( selection ) )
		{
			vector selection_pos = ModelToWorld( GetMemoryPointPos( selection ) );
			float distance = vector.Distance( selection_pos, player.GetPosition() );
			if ( distance >= 0.1 )
			{
				return true;
			}
		}
			
		return true;
	}
}