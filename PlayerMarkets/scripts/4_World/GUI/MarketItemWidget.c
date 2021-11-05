class MarketStallItemWidget  extends ScriptedWidgetEventHandler {
	protected const string ITEM_LAYOUT_PATH = "PlayerMarkets/gui/layout/MarketItem.layout";
	protected autoptr PlayerMarketSoloItemDetails m_ItemDetails;
	protected autoptr PlayerMarketBulkItemDetails m_BulkItemDetails;
	
	protected Widget m_LayoutRoot;
	
	void MarketStallItemWidget(Widget parent, PlayerMarketItemDetails details){
		m_LayoutRoot = Widget.Cast(GetGame().GetWorkspace().CreateWidgets(ITEM_LAYOUT_PATH,parent));
	}
}