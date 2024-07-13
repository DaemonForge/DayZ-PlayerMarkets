#ifdef TRADERPLUS
modded class ActionDeployObject
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (item && item.IsInherited(PM_MarketKitBase) && player.GetSafeZoneStatus() == SZ_IN_SAFEZONE){
			//Action not allowed if player has broken legs
			if (player.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
				return false;
			
			//Client
			if ( !GetGame().IsDedicatedServer() )
			{
				if ( player.IsPlacingLocal() )
				{
					if ( !player.GetHologramLocal().IsColliding() )
					{
						if ( item.CanBePlaced(player, player.GetHologramLocal().GetProjectionEntity().GetPosition()) )
						{
							return true;
						}
					}
				}
				return false;
			}
			//Server
			return true;
		}
		return super.ActionCondition(player, target, item);
	}
}
#endif