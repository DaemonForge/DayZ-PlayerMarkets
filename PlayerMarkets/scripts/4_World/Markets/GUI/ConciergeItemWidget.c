// Widget for displaying a single item in the concierge menu
class ConciergeItemWidget extends ScriptedWidgetEventHandler {
	
	protected static const autoptr TStringArray ITEM_LAYOUT_PATH = {"PlayerMarkets/gui/layout/ConciergeItem.layout","PlayerMarkets/gui/layout/modern/ConciergeItem.layout"};
	
	protected ref PMConciergeEntry m_Entry;
	protected ConciergeMenu m_Parent;
	protected Widget m_LayoutRoot;
	
	protected ItemPreviewWidget m_ItemPreview;
	protected TextWidget m_ItemName;
	protected TextWidget m_StallName;
	protected TextWidget m_Cost;
	protected TextWidget m_CurrencyLabel;
	protected TextWidget m_CategoryLabel;
	protected TextWidget m_HealthText;
	protected TextWidget m_Quanity;
	protected Widget m_ItemStateFrame;
	protected Widget m_ItemState;
	protected Widget m_BG;
	protected EntityAI m_PreviewItem;
	protected bool m_OwnsPreviewItem;
	
	void ConciergeItemWidget(Widget parent, PMConciergeEntry entry, ConciergeMenu menu){
		m_LayoutRoot = Widget.Cast(g_Game.GetWorkspace().CreateWidgets(ITEM_LAYOUT_PATH[GetPMConfig().GUIOption], parent));
		m_Parent = menu;
		m_Entry = entry;
		
		m_BG = Widget.Cast(m_LayoutRoot.FindAnyWidget("BG"));
		m_ItemPreview = ItemPreviewWidget.Cast(m_LayoutRoot.FindAnyWidget("ItemPreview"));
		m_ItemName = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("ItemName"));
		m_StallName = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("StallName"));
		m_Cost = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Cost"));
		m_CurrencyLabel = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("CurrencyLabel"));
		m_CategoryLabel = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("CategoryLabel"));
		m_HealthText = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("HealthText"));
		m_Quanity = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Quanity"));
		m_ItemStateFrame = Widget.Cast(m_LayoutRoot.FindAnyWidget("ItemStateFrame"));
		m_ItemState = Widget.Cast(m_LayoutRoot.FindAnyWidget("ItemState"));
		
		// Size InfoFrame from code (FrameWidget proportional sizing references screen, not parent)
		Widget infoFrame = m_LayoutRoot.FindAnyWidget("InfoFrame");
		if (infoFrame) {
			int screenW, screenH;
			GetScreenSize(screenW, screenH);
			float panelW = Math.Min(1400, screenW * 0.85);
			float contentW = panelW * 0.92;
			float cellW = (contentW - 12) / 2;
			float infoX, infoY;
			infoFrame.GetPos(infoX, infoY);
			float rootW, rootH;
			m_LayoutRoot.GetSize(rootW, rootH);
			Widget priceFrame = m_LayoutRoot.FindAnyWidget("PriceFrame");
			float priceW = 130;
			if (priceFrame) {
				float pw, ph;
				priceFrame.GetSize(pw, ph);
				priceW = pw + 10;
			}
			float infoW = cellW - infoX - priceW;
			if (infoW < 100) infoW = 100;
			infoFrame.SetSize(infoW, rootH - 20);
		}
		
		// Set item info
		m_ItemName.SetText(entry.m_ItemName);
		
		if (entry.m_StallName != ""){
			m_StallName.SetText("Sold by: " + entry.m_StallName);
		} else {
			m_StallName.SetText("Sold by: Unknown Stall");
		}
		
		if (m_CategoryLabel){
			m_CategoryLabel.SetText(PMItemCategoryHelper.GetCategoryName(entry.m_Category));
		}
		
		int price = entry.GetPriceWithTax();
		m_Cost.SetText("$" + UUtil.ConvertIntToNiceString(price));
		
		if (m_CurrencyLabel){
			m_CurrencyLabel.SetText(entry.m_CurrencyUsed);
		}
		
		// Health state from synced data
		UpdateHealthDisplay(entry.m_HealthLevel);
		
		// Quantity display from synced data
		UpdateQuantityDisplay(entry);
		
		// Item preview — use actual item from stall if available, fall back to replica
		m_OwnsPreviewItem = false;
		if (entry.m_ItemClass != ""){
			// Try to find the real item on the stall (client-side)
			m_PreviewItem = GetMarketAreaManager().FindActualItemForEntry(entry);
			if (!m_PreviewItem){
				// Fallback: create a replica from class name
				m_PreviewItem = EntityAI.Cast(GetGame().CreateObjectEx(entry.m_ItemClass, "0 0 0", ECE_LOCAL | ECE_NOLIFETIME));
				m_OwnsPreviewItem = true;
				// Spawn attachments on replica
				if (m_PreviewItem && entry.m_Attachments && entry.m_Attachments.Count() > 0){
					for (int att = 0; att < entry.m_Attachments.Count(); att++){
						string attClass = entry.m_Attachments[att];
						if (attClass == "") continue;
						m_PreviewItem.GetInventory().CreateInInventory(attClass);
					}
				}
			}
			if (m_PreviewItem && m_ItemPreview){
				InventoryItem iItem = InventoryItem.Cast(m_PreviewItem);
				if (iItem){
					m_ItemPreview.SetItem(iItem);
					m_ItemPreview.SetModelPosition(Vector(0,0,0));
					m_ItemPreview.SetModelOrientation(Vector(0,0,0));
					m_ItemPreview.SetView(0);
					m_ItemPreview.SetPos(0, 0);
					m_ItemPreview.SetSize(1, 1);
				}
			}
		}
		
		m_LayoutRoot.SetHandler(this);
	}
	
	void ~ConciergeItemWidget(){
		if (m_PreviewItem && m_OwnsPreviewItem){
			GetGame().ObjectDelete(m_PreviewItem);
		}
		m_PreviewItem = NULL;
		if (m_LayoutRoot){
			m_LayoutRoot.Show(false);
			delete m_LayoutRoot;
		}
	}
	
	PMConciergeEntry GetEntry(){
		return m_Entry;
	}
	
	protected void UpdateHealthDisplay(int healthLevel){
		if (!m_ItemStateFrame) return;
		
		string healthName = "";
		int healthColor = 0;
		
		switch (healthLevel){
			case GameConstants.STATE_PRISTINE:
				m_ItemStateFrame.Show(true);
				if (m_ItemState) m_ItemState.SetColor(Colors.COLOR_PRISTINE);
				healthName = "Pristine";
				break;
			case GameConstants.STATE_WORN:
				m_ItemStateFrame.Show(true);
				if (m_ItemState) m_ItemState.SetColor(Colors.COLOR_WORN);
				healthName = "Worn";
				break;
			case GameConstants.STATE_DAMAGED:
				m_ItemStateFrame.Show(true);
				if (m_ItemState) m_ItemState.SetColor(Colors.COLOR_DAMAGED);
				healthName = "Damaged";
				break;
			case GameConstants.STATE_BADLY_DAMAGED:
				m_ItemStateFrame.Show(true);
				if (m_ItemState) m_ItemState.SetColor(Colors.COLOR_BADLY_DAMAGED);
				healthName = "Badly Damaged";
				break;
			case GameConstants.STATE_RUINED:
				m_ItemStateFrame.Show(true);
				if (m_ItemState) m_ItemState.SetColor(Colors.COLOR_RUINED);
				healthName = "Ruined";
				break;
			default:
				m_ItemStateFrame.Show(false);
				break;
		}
		
		if (m_HealthText && healthName != ""){
			m_HealthText.SetText(healthName);
		}
	}
	
	protected void UpdateQuantityDisplay(PMConciergeEntry entry){
		if (!m_Quanity) return;
		
		if (entry.m_QuantityMax > 0){
			int qty = entry.m_Quantity;
			int qtyMax = entry.m_QuantityMax;
			m_Quanity.SetText(qty.ToString() + "/" + qtyMax.ToString());
			m_Quanity.Show(true);
		} else {
			m_Quanity.Show(false);
		}
	}
	
	// Click opens item detail view instead of buying directly
	override bool OnClick(Widget w, int x, int y, int button){
		if (w == m_LayoutRoot && m_Parent && m_Entry){
			m_Parent.OpenViewItem(m_Entry);
			return true;
		}
		return super.OnClick(w, x, y, button);
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button){
		if (w == m_LayoutRoot && m_Parent && m_Entry){
			m_Parent.OpenViewItem(m_Entry);
			return true;
		}
		return super.OnMouseButtonUp(w, x, y, button);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y){
		if (w == m_LayoutRoot && m_BG){
			m_BG.SetSize(1.02, 1.02);
		}
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y){
		if (w == m_LayoutRoot && m_BG){
			m_BG.SetSize(1, 1);
		}
		return super.OnMouseLeave(w, enterW, x, y);
	}
}

// Filter button widget
class ConciergeFilterButton extends ScriptedWidgetEventHandler {
	
	protected static const autoptr TStringArray FILTER_LAYOUT_PATH = {"PlayerMarkets/gui/layout/ConciergeFilterButton.layout","PlayerMarkets/gui/layout/modern/ConciergeFilterButton.layout"};
	
	protected Widget m_LayoutRoot;
	protected TextWidget m_FilterText;
	protected ConciergeMenu m_Parent;
	protected int m_Category;
	protected bool m_IsActive = false;
	
	void ConciergeFilterButton(Widget parent, int category, ConciergeMenu menu){
		m_LayoutRoot = Widget.Cast(g_Game.GetWorkspace().CreateWidgets(FILTER_LAYOUT_PATH[GetPMConfig().GUIOption], parent));
		m_Parent = menu;
		m_Category = category;
		
		m_FilterText = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("FilterText"));
		m_FilterText.SetText(PMItemCategoryHelper.GetCategoryName(category));
		
		// Auto-size button width based on text length
		string catName = PMItemCategoryHelper.GetCategoryName(category);
		int btnW = 40 + (catName.Length() * 8);
		if (btnW < 60) btnW = 60;
		m_LayoutRoot.SetSize(btnW, 32);
		
		m_LayoutRoot.SetHandler(this);
	}
	
	void ~ConciergeFilterButton(){
		if (m_LayoutRoot){
			m_LayoutRoot.Show(false);
			delete m_LayoutRoot;
		}
	}
	
	void SetActive(bool active){
		m_IsActive = active;
		if (active){
			m_LayoutRoot.SetColor(ARGBF(0.9, 0.2, 0.6, 0.2));
		} else {
			m_LayoutRoot.SetColor(ARGBF(0.7, 0.3, 0.3, 0.3));
		}
	}
	
	int GetCategory(){
		return m_Category;
	}
	
	override bool OnClick(Widget w, int x, int y, int button){
		if (w == m_LayoutRoot && m_Parent){
			m_Parent.SetFilter(m_Category);
			return true;
		}
		return super.OnClick(w, x, y, button);
	}
}
