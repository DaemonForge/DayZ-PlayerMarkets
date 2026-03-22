class MarketStallItemWidget  extends ScriptedWidgetEventHandler {
	protected const autoptr TStringArray ITEM_LAYOUT_PATH = {"PlayerMarkets/gui/layout/MarketItem.layout","PlayerMarkets/gui/layout/modern/MarketItem.layout"};
	protected autoptr PlayerMarketItemDetails m_ItemDetails;
	
	protected autoptr MarketStallMenu m_parent;
	protected Widget m_LayoutRoot;
	
	protected ItemPreviewWidget m_ItemPreview;
	protected TextWidget m_ItemName;
	protected TextWidget m_Cost;
	protected Widget m_QuanityFrame;
	protected TextWidget m_QuanityAmount;
	protected TextWidget m_QuanityMax;
	protected TextWidget m_Quanity;
	protected Widget m_ItemStateFrame;
	protected Widget m_ItemState;
	
	protected ImageWidget m_BG;
	
	
	void MarketStallItemWidget(Widget parent, PlayerMarketItemDetails details, MarketStallMenu menu ){
		m_LayoutRoot = Widget.Cast(g_Game.GetWorkspace().CreateWidgets(ITEM_LAYOUT_PATH[GetPMConfig().GUIOption],parent));
		m_parent = MarketStallMenu.Cast(menu);
		m_ItemDetails = PlayerMarketItemDetails.Cast(details);
		m_BG = ImageWidget.Cast(m_LayoutRoot.FindAnyWidget("BG"));
		m_ItemPreview = ItemPreviewWidget.Cast(m_LayoutRoot.FindAnyWidget("ItemPreview"));
		m_ItemName = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("ItemName"));
		m_Cost = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Cost"));
		m_QuanityFrame = Widget.Cast(m_LayoutRoot.FindAnyWidget("QuanityFrame"));
		m_QuanityAmount = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("QuanityAmount"));
		m_Quanity = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Quanity"));
		m_QuanityMax = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("QuanityMax"));
		m_ItemStateFrame = Widget.Cast(m_LayoutRoot.FindAnyWidget("ItemStateFrame"));
		m_ItemState = Widget.Cast(m_LayoutRoot.FindAnyWidget("ItemState"));
		
		
		int price = details.GetPrice();
		float effectiveTax = 0;
		if (m_parent && m_parent.GetStand()){
			effectiveTax = m_parent.GetStand().GetEffectiveSaleTax();
		} else {
			effectiveTax = GetPMConfig().SaleTaxAmount;
		}
		if (effectiveTax > 0){
			price+= price * effectiveTax;
		}
		
		m_Cost.SetText("$" + UUtil.ConvertIntToNiceString(price));
		
		//0 = pristine, 1 = worn, 2 = damaged, 3 = badly damaged, 4
		EntityAI item = m_ItemDetails.GetItem();
		m_ItemName.SetText(item.GetDisplayName());
		PMWidgetHelper.UpdateHealthState(m_ItemStateFrame, m_ItemState, item);
		PMWidgetHelper.UpdateQuantity(m_Quanity, m_QuanityFrame, m_QuanityAmount, m_QuanityMax, item);
		PMWidgetHelper.UpdateItemPreview(m_ItemPreview, item);
		
		
		m_LayoutRoot.SetHandler(this);
	}
	
	
	void ~MarketStallItemWidget(){
		m_LayoutRoot.Show(false);
		delete m_LayoutRoot;
	}
	
	override bool OnClick(Widget w, int x, int y, int button){
		if (w == m_LayoutRoot && m_parent){
			m_parent.OpenViewItem(m_ItemDetails);
			return true;
		}
		
		return super.OnClick(w,x,y,button);
	
	}
	
	override bool OnMouseEnter(Widget w, int x, int y){
		if (w == m_LayoutRoot){
			m_BG.SetSize(1.05,1.05);
		}
		return super.OnMouseEnter(w,x,y);
	}
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y){
		if (w == m_LayoutRoot){
			m_BG.SetSize(1,1);
			
		}
		return super.OnMouseLeave(w,enterW,x,y);
	}
	override bool OnMouseButtonUp(Widget w, int x, int y, int button){
		if (w == m_LayoutRoot && m_parent){
			m_parent.OpenViewItem(m_ItemDetails);
			return true;
		}
		return super.OnMouseButtonUp(w,x,y, button);
	}
	
	
}