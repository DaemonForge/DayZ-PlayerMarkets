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