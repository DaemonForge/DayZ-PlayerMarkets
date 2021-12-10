ref MarketStallMenu m_MarketStallMenu;
class MarketStallMenu extends UIScriptedMenu {
	
	protected const string ROOT_LAYOUT_PATH = "PlayerMarkets/gui/layout/MarketStall.layout";
	protected TextWidget m_ShopTitle;
	protected GridSpacerWidget m_ItemGrid;
	protected TextWidget m_Balance;
	
	protected autoptr array<autoptr MarketStallItemWidget> m_ItemWidgets;
	
	protected MarketStandBase m_Stand;
	
	override Widget Init()
    {
		layoutRoot 		= Widget.Cast(GetGame().GetWorkspace().CreateWidgets(ROOT_LAYOUT_PATH));
		
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
			m_Stand.SetIsInUse(true);
			m_ShopTitle.SetText(m_Stand.GetStandName());
			m_ItemWidgets = new array<autoptr MarketStallItemWidget>;
			array<autoptr PlayerMarketItemDetails> itemsArray = m_Stand.GetItemsArray();
			if (itemsArray && itemsArray.Count() > 0){
				for (int i = 0; i < itemsArray.Count(); i++){
					if (itemsArray.Get(i).GetItem()){
						m_ItemWidgets.Insert(new MarketStallItemWidget(m_ItemGrid, itemsArray.Get(i),this));
					}
				}
			}
			MSLockControls();
		}
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
			m_Balance.SetText("On You: $" +  UUtil.ConvertIntToNiceString(player.UGetPlayerBalance("Coins")));
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