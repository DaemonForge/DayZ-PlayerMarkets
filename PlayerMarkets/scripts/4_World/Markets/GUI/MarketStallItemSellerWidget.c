class MarketStallItemSellerWidget  extends ScriptedWidgetEventHandler {
	protected const autoptr TStringArray ITEM_LAYOUT_PATH = {"PlayerMarkets/gui/layout/MarketStallSeller_StallItem.layout","PlayerMarkets/gui/layout/modern/MarketStallSeller_StallItem.layout"};
	protected autoptr PlayerMarketItemDetails m_ItemDetails;
	
	protected autoptr MarketStallSellerMenu m_parent;
	protected Widget m_LayoutRoot;
	
	protected ItemPreviewWidget m_ItemPreview;
	protected TextWidget m_DisplayName;
	protected Widget m_QuanityFrame;
	protected TextWidget m_Quanity;
	protected Widget m_ItemStateFrame;
	protected Widget m_ItemState;
	protected TextWidget m_QuanityAmount;
	protected TextWidget m_QuanityMax;
	protected TextWidget m_Price;
	
	protected ButtonWidget m_Edit;
	protected ButtonWidget m_Delist;
	
	void MarketStallItemSellerWidget(Widget parent, PlayerMarketItemDetails details, MarketStallSellerMenu menu ){
		m_LayoutRoot = Widget.Cast(g_Game.GetWorkspace().CreateWidgets(ITEM_LAYOUT_PATH[GetPMConfig().GUIOption],parent));
		m_parent = MarketStallSellerMenu.Cast(menu);
		m_ItemDetails = PlayerMarketItemDetails.Cast(details);
		
		m_ItemPreview = ItemPreviewWidget.Cast(m_LayoutRoot.FindAnyWidget("ItemPreview"));
		m_DisplayName = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("DisplayName"));
		m_QuanityFrame = Widget.Cast(m_LayoutRoot.FindAnyWidget("QuanityFrame"));
		m_QuanityAmount = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("QuanityAmount"));
		m_Quanity = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Quanity"));
		m_QuanityMax = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("QuanityMax"));
		m_ItemStateFrame = Widget.Cast(m_LayoutRoot.FindAnyWidget("ItemStateFrame"));
		m_ItemState = Widget.Cast(m_LayoutRoot.FindAnyWidget("ItemState"));
		
		m_Price = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Price"));
		
		m_Edit = ButtonWidget.Cast(m_LayoutRoot.FindAnyWidget("Edit"));
		m_Delist = ButtonWidget.Cast(m_LayoutRoot.FindAnyWidget("Delist"));
		
		m_Price.SetText("$" + UUtil.ConvertIntToNiceString(m_ItemDetails.GetPrice()));
		
		//0 = pristine, 1 = worn, 2 = damaged, 3 = badly damaged, 4
		EntityAI item = m_ItemDetails.GetItem();
		m_DisplayName.SetText(item.GetDisplayName());
		PMWidgetHelper.UpdateHealthState(m_ItemStateFrame, m_ItemState, item);
		PMWidgetHelper.UpdateQuantity(m_Quanity, m_QuanityFrame, m_QuanityAmount, m_QuanityMax, item);
		PMWidgetHelper.UpdateItemPreview(m_ItemPreview, item);
		
		m_LayoutRoot.SetHandler(this);
	}
	
	void ~MarketStallItemSellerWidget(){
		m_LayoutRoot.Show(false);
		delete m_LayoutRoot;
	}
	
	override bool OnClick(Widget w, int x, int y, int button){
		
		if (w == m_Edit && m_parent){
			m_parent.EditPrice(m_ItemDetails);
			return true;
		}
		if (w == m_Delist && m_parent){
			m_parent.GetStand().RequestDeList(m_ItemDetails);
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_parent.RequestRefresh,100);
			return true;
		}
		
		return super.OnClick(w,x,y,button);
	
	}
	

	
		
}