class PM_MarketStand extends MarketStandBase 
{
	
	
	protected void InitMarketData(){
		super.InitMarketData();
	}
	
	int GetMaxItemsForSale(){
		return GetPMConfig().MarketStallLv1SaleSlots;
	}
	
	
	override string GetConstructionKitType()
	{
		return "PM_MarketKit";
	}
	
}
class PM_MarketStandV2 extends MarketStandBase
{
	int GetMaxItemsForSale(){
		return GetPMConfig().MarketStallLv2SaleSlots;
	}
	
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
			if ( distance >= 1.0 )
			{
				return false;
			}
		}
			
		return true;
	}
}
class PM_TraderTable extends MarketStandBase
{
	int GetMaxItemsForSale(){
		return GetPMConfig().MarketTableSaleSlots;
	}
	
	protected void InitMarketData(){
		super.InitMarketData();
	}
	
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