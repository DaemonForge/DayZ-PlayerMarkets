class MarketStallItemWidget  extends ScriptedWidgetEventHandler {
	protected const string ITEM_LAYOUT_PATH = "PlayerMarkets/gui/layout/MarketItem.layout";
	protected autoptr PlayerMarketItemDetails m_ItemDetails;
	
	protected autoptr MarketStallMenu m_parent;
	protected Widget m_LayoutRoot;
	
	protected ItemPreviewWidget m_ItemPreview;
	protected TextWidget m_ItemName;
	protected TextWidget m_Cost;
	protected TextWidget m_Quanity;
	protected TextWidget m_State;
	
	
	void MarketStallItemWidget(Widget parent, PlayerMarketItemDetails details, MarketStallMenu menu ){
		m_LayoutRoot = Widget.Cast(GetGame().GetWorkspace().CreateWidgets(ITEM_LAYOUT_PATH,parent));
		m_parent = MarketStallMenu.Cast(menu);
		m_ItemDetails = PlayerMarketItemDetails.Cast(details);
		
		m_ItemPreview = ItemPreviewWidget.Cast(m_LayoutRoot.FindAnyWidget("ItemPreview"));
		m_ItemName = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("ItemName"));
		m_Cost = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Cost"));
		m_Quanity = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Quanity"));
		m_State = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("State"));
		
		
		
		m_Cost.SetText(m_ItemDetails.GetPrice().ToString());
		
		//0 = pristine, 1 = worn, 2 = damaged, 3 = badly damaged, 4
		EntityAI item = m_ItemDetails.GetItem();
		m_ItemName.SetText(item.GetDisplayName());
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
	}
	
	void UpdateQuanity(EntityAI item){
		ItemBase itemB;
		if (!Class.CastTo(itemB, item) || !itemB.HasQuantity()){
			return;
		}
		m_Quanity.Show(true);
		
	}
}