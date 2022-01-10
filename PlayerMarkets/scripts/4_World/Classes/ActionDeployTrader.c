#ifdef TRADERPLUS
modded class ActionDeployObject
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (item.IsInherited(PM_MarketKitBase) && !player.IsSZAdmin){
			player.IsSZAdmin = true;
			if (super.ActionCondition(player, target, item)) {
				player.IsSZAdmin = false;
				return true;
			}
			return false;
		}
		return super.ActionCondition(player, target, item);
	}
}
#endif