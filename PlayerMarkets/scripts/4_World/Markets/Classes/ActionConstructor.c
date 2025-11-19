modded class ActionConstructor 
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);

		actions.Insert(ActionDestroyPM_MarketPart);
		actions.Insert( ActionOpenMarketStallBuy );
		actions.Insert( ActionOpenMarketStallSell );
	}
}

modded class SledgeHammer
{
	/*override bool IsMeleeFinisher()
	{
		return true;
	}*/
	
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionDestroyPM_MarketPart);
	}
}
