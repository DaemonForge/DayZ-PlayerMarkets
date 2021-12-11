ref MarketStallSellerMenu m_MarketStallSellerMenu;
class MarketStallSellerMenu extends UIScriptedMenu {
	
	protected const string ROOT_LAYOUT_PATH = "PlayerMarkets/gui/layout/MarketStallSeller.layout";
	protected GridSpacerWidget m_StallItemsGrid;
	protected GridSpacerWidget m_InventoryGrid;
	protected ButtonWidget m_Withdraw;
	protected TextWidget m_Withdraw_label;
	
	protected autoptr array<autoptr MarketStallItemSellerWidget> m_ItemWidgets;
	protected autoptr array<autoptr MarketStallAvailableItemWidget> m_AvailableItemWidgets;
	
	protected autoptr MarketStallSetPriceWidget m_MarketStallSetPriceWidget;
	protected bool m_AwaitingRefresh = false;
	protected MarketStandBase m_Stand;
	
	override Widget Init()
    {
		layoutRoot 			= Widget.Cast(GetGame().GetWorkspace().CreateWidgets(ROOT_LAYOUT_PATH));
		
		m_StallItemsGrid 	= GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("StallItemsGrid"));
		m_InventoryGrid 	= GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("InventoryGrid"));
		
		m_Withdraw 			= ButtonWidget.Cast(layoutRoot.FindAnyWidget("Withdraw"));
		m_Withdraw_label 	= TextWidget.Cast(layoutRoot.FindAnyWidget("Withdraw_label"));
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.RefreshGUI, 900,true);
		return layoutRoot;
	}
	
	void ~MarketStallSellerMenu(){
		if (m_Stand){
			m_Stand.SetIsInUse(false);
		}
		//if (m_AwaitingRefresh){
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.RefreshGUI);
		//}
		MSUnLockControls();
	}
	
	void SetStall(MarketStandBase stand){
		if (Class.CastTo(m_Stand, stand)){
			RefreshGUI();
			MSLockControls();
		}
	}
	
	override bool OnClick(Widget w, int x, int y, int button){
		
		if (w == m_Withdraw && m_Stand){
			m_Stand.RequestWithdraw();
			return true;
		}
		
		return super.OnClick(w,x,y,button);
	
	}
	
	void CloseSetPrice(){
		m_MarketStallSetPriceWidget = NULL;
		m_AwaitingRefresh = true;
		//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.RefreshGUI, 900);
	}
	
	void OpenSetPrice(EntityAI item){
		m_MarketStallSetPriceWidget = new MarketStallSetPriceWidget(layoutRoot, item, this);
	}
	
	override void Update( float timeslice )
	{
		super.Update( timeslice );
		if ( GetGame().GetInput().LocalPress( "UAUIBack", false ) && !EditIsOpen()) {
			GetGame().GetUIManager().CloseMenu(PLAYER_MARKET_MENU_SELL);
		}
		if ( GetGame().GetInput().LocalPress( "UAUIBack", false ) && EditIsOpen()) {
			CloseSetPrice();
		}
	}
	
	bool EditIsOpen() {
		return (m_MarketStallSetPriceWidget != NULL);
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
	
	void RequestRefresh(){
		if (!m_AwaitingRefresh){
			m_AwaitingRefresh = true;
			//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.RefreshGUI, 900);
		}
	}
	
	void RefreshGUI(){
		m_AwaitingRefresh = false;
		if (!GetStand()){return;}
		GetStand().SetIsInUse(true);
		if (m_ItemWidgets){
			m_ItemWidgets.Clear();
		}
		m_ItemWidgets = new array<autoptr MarketStallItemSellerWidget>;
		array<autoptr PlayerMarketItemDetails> itemsArray = GetStand().GetItemsArray();
		if (itemsArray && itemsArray.Count() > 0){
			for (int i = 0; i < itemsArray.Count(); i++){
				if (itemsArray.Get(i).GetItem()){
					m_ItemWidgets.Insert(new MarketStallItemSellerWidget(m_StallItemsGrid, itemsArray.Get(i),this));
				}
			}
		}
		array<EntityAI> availableItems = GetStand().GetItemsInCargo();
		if (m_AvailableItemWidgets){
			m_AvailableItemWidgets.Clear();
		}
		m_AvailableItemWidgets = new array<autoptr MarketStallAvailableItemWidget>;
		foreach (EntityAI item : availableItems){
			m_AvailableItemWidgets.Insert(new MarketStallAvailableItemWidget(m_InventoryGrid, item, this));
		}
		m_Withdraw_label.SetText("Withdraw: " + UUtil.ConvertIntToNiceString(GetStand().GetMoneyBalance()));
	}
}