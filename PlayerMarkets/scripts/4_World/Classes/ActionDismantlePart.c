modded class ActionDismantlePart: ActionContinuousBase
{

	
	protected bool DismantleCondition( PlayerBase player, ActionTarget target, ItemBase item, bool camera_check )
	{	
		MarketStandBase stall;
		if ( Class.CastTo(stall, target.GetObject()) || Class.CastTo(stall, target.GetParent()) ) {
			if (!stall.IsOwner(player) || (stall.GetItemsArray() && stall.GetItemsArray().Count() > 0)){
				return false;
			}
		} 
		
		return super.DismantleCondition(player,target,item,camera_check);
	}
	
	
}