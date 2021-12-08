ref MarketStallSellerMenu m_MarketStallSellerMenu;
class MarketStallSellerMenu extends UIScriptedMenu {
	
	protected const string ROOT_LAYOUT_PATH = "PlayerMarkets/gui/layout/MarketStallSeller.layout";
	protected GridSpacerWidget m_StallItemsGrid;
	protected GridSpacerWidget m_InventoryGrid;
	
	protected autoptr array<autoptr MarketStallItemSellerWidget> m_ItemWidgets;
	protected autoptr array<autoptr MarketStallAvailableItemWidget> m_AvailableItemWidgets;
	
	protected MarketStandBase m_Stand;
	
	override Widget Init()
    {
		layoutRoot 			= Widget.Cast(GetGame().GetWorkspace().CreateWidgets(ROOT_LAYOUT_PATH));
		
		m_StallItemsGrid 	= GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("StallItemsGrid"));
		m_InventoryGrid 	= GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("InventoryGrid"));
		Print(m_StallItemsGrid);
		Print(m_InventoryGrid);
		return layoutRoot;
	}
	
	void ~MarketStallSellerMenu(){
		if (m_Stand){
			m_Stand.SetIsInUse(false);
		}
		MSUnLockControls();
	}
	
	void SetStall(MarketStandBase stand){
		if (Class.CastTo(m_Stand, stand)){
			m_Stand.SetIsInUse(true);
			m_ItemWidgets = new array<autoptr MarketStallItemSellerWidget>;
			array<autoptr PlayerMarketItemDetails> itemsArray = m_Stand.GetItemsArray();
			if (itemsArray && itemsArray.Count() > 0){
				for (int i = 0; i < itemsArray.Count(); i++){
					if (itemsArray.Get(i).GetItem() && itemsArray.Get(i).GetItem().GetHierarchyRoot() == stand){
						m_ItemWidgets.Insert(new MarketStallItemSellerWidget(m_StallItemsGrid, itemsArray.Get(i),this));
					}
				}
			}
			array<EntityAI> availableItems = m_Stand.GetItemsInCargo();
			m_AvailableItemWidgets = new array<autoptr MarketStallAvailableItemWidget>;
			foreach (EntityAI item : availableItems){
				m_AvailableItemWidgets.Insert(new MarketStallAvailableItemWidget(m_InventoryGrid, item, this));
			}
			MSLockControls();
		}
	}
	
	override bool OnKeyPress(Widget w, int x, int y, int key){
		Print(key);
		return super.OnKeyPress(w,x,y,key);
	}
	
	override void Update( float timeslice )
	{
		super.Update( timeslice );
		if ( GetGame().GetInput().LocalPress( "UAUIBack", false ) && !EditIsOpen()) {
			Print("UAUIBack");
			GetGame().GetUIManager().CloseMenu(PLAYER_MARKET_MENU_SELL);
		}
	}
	
	bool EditIsOpen(){
		return false;
	}
	
	MarketStandBase GetStand(){
		return m_Stand;
	}
	
	protected void MSLockControls() {
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_INVENTORY);
        GetGame().GetUIManager().ShowUICursor(true);
    }

    protected void MSUnLockControls() {
        GetGame().GetMission().PlayerControlEnable(false);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetUIManager().ShowUICursor(false);
    }
}