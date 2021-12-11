ref MarketStallMenu m_MarketStallMenu;
class MarketStallMenu extends UIScriptedMenu {
	
	protected const string ROOT_LAYOUT_PATH = "PlayerMarkets/gui/layout/MarketStall.layout";
	protected TextWidget m_ShopTitle;
	protected GridSpacerWidget m_ItemGrid;
	protected TextWidget m_Balance;
	protected int m_PlayerBalance;
	protected Widget m_ItemsListed;
	
	protected autoptr array<autoptr MarketStallItemWidget> m_ItemWidgets;
	
	protected MarketStandBase m_Stand;
	protected autoptr MarketStallItemView m_MarketStallItemView;
	protected bool m_AwaitingRefresh = false;
	
	override Widget Init()
    {
		layoutRoot 		= Widget.Cast(GetGame().GetWorkspace().CreateWidgets(ROOT_LAYOUT_PATH));
		m_ItemsListed 	= Widget.Cast(layoutRoot.FindAnyWidget("ItemsListed"));
		m_ShopTitle 	= TextWidget.Cast(layoutRoot.FindAnyWidget("ShopTitle"));
		m_ItemGrid 		= GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("ItemGrid"));
		m_Balance		= TextWidget.Cast(layoutRoot.FindAnyWidget("Balance"));
		return layoutRoot;
	}
	
	void ~MarketStallMenu() {
		if (m_Stand) {
			m_Stand.SetIsInUse(false);
		}
		MSUnLockControls();
	}
	
	void SetStall(MarketStandBase stand){
		Print("SetStall");
		if (Class.CastTo(m_Stand, stand)){
			RefreshGUI();
			MSLockControls();
		}
	}
	
	void CloseViewItem(){
		m_ItemsListed.Show(true);
		Print("CloseViewItem");
		m_MarketStallItemView = NULL;
		m_AwaitingRefresh = true;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.RefreshGUI, 900);
		Print("CloseViewItem End");
	}
	
	void OpenViewItem(PlayerMarketItemDetails item){
		m_ItemsListed.Show(false);
		m_MarketStallItemView = new MarketStallItemView(layoutRoot, item, this);
	}
	
	override bool OnKeyPress(Widget w, int x, int y, int key){
		return super.OnKeyPress(w,x,y,key);
	}
	
	override void Update( float timeslice )
	{
		super.Update( timeslice );
		if ( GetGame().GetInput().LocalPress( "UAUIBack", false ) && !InspectIsOpen()) {
			GetGame().GetUIManager().CloseMenu(PLAYER_MARKET_MENU_BUY);
		}
		PlayerBase player;
		if (Class.CastTo(player, GetGame().GetPlayer())){
			m_PlayerBalance = player.UGetPlayerBalance("Coins")
			m_Balance.SetText("On You: $" +  UUtil.ConvertIntToNiceString(m_PlayerBalance));
		}
	}
	
	int GetPlayerBalance(){
		return m_PlayerBalance;
	}
	
	void RefreshGUI(){
			GetStand().SetIsInUse(true);
			m_ShopTitle.SetText(GetStand().GetStandName());
			m_ItemWidgets = new array<autoptr MarketStallItemWidget>;
			array<autoptr PlayerMarketItemDetails> itemsArray = GetStand().GetItemsArray();
			if (itemsArray && itemsArray.Count() > 0){
				for (int i = 0; i < itemsArray.Count(); i++){
					if (itemsArray.Get(i).GetItem()){
						m_ItemWidgets.Insert(new MarketStallItemWidget(m_ItemGrid, itemsArray.Get(i),this));
					}
				}
			}
	}
	
	bool InspectIsOpen(){
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