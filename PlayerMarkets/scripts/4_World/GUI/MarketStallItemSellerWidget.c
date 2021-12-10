class MarketStallItemSellerWidget  extends ScriptedWidgetEventHandler {
	protected const string ITEM_LAYOUT_PATH = "PlayerMarkets/gui/layout/MarketStallSeller_StallItem.layout";
	protected autoptr PlayerMarketItemDetails m_ItemDetails;
	
	protected autoptr MarketStallSellerMenu m_parent;
	protected Widget m_LayoutRoot;
	
	protected ItemPreviewWidget m_ItemPreview;
	protected TextWidget m_DisplayName;
	protected Widget m_QuanityFrame;
	protected TextWidget m_Quanity;
	protected TextWidget m_State;
	protected TextWidget m_QuanityAmount;
	protected TextWidget m_QuanityMax;
	
	protected ButtonWidget m_Edit;
	protected ButtonWidget m_Delist;
	
	void MarketStallItemSellerWidget(Widget parent, PlayerMarketItemDetails details, MarketStallSellerMenu menu ){
		m_LayoutRoot = Widget.Cast(GetGame().GetWorkspace().CreateWidgets(ITEM_LAYOUT_PATH,parent));
		m_parent = MarketStallSellerMenu.Cast(menu);
		m_ItemDetails = PlayerMarketItemDetails.Cast(details);
		
		m_ItemPreview = ItemPreviewWidget.Cast(m_LayoutRoot.FindAnyWidget("ItemPreview"));
		m_DisplayName = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("DisplayName"));
		m_QuanityFrame = Widget.Cast(m_LayoutRoot.FindAnyWidget("QuanityFrame"));
		m_QuanityAmount = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("QuanityAmount"));
		m_Quanity = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Quanity"));
		m_QuanityMax = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("QuanityMax"));
		m_State = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("State"));
		
		m_Edit = ButtonWidget.Cast(m_LayoutRoot.FindAnyWidget("Edit"));
		m_Delist = ButtonWidget.Cast(m_LayoutRoot.FindAnyWidget("Delist"));
		
		
		//0 = pristine, 1 = worn, 2 = damaged, 3 = badly damaged, 4
		EntityAI item = m_ItemDetails.GetItem();
		m_DisplayName.SetText(item.GetDisplayName());
		int healthLevel = item.GetHealthLevel("");
		switch  (healthLevel) {
			case GameConstants.STATE_RUINED:
				m_State.SetText("RUINED");
				m_State.SetColor(Colors.COLOR_RUINED);
			break;
			case GameConstants.STATE_BADLY_DAMAGED:
				m_State.SetText("BADLY DAMAGED");
				m_State.SetColor(Colors.COLOR_BADLY_DAMAGED);
			
			break;
			case GameConstants.STATE_DAMAGED:
				m_State.SetText("DAMAGED");
				m_State.SetColor(Colors.COLOR_DAMAGED);
			break;
			case GameConstants.STATE_WORN:
				m_State.SetText("WORN");
				m_State.SetColor(Colors.COLOR_WORN);
			break;
			case GameConstants.STATE_PRISTINE:
				m_State.SetText("PRISTINE");
				m_State.SetColor(Colors.COLOR_PRISTINE);
			break;
			default:
				m_State.Show(false);
			break;
		}
		m_Quanity.Show(false);
		UpdateQuanity(item);
		UpdateItemPreviw(item);
		
		m_LayoutRoot.SetHandler(this);
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
	
	void ~MarketStallItemSellerWidget(){
		Print("~MarketStallItemSellerWidget");
		delete m_LayoutRoot;
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