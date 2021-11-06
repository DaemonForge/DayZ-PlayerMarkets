modded class ActionConstructor 
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);

		actions.Insert(ActionDestroyPM_MarketPart);
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