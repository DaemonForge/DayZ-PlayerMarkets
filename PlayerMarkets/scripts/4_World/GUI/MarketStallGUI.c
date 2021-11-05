class MarketStallMenu extends UIScriptedMenu {
	
	protected const string ROOT_LAYOUT_PATH = "PlayerMarkets/gui/layout/MarketStall.layout";
	protected TextWidget m_ShopTitle;
	protected GridSpacerWidget m_ItemGrid;
	
	protected autoptr array<autoptr MarketStallItemWidget> m_ItemWidgets;
	
	protected PM_MarketStand m_Stand;
	
	override Widget Init()
    {
		layoutRoot 		= Widget.Cast(GetGame().GetWorkspace().CreateWidgets(ROOT_LAYOUT_PATH));
		
		m_ShopTitle 	= TextWidget.Cast(layoutRoot.FindAnyWidget("ShopTitle"));
		m_ItemGrid 		= GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("ItemGrid"));
		
		return layoutRoot;
	}
	
	void SetStall(PM_MarketStand stand){
		Class.CastTo(m_Stand, stand);
		m_ShopTitle.SetText(m_Stand.GetStandName());
		if (!m_ItemWidgets){m_ItemWidgets = new array<autoptr MarketStallItemWidget>;}
		autoptr array<autoptr PlayerMarketItemDetails> itemsArray = m_Stand.GetItemsArray();
		if (itemsArray && itemsArray.Count() > 0){
			for (int i = 0; i < itemsArray.Count(); i++){
				m_ItemWidgets.Insert(new MarketStallItemWidget(m_ItemGrid, itemsArray.Get(i),this));
			}
		
		}
	}
	
	
	override void Update( float timeslice )
	{
		super.Update( timeslice );
		if ( GetGame().GetInput().LocalPress( "UAUIBack", false ) ) {
			GetGame().GetUIManager().CloseMenu(PLAYER_MARKET_MENU_BUY);
		}		
	}
	
}