class MarketStallItemWidget  extends ScriptedWidgetEventHandler {
	protected const string ITEM_LAYOUT_PATH = "PlayerMarkets/gui/layout/MarketItem.layout";
	protected autoptr PlayerMarketItemDetails m_ItemDetails;
	
	protected autoptr MarketStallMenu m_parent;
	protected Widget m_LayoutRoot;
	
	void MarketStallItemWidget(Widget parent, PlayerMarketItemDetails details, MarketStallMenu menu ){
		m_LayoutRoot = Widget.Cast(GetGame().GetWorkspace().CreateWidgets(ITEM_LAYOUT_PATH,parent));
		m_parent = MarketStallMenu.Cast(menu);
		m_ItemDetails = PlayerMarketItemDetails.Cast(details);
	}
}