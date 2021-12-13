ref MarketStallSellerMenu m_MarketStallSellerMenu;
class MarketStallSellerMenu extends UIScriptedMenu {
	
	protected const autoptr TStringArray ROOT_LAYOUT_PATH = {"PlayerMarkets/gui/layout/MarketStallSeller.layout","PlayerMarkets/gui/layout/Modern/MarketStallSeller.layout"};
	protected GridSpacerWidget m_StallItemsGrid;
	protected GridSpacerWidget m_InventoryGrid;
	protected ButtonWidget m_Withdraw;
	protected ButtonWidget m_CurrencyUsed;
	protected TextWidget m_Withdraw_label;
	protected TextWidget m_Withdraw_Text;
	protected TextWidget m_RemainingSlots;
	protected TextWidget m_CurrencyUsed_label;
	
	protected autoptr array<autoptr MarketStallItemSellerWidget> m_ItemWidgets;
	protected autoptr array<autoptr MarketStallAvailableItemWidget> m_AvailableItemWidgets;
	
	protected autoptr MarketStallSetPriceWidget m_MarketStallSetPriceWidget;
	protected bool m_AwaitingRefresh = false;
	protected MarketStandBase m_Stand;
	
	protected int m_CurrencyUsedIdx = -1;
	
	override Widget Init()
    {
		layoutRoot 				= Widget.Cast(GetGame().GetWorkspace().CreateWidgets(ROOT_LAYOUT_PATH[GetPMConfig().GUIOption]));
		
		m_StallItemsGrid 		= GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("StallItemsGrid"));
		m_InventoryGrid 		= GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("InventoryGrid"));
		
		m_Withdraw 				= ButtonWidget.Cast(layoutRoot.FindAnyWidget("Withdraw"));
		m_Withdraw_label 		= TextWidget.Cast(layoutRoot.FindAnyWidget("Withdraw_label"));
		
		m_Withdraw_Text 		= TextWidget.Cast(layoutRoot.FindAnyWidget("Withdraw_Text"));
		m_RemainingSlots 		= TextWidget.Cast(layoutRoot.FindAnyWidget("RemainingSlots"));
		m_CurrencyUsed 			= ButtonWidget.Cast(layoutRoot.FindAnyWidget("CurrencyUsed"));
		m_CurrencyUsed_label 	= TextWidget.Cast(layoutRoot.FindAnyWidget("CurrencyUsed_label"));
		
		
		
		GetGame().GetMission().GetHud().Show(false);
    	PPEffects.SetBlurInventory(0.5);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.RefreshGUI, 1200,true);
		return layoutRoot;
	}
	
	void ~MarketStallSellerMenu(){
		if (m_Stand){
			m_Stand.SetIsInUse(false);
		}
		GetGame().GetMission().GetHud().Show(true);
    	PPEffects.SetBlurInventory(0);
		//if (m_AwaitingRefresh){
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.RefreshGUI);
		//}
		MSUnLockControls();
	}
	
	void SetStall(MarketStandBase stand){
		if (Class.CastTo(m_Stand, stand)){
			RefreshGUI();
			if (GetPMConfig().Currencies.Count() == 1){
				m_CurrencyUsed.Show(false);
				m_Withdraw_label.SetText(GetPMConfig().Currencies.Get(0).CurrencyName);
				m_Stand.SetCurrencyUsed(GetPMConfig().Currencies.Get(0).CurrencyName);
			} else {
				m_CurrencyUsed_label.SetText(m_Stand.GetCurrencyUsed());
				m_CurrencyUsedIdx = GetPMConfig().GetCurrencyIdx(m_Stand.GetCurrencyUsed());
				if (m_CurrencyUsedIdx == -1){
					Error2("[PLAYER MARKETS]","Currency Index is out of bounds");
					m_CurrencyUsedIdx = 0;
					m_CurrencyUsed_label.SetText("ERROR");
				}
			}
			MSLockControls();
		}
	}
	
	protected int m_NextClickTimer = 0;
	override bool OnClick(Widget w, int x, int y, int button){
		int curTime = GetGame().GetTime();
		
		if (w == m_Withdraw && m_Stand && curTime > m_NextClickTimer){
			m_NextClickTimer = curTime + 1500;
			m_Stand.RequestWithdraw();
			return true;
		}
		
		if (w == m_CurrencyUsed && GetPMConfig().Currencies.Count() > 1 && m_Stand && curTime > m_NextClickTimer && GetStand().GetMoneyBalance() == 0){
			m_NextClickTimer = curTime + 1500;
			m_CurrencyUsedIdx++;
			if (m_CurrencyUsedIdx >= GetPMConfig().Currencies.Count()){
				m_CurrencyUsedIdx = 0;
			}
			m_CurrencyUsed_label.SetText(GetPMConfig().Currencies.Get(m_CurrencyUsedIdx).CurrencyName);
			m_Stand.SetCurrencyUsed(GetPMConfig().Currencies.Get(m_CurrencyUsedIdx).CurrencyName);
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
		m_Withdraw_Text.SetText("#@ui_withdraw_available " + UUtil.ConvertIntToNiceString(GetStand().GetMoneyBalance()));
		if (m_Stand.GetMaxItemsForSale() < 0){
			m_RemainingSlots.Show(false);
		} else {
			m_RemainingSlots.SetText(m_Stand.GetItemsForSaleCount().ToString() + "/" + m_Stand.GetMaxItemsForSale() + " Items for sale");
		}
	}
}