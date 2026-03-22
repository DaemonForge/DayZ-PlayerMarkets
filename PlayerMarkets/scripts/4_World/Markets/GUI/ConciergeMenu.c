static ref ConciergeMenu m_ConciergeMenu;

class ConciergeMenu extends UIScriptedMenu {
	
	protected static const autoptr TStringArray ROOT_LAYOUT_PATH = {"PlayerMarkets/gui/layout/ConciergeMenu.layout","PlayerMarkets/gui/layout/modern/ConciergeMenu.layout"};
	
	protected TextWidget m_Title;
	protected GridSpacerWidget m_ItemGrid;
	protected GridSpacerWidget m_FilterGrid;
	protected TextWidget m_Balance;
	protected TextWidget m_ItemCount;
	protected Widget m_ItemsListed;
	protected Widget m_ItemViewFrame;
	protected int m_PlayerBalance;
	
	// Confirmation dialog widgets
	protected Widget m_ConfirmOverlay;
	protected TextWidget m_ConfirmItemName;
	protected TextWidget m_ConfirmStallName;
	protected TextWidget m_ConfirmBasePrice;
	protected TextWidget m_ConfirmTax;
	protected TextWidget m_ConfirmTotal;
	protected TextWidget m_ConfirmCurrency;
	protected TextWidget m_ConfirmBalanceInfo;
	protected ButtonWidget m_ConfirmBuyBtn;
	protected ButtonWidget m_ConfirmCancelBtn;
	
	protected ref array<ref ConciergeItemWidget> m_ItemWidgets;
	protected ref array<ref ConciergeFilterButton> m_FilterButtons;
	
	protected ref array<ref PMConciergeEntry> m_AllEntries;
	protected ref array<string> m_UniqueCurrencies;
	protected int m_ActiveFilter = PMItemCategory.ALL;
	
	protected PM_ConciergeNPC m_Concierge;
	protected ref PMConciergeEntry m_PendingBuyEntry;
	protected ref ConciergeItemView m_ItemView;
	
	override Widget Init(){
		layoutRoot = Widget.Cast(g_Game.GetWorkspace().CreateWidgets(ROOT_LAYOUT_PATH[GetPMConfig().GUIOption]));
		if (!layoutRoot) return layoutRoot;
		
		int screenW, screenH;
		GetScreenSize(screenW, screenH);
		float panelW = Math.Min(1400, screenW * 0.85);
		float panelH = Math.Min(850, screenH * 0.85);
		layoutRoot.SetSize(panelW, panelH);
		
		// Size child FrameWidgets from code (proportional sizing in FrameWidget children references screen, not parent)
		float contentW = panelW * 0.92;
		Widget header = layoutRoot.FindAnyWidget("Header");
		if (header) header.SetSize(panelW * 0.9, panelH * 0.06);
		Widget filterBar = layoutRoot.FindAnyWidget("FilterBar");
		if (filterBar) filterBar.SetSize(contentW, 36);
		Widget itemsListedW = layoutRoot.FindAnyWidget("ItemsListed");
		if (itemsListedW) itemsListedW.SetSize(contentW, panelH * 0.74);
		Widget itemViewFrameW = layoutRoot.FindAnyWidget("ItemViewFrame");
		if (itemViewFrameW) itemViewFrameW.SetSize(contentW, panelH * 0.82);
		Widget bottomBar = layoutRoot.FindAnyWidget("BottomBar");
		if (bottomBar) bottomBar.SetSize(contentW, 44);
		Widget confirmOverlayW = layoutRoot.FindAnyWidget("ConfirmOverlay");
		if (confirmOverlayW) confirmOverlayW.SetSize(panelW, panelH);
		
		m_Title = TextWidget.Cast(layoutRoot.FindAnyWidget("Title"));
		m_ItemGrid = GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("ItemGrid"));
		m_FilterGrid = GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("FilterGrid"));
		m_Balance = TextWidget.Cast(layoutRoot.FindAnyWidget("Balance"));
		m_ItemCount = TextWidget.Cast(layoutRoot.FindAnyWidget("ItemCount"));
		m_ItemsListed = Widget.Cast(layoutRoot.FindAnyWidget("ItemsListed"));
		m_ItemViewFrame = Widget.Cast(layoutRoot.FindAnyWidget("ItemViewFrame"));
		
		// Confirmation dialog
		m_ConfirmOverlay = Widget.Cast(layoutRoot.FindAnyWidget("ConfirmOverlay"));
		m_ConfirmItemName = TextWidget.Cast(layoutRoot.FindAnyWidget("ConfirmItemName"));
		m_ConfirmStallName = TextWidget.Cast(layoutRoot.FindAnyWidget("ConfirmStallName"));
		m_ConfirmBasePrice = TextWidget.Cast(layoutRoot.FindAnyWidget("ConfirmBasePrice"));
		m_ConfirmTax = TextWidget.Cast(layoutRoot.FindAnyWidget("ConfirmTax"));
		m_ConfirmTotal = TextWidget.Cast(layoutRoot.FindAnyWidget("ConfirmTotal"));
		m_ConfirmCurrency = TextWidget.Cast(layoutRoot.FindAnyWidget("ConfirmCurrency"));
		m_ConfirmBalanceInfo = TextWidget.Cast(layoutRoot.FindAnyWidget("ConfirmBalanceInfo"));
		m_ConfirmBuyBtn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ConfirmBuyBtn"));
		m_ConfirmCancelBtn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ConfirmCancelBtn"));
		
		m_ItemWidgets = new array<ref ConciergeItemWidget>;
		m_FilterButtons = new array<ref ConciergeFilterButton>;
		m_AllEntries = new array<ref PMConciergeEntry>;
		m_UniqueCurrencies = new array<string>;
		
		CreateFilterButtons();
		
		g_Game.GetMission().GetHud().Show(false);
		PPEffects.SetBlurInventory(0.5);
		
		return layoutRoot;
	}
	
	void ~ConciergeMenu(){
		g_Game.GetMission().GetHud().Show(true);
		PPEffects.SetBlurInventory(0);
		MSUnLockControls();
		m_ConciergeMenu = NULL;
	}
	
	void SetConcierge(PM_ConciergeNPC concierge){
		m_Concierge = concierge;
		
		PMMarketArea area = concierge.GetLinkedArea();
		if (area){
			m_Title.SetText(area.Name + " - Concierge");
		} else {
			m_Title.SetText("Market Concierge");
		}
		
		MSLockControls();
		concierge.RequestConciergeSync();
	}
	
	void OnDataReceived(array<ref PMConciergeEntry> entries){
		m_AllEntries = entries;
		HideConfirmDialog();
		CacheUniqueCurrencies();
		RefreshItemGrid();
	}
	
	protected void CacheUniqueCurrencies(){
		m_UniqueCurrencies.Clear();
		if (m_AllEntries){
			for (int i = 0; i < m_AllEntries.Count(); i++){
				PMConciergeEntry e = m_AllEntries.Get(i);
				if (!e) continue;
				if (m_UniqueCurrencies.Find(e.m_CurrencyUsed) == -1){
					m_UniqueCurrencies.Insert(e.m_CurrencyUsed);
				}
			}
		}
		if (m_UniqueCurrencies.Count() == 0){
			m_UniqueCurrencies.Insert("Coins");
		}
	}
	
	void SetFilter(int category){
		m_ActiveFilter = category;
		foreach (ConciergeFilterButton btn : m_FilterButtons){
			btn.SetActive(btn.GetCategory() == category);
		}
		RefreshItemGrid();
	}
	
	// Called when player clicks an item card - opens the detail view
	void OpenViewItem(PMConciergeEntry entry){
		if (!entry) return;
		m_ItemsListed.Show(false);
		m_ItemViewFrame.Show(true);
		m_ItemView = new ConciergeItemView(m_ItemViewFrame, entry, this);
	}
	
	void CloseViewItem(){
		m_ItemsListed.Show(true);
		m_ItemViewFrame.Show(false);
		m_ItemView = NULL;
	}
	
	bool InspectIsOpen(){
		return (m_ItemView != NULL);
	}
	
	// Called from item view Buy button - shows confirmation
	void OnBuyItem(PMConciergeEntry entry){
		if (!m_Concierge || !entry) return;
		ShowConfirmDialog(entry);
	}
	
	protected void ShowConfirmDialog(PMConciergeEntry entry){
		m_PendingBuyEntry = entry;
		
		m_ConfirmItemName.SetText(entry.m_ItemName);
		m_ConfirmStallName.SetText("From: " + entry.m_StallName);
		
		int basePrice = entry.m_Price;
		int totalPrice = entry.GetPriceWithTax();
		int taxAmount = totalPrice - basePrice;
		
		m_ConfirmBasePrice.SetText("Base Price: $" + UUtil.ConvertIntToNiceString(basePrice));
		
		float effectiveTax = Math.Clamp(GetPMConfig().SaleTaxAmount + entry.m_AreaTaxModifier, 0.0, 1.0);
		int taxPercent = Math.Round(effectiveTax * 100);
		if (taxAmount > 0){
			m_ConfirmTax.SetText("Tax: $" + UUtil.ConvertIntToNiceString(taxAmount) + " (" + taxPercent.ToString() + "%)");
		} else {
			m_ConfirmTax.SetText("Tax: None");
		}
		
		m_ConfirmTotal.SetText("Total: $" + UUtil.ConvertIntToNiceString(totalPrice));
		m_ConfirmCurrency.SetText("Currency: " + entry.m_CurrencyUsed);
		
		PlayerBase player;
		if (Class.CastTo(player, g_Game.GetPlayer())){
			int balance = player.UGetPlayerBalance(entry.m_CurrencyUsed);
			m_ConfirmBalanceInfo.SetText("Your Balance: $" + UUtil.ConvertIntToNiceString(balance));
			if (balance < totalPrice){
				m_ConfirmBalanceInfo.SetColor(ARGBF(1, 0.8, 0.3, 0.3));
			} else {
				m_ConfirmBalanceInfo.SetColor(ARGBF(1, 0.4, 0.8, 0.4));
			}
		}
		
		m_ConfirmOverlay.Show(true);
	}
	
	protected void HideConfirmDialog(){
		m_PendingBuyEntry = NULL;
		if (m_ConfirmOverlay){
			m_ConfirmOverlay.Show(false);
		}
	}
	
	protected void ExecuteBuy(){
		if (!m_Concierge || !m_PendingBuyEntry) return;
		
		PlayerBase player;
		if (Class.CastTo(player, g_Game.GetPlayer())){
			int balance = player.UGetPlayerBalance(m_PendingBuyEntry.m_CurrencyUsed);
			int priceWithTax = m_PendingBuyEntry.GetPriceWithTax();
			if (balance < priceWithTax){
				HideConfirmDialog();
				return;
			}
		}
		
		m_Concierge.RequestConciergeBuy(m_PendingBuyEntry);
		HideConfirmDialog();
		CloseViewItem();
	}
	
	override bool OnClick(Widget w, int x, int y, int button){
		if (w == m_ConfirmBuyBtn){
			ExecuteBuy();
			return true;
		}
		if (w == m_ConfirmCancelBtn){
			HideConfirmDialog();
			return true;
		}
		return super.OnClick(w, x, y, button);
	}
	
	override void Update(float timeslice){
		super.Update(timeslice);
		
		if (g_Game.GetInput().LocalPress("UAUIBack", false)){
			if (m_PendingBuyEntry){
				HideConfirmDialog();
				return;
			}
			if (InspectIsOpen()){
				CloseViewItem();
				return;
			}
			g_Game.GetUIManager().CloseMenu(PLAYER_MARKET_MENU_CONCIERGE);
		}
		
		PlayerBase player;
		if (Class.CastTo(player, g_Game.GetPlayer()) && m_Concierge){
			string balanceText = "";
			for (int c = 0; c < m_UniqueCurrencies.Count(); c++){
				string curr = m_UniqueCurrencies.Get(c);
				int bal = player.UGetPlayerBalance(curr);
				if (c > 0){
					balanceText = balanceText + "  |  ";
				}
				balanceText = balanceText + curr + ": $" + UUtil.ConvertIntToNiceString(bal);
			}
			m_Balance.SetText(balanceText);
		}
	}
	
	int GetPlayerBalance(string currency){
		PlayerBase player;
		if (Class.CastTo(player, g_Game.GetPlayer())){
			return player.UGetPlayerBalance(currency);
		}
		return 0;
	}
	
	PM_ConciergeNPC GetConcierge(){
		return m_Concierge;
	}
	
	protected void CreateFilterButtons(){
		for (int i = 0; i < PMItemCategoryHelper.GetCategoryCount(); i++){
			ConciergeFilterButton btn = new ConciergeFilterButton(m_FilterGrid, i, this);
			btn.SetActive(i == PMItemCategory.ALL);
			m_FilterButtons.Insert(btn);
		}
	}
	
	protected void RefreshItemGrid(){
		m_ItemWidgets = new array<ref ConciergeItemWidget>;
		
		if (!m_AllEntries || m_AllEntries.Count() == 0){
			m_ItemCount.SetText("No items listed");
			return;
		}
		
		array<ref PMConciergeEntry> filtered = new array<ref PMConciergeEntry>;
		for (int i = 0; i < m_AllEntries.Count(); i++){
			PMConciergeEntry entry = m_AllEntries.Get(i);
			if (!entry) continue;
			
			if (m_ActiveFilter != PMItemCategory.ALL && entry.m_Category != m_ActiveFilter){
				continue;
			}
			
			bool inserted = false;
			for (int j = 0; j < filtered.Count(); j++){
				PMConciergeEntry existing = filtered.Get(j);
				if (existing && entry.m_StallName < existing.m_StallName){
					filtered.InsertAt(entry, j);
					inserted = true;
					break;
				}
			}
			if (!inserted){
				filtered.Insert(entry);
			}
		}
		
		int displayCount = 0;
		for (int k = 0; k < filtered.Count(); k++){
			PMConciergeEntry filteredEntry = filtered.Get(k);
			if (!filteredEntry) continue;
			
			ConciergeItemWidget widget = new ConciergeItemWidget(m_ItemGrid, filteredEntry, this);
			m_ItemWidgets.Insert(widget);
			displayCount++;
		}
		
		m_ItemCount.SetText(displayCount.ToString() + " Items Listed");
	}
	
	protected void MSLockControls(){
		g_Game.GetMission().PlayerControlDisable(INPUT_EXCLUDE_INVENTORY);
		g_Game.GetUIManager().ShowUICursor(true);
	}
	
	protected void MSUnLockControls(){
		g_Game.GetMission().PlayerControlEnable(false);
		g_Game.GetInput().ResetGameFocus();
		g_Game.GetUIManager().ShowUICursor(false);
	}
}
