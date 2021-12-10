class MarketStallItemEditScreen  extends ScriptedWidgetEventHandler {
	protected const string ITEM_LAYOUT_PATH = "PlayerMarkets/gui/layout/MarketStallItemEdit.layout";
	protected autoptr PlayerMarketItemDetails m_ItemDetails;
	
	protected autoptr MarketStallSellerMenu m_parent;
	protected Widget m_LayoutRoot;
	
	protected ItemPreviewWidget m_ItemPreview;
	protected TextWidget m_DisplayName;
	protected TextWidget m_Quanity;
	protected TextWidget m_State;
	
	protected ButtonWidget m_Edit;
	protected ButtonWidget m_Delist;
	
	void MarketStallItemEditScreen(Widget parent, EntityAI item, MarketStallSellerMenu menu ){
		m_LayoutRoot = Widget.Cast(GetGame().GetWorkspace().CreateWidgets(ITEM_LAYOUT_PATH,parent));
		m_parent = MarketStallSellerMenu.Cast(menu);
		//m_ItemDetails = PlayerMarketItemDetails.Cast(details);
		
		m_ItemPreview = ItemPreviewWidget.Cast(m_LayoutRoot.FindAnyWidget("ItemPreview"));
		m_DisplayName = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("DisplayName"));
		m_Quanity = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Quanity"));
		m_State = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("State"));
		
		m_Edit = ButtonWidget.Cast(m_LayoutRoot.FindAnyWidget("Edit"));
		m_Delist = ButtonWidget.Cast(m_LayoutRoot.FindAnyWidget("Delist"));
		
		
		//0 = pristine, 1 = worn, 2 = damaged, 3 = badly damaged, 4 = ruined
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
	
	
	void UpdateQuanity(EntityAI item){
		Magazine mag;
		if (Class.CastTo(mag, item)){
			m_Quanity.Show(true);
			string magText = mag.GetAmmoCount().ToString();
			m_Quanity.SetText( magText );
			return;
		}
		ItemBase itemB;
		if (!Class.CastTo(itemB, item) || !itemB.HasQuantity()){
			return;
		}
		m_Quanity.Show(true);
		string text = itemB.GetQuantity().ToString();
		string units = itemB.ConfigGetString("stackedUnit");
		if (units == "percent" && itemB.GetQuantityMax() > 0){
			float number = itemB.GetQuantity() / itemB.GetQuantityMax();
			int num = number * 100;
			text = num.ToString() + "%";
		} else if (units != ""){
			text = UUtil.ConvertIntToNiceString(itemB.GetQuantity()) + "/" + UUtil.ConvertIntToNiceString(itemB.GetQuantityMax()) + units;
		}
		m_Quanity.SetText( text );
	}
	
		
}