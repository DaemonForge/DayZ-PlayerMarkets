class MarketStallItemWidget  extends ScriptedWidgetEventHandler {
	protected const autoptr TStringArray ITEM_LAYOUT_PATH = {"PlayerMarkets/gui/layout/MarketItem.layout","PlayerMarkets/gui/layout/Modern/MarketItem.layout"};
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
		m_LayoutRoot = Widget.Cast(GetGame().GetWorkspace().CreateWidgets(ITEM_LAYOUT_PATH[GetPMConfig().GUIOption],parent));
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
		
		
		
		m_Cost.SetText("$" + UUtil.ConvertIntToNiceString(m_ItemDetails.GetPrice()));
		
		//0 = pristine, 1 = worn, 2 = damaged, 3 = badly damaged, 4
		EntityAI item = m_ItemDetails.GetItem();
		m_ItemName.SetText(item.GetDisplayName());
		int healthLevel = item.GetHealthLevel("");
		switch  (healthLevel) {
			case GameConstants.STATE_RUINED:
				m_ItemStateFrame.Show(true);
				m_ItemState.SetColor(Colors.COLOR_RUINED);
				m_ItemState.SetAlpha(1);
			break;
			case GameConstants.STATE_BADLY_DAMAGED:
				m_ItemStateFrame.Show(true);
				m_ItemState.SetColor(Colors.COLOR_BADLY_DAMAGED);
				m_ItemState.SetAlpha(1);
			break;
			case GameConstants.STATE_DAMAGED:
				m_ItemStateFrame.Show(true);
				m_ItemState.SetColor(Colors.COLOR_DAMAGED);
				m_ItemState.SetAlpha(1);
			break;
			case GameConstants.STATE_WORN:
				m_ItemStateFrame.Show(true);
				m_ItemState.SetColor(Colors.COLOR_WORN);
				m_ItemState.SetAlpha(1);
			break;
			case GameConstants.STATE_PRISTINE:
				m_ItemStateFrame.Show(true);
				m_ItemState.SetColor(Colors.COLOR_PRISTINE);
				m_ItemState.SetAlpha(1);
			break;
			default:
				m_ItemStateFrame.Show(false);
			break;
		}
		m_Quanity.Show(false);
		UpdateQuanity(item);
		UpdateItemPreviw(item);
		
		
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
	
	bool OnMouseEnter(Widget w, int x, int y){
		if (w == m_LayoutRoot){
			m_BG.SetSize(1.05,1.05);
		}
		return super.OnMouseEnter(w,x,y);
	}
	bool OnMouseLeave(Widget w, Widget enterW, int x, int y){
		if (w == m_LayoutRoot){
			m_BG.SetSize(1,1);
			
		}
		return super.OnMouseLeave(w,enterW,x,y);
	}
	bool OnMouseButtonUp(Widget w, int x, int y, int button){
		if (w == m_LayoutRoot && m_parent){
			m_parent.OpenViewItem(m_ItemDetails);
			return true;
		}
		return super.OnMouseButtonUp(w,x,y, button);
	}
	
	
	void UpdateItemPreviw(EntityAI item){
		InventoryItem iItem = InventoryItem.Cast(item);
		if (iItem){
			m_ItemPreview.SetItem( iItem );
			m_ItemPreview.SetModelPosition( Vector(0,0,0) );
			m_ItemPreview.SetModelOrientation( Vector(0,0,0) );
			m_ItemPreview.SetPos( 0, 0);
			m_ItemPreview.SetSize( 1, 1);
			m_ItemPreview.SetView( 0 );
		}
	}
	
	
	
	void UpdateQuanity(EntityAI item){
		Magazine mag;
		if (Class.CastTo(mag, item)){
			m_Quanity.Show(true);
			m_Quanity.SetText( mag.GetAmmoCount().ToString() + "/" + mag.GetAmmoMax().ToString() );
			return;
		}
		
		ItemBase itemB;
		if (!Class.CastTo(itemB, item) || !itemB.HasQuantity()){
			return;
		}
		string text = itemB.GetQuantity().ToString();
		string units = itemB.ConfigGetString("stackedUnit");
		if ((units == "percent" || units == "w") && itemB.GetQuantityMax() > 0){
			float number = itemB.GetQuantity() / itemB.GetQuantityMax();
			int num = number * 100;
			text = num.ToString() + "%";
			m_Quanity.Show(true);
			m_Quanity.SetText( text );
		} else if (units != ""){
			m_QuanityFrame.Show(true);
			m_QuanityAmount.SetText(itemB.GetQuantity().ToString() + units);
			text = itemB.GetQuantityMax().ToString() + units;
			m_QuanityMax.SetText(text);
		}
	}
}