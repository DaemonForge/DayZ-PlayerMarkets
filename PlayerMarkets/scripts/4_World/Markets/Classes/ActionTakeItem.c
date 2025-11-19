modded class ActionTakeItem: ActionInteractBase
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (super.ActionCondition(player,target,item)){
			MarketStandBase stall;
			if ( Class.CastTo(stall, target.GetParent()) ){
				return false;
			}
			ItemBase parent;
			if ( Class.CastTo(parent, target.GetParent()) && Class.CastTo(stall, parent.GetHierarchyRoot()) ){
				return false;
			}
			return true;
		}
		return false;
	}
}