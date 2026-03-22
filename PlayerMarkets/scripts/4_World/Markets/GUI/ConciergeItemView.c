// Detail inspection panel for a concierge item
class ConciergeItemView extends ScriptedWidgetEventHandler {
	
	protected static const autoptr TStringArray ITEM_VIEW_LAYOUT_PATH = {"PlayerMarkets/gui/layout/ConciergeItemView.layout","PlayerMarkets/gui/layout/modern/ConciergeItemView.layout"};
	
	protected ref PMConciergeEntry m_Entry;
	protected ConciergeMenu m_Parent;
	protected Widget m_LayoutRoot;
	
	protected ItemPreviewWidget m_ItemPreview;
	protected TextWidget m_ItemName;
	protected TextWidget m_DamageState;
	protected MultilineTextWidget m_Description;
	protected TextWidget m_Quanity;
	protected Widget m_QuanityFrame;
	protected TextWidget m_QuanityAmount;
	protected TextWidget m_QuanityMax;
	protected TextWidget m_Weight;
	protected TextWidget m_LiquidType;
	protected TextWidget m_StallInfo;
	protected TextWidget m_Price;
	protected Widget m_ItemStateFrame;
	protected Widget m_ItemState;
	protected ButtonWidget m_Buy;
	protected ButtonWidget m_Cancel;
	
	private vector m_PreviewOrientation;
	private int m_PreviewRotationX;
	private int m_PreviewRotationY;
	
	void ConciergeItemView(Widget parent, PMConciergeEntry entry, ConciergeMenu menu){
		m_LayoutRoot = Widget.Cast(g_Game.GetWorkspace().CreateWidgets(ITEM_VIEW_LAYOUT_PATH[GetPMConfig().GUIOption], parent));
		m_Parent = menu;
		m_Entry = entry;
		
		m_ItemPreview = ItemPreviewWidget.Cast(m_LayoutRoot.FindAnyWidget("ItemPreview"));
		m_ItemName = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("ItemName"));
		m_DamageState = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("DamageState"));
		m_Description = MultilineTextWidget.Cast(m_LayoutRoot.FindAnyWidget("Description"));
		m_Quanity = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Quanity"));
		m_QuanityFrame = Widget.Cast(m_LayoutRoot.FindAnyWidget("QuanityFrame"));
		m_QuanityAmount = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("QuanityAmount"));
		m_QuanityMax = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("QuanityMax"));
		m_Weight = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Weight"));
		m_LiquidType = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("LiquidType"));
		m_StallInfo = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("StallInfo"));
		m_Price = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Price"));
		m_ItemStateFrame = Widget.Cast(m_LayoutRoot.FindAnyWidget("ItemStateFrame"));
		m_ItemState = Widget.Cast(m_LayoutRoot.FindAnyWidget("ItemState"));
		m_Buy = ButtonWidget.Cast(m_LayoutRoot.FindAnyWidget("Buy"));
		m_Cancel = ButtonWidget.Cast(m_LayoutRoot.FindAnyWidget("Cancel"));
		
		// Populate display
		m_ItemName.SetText(entry.m_ItemName);
		
		int totalPrice = entry.GetPriceWithTax();
		m_Price.SetText("$" + UUtil.ConvertIntToNiceString(totalPrice));
		
		if (m_StallInfo){
			m_StallInfo.SetText("Sold by: " + entry.m_StallName);
		}
		
		// Description from config
		string description = "";
		UUtil.GetConfigString(entry.m_ItemClass, "descriptionShort", description);
		if (m_Description){
			m_Description.SetText(description);
		}
		
		// Health state from synced data
		UpdateHealthDisplay(entry.m_HealthLevel);
		
		// Quantity from synced data
		UpdateQuantityDisplay(entry);
		
		// Weight display
		UpdateWeightDisplay(entry.m_Weight);
		
		// Liquid type
		UpdateLiquidDisplay(entry.m_LiquidType);
		
		// Item preview from class name
		if (entry.m_ItemClass != ""){
			EntityAI previewItem = EntityAI.Cast(GetGame().CreateObjectEx(entry.m_ItemClass, "0 0 0", ECE_LOCAL | ECE_NOLIFETIME));
			if (previewItem && m_ItemPreview){
				InventoryItem iItem = InventoryItem.Cast(previewItem);
				if (iItem){
					m_ItemPreview.SetItem(iItem);
					m_ItemPreview.SetModelPosition(Vector(0,0,0));
					m_PreviewOrientation = Vector(0,0,0);
					m_ItemPreview.SetModelOrientation(m_PreviewOrientation);
					m_ItemPreview.SetPos(0, 0);
					m_ItemPreview.SetSize(1, 1);
					m_ItemPreview.SetView(0);
				}
				GetGame().ObjectDelete(previewItem);
			}
		}
		
		m_LayoutRoot.SetHandler(this);
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.PMOnUpdate, 400, true);
	}
	
	void ~ConciergeItemView(){
		g_Game.GetDragQueue().RemoveCalls(this);
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.PMOnUpdate);
		if (m_LayoutRoot){
			m_LayoutRoot.Show(false);
			delete m_LayoutRoot;
		}
	}
	
	protected void UpdateHealthDisplay(int healthLevel){
		string healthName = "";
		int healthColor = 0;
		
		switch (healthLevel){
			case GameConstants.STATE_PRISTINE:
				healthColor = Colors.COLOR_PRISTINE;
				healthName = "Pristine";
				break;
			case GameConstants.STATE_WORN:
				healthColor = Colors.COLOR_WORN;
				healthName = "Worn";
				break;
			case GameConstants.STATE_DAMAGED:
				healthColor = Colors.COLOR_DAMAGED;
				healthName = "Damaged";
				break;
			case GameConstants.STATE_BADLY_DAMAGED:
				healthColor = Colors.COLOR_BADLY_DAMAGED;
				healthName = "Badly Damaged";
				break;
			case GameConstants.STATE_RUINED:
				healthColor = Colors.COLOR_RUINED;
				healthName = "Ruined";
				break;
			default:
				break;
		}
		
		if (m_ItemStateFrame && healthName != ""){
			m_ItemStateFrame.Show(true);
			if (m_ItemState) m_ItemState.SetColor(healthColor);
		} else if (m_ItemStateFrame){
			m_ItemStateFrame.Show(false);
		}
		
		if (m_DamageState){
			m_DamageState.SetText(healthName);
		}
	}
	
	protected void UpdateQuantityDisplay(PMConciergeEntry entry){
		if (entry.m_QuantityMax > 0){
			int qty = entry.m_Quantity;
			int qtyMax = entry.m_QuantityMax;
			
			// Show fraction display if available
			if (m_QuanityFrame){
				m_QuanityFrame.Show(true);
				if (m_QuanityAmount) m_QuanityAmount.SetText(qty.ToString());
				if (m_QuanityMax) m_QuanityMax.SetText(qtyMax.ToString());
			}
			
			// Also set simple quantity text
			if (m_Quanity){
				m_Quanity.SetText(qty.ToString() + "/" + qtyMax.ToString());
				m_Quanity.Show(true);
			}
		} else {
			if (m_QuanityFrame) m_QuanityFrame.Show(false);
			if (m_Quanity) m_Quanity.Show(false);
		}
	}
	
	protected void UpdateWeightDisplay(float weight){
		if (!m_Weight) return;
		
		if (weight > 0){
			if (weight > 1450){
				float kgWeight = Math.Round(weight / 100) / 10;
				m_Weight.SetText(kgWeight.ToString() + "kg");
			} else {
				int gWeight = weight;
				m_Weight.SetText(gWeight.ToString() + "g");
			}
			m_Weight.Show(true);
		} else {
			m_Weight.Show(false);
		}
	}
	
	protected void UpdateLiquidDisplay(int liquidType){
		if (!m_LiquidType) return;
		
		if (liquidType <= 0){
			m_LiquidType.Show(false);
			return;
		}
		
		switch (liquidType){
			case LIQUID_WATER:
				m_LiquidType.SetText("#inv_inspect_water");
				break;
			case LIQUID_RIVERWATER:
				m_LiquidType.SetText("#inv_inspect_river_water");
				break;
			case LIQUID_VODKA:
				m_LiquidType.SetText("#inv_inspect_vodka");
				break;
			case LIQUID_BEER:
				m_LiquidType.SetText("#inv_inspect_beer");
				break;
			case LIQUID_GASOLINE:
				m_LiquidType.SetText("#inv_inspect_gasoline");
				break;
			case LIQUID_DIESEL:
				m_LiquidType.SetText("#inv_inspect_diesel");
				break;
			case LIQUID_DISINFECTANT:
				m_LiquidType.SetText("#inv_inspect_disinfectant");
				break;
			case LIQUID_SALINE:
				m_LiquidType.SetText("#inv_inspect_saline");
				break;
			default:
				m_LiquidType.Show(false);
				return;
		}
		m_LiquidType.Show(true);
	}
	
	// Drag-to-rotate preview
	override bool OnMouseButtonDown(Widget w, int x, int y, int button){
		super.OnMouseButtonDown(w, x, y, button);
		if (w == m_ItemPreview){
			g_Game.GetDragQueue().Call(this, "UpdateRotation");
			GetMousePos(m_PreviewRotationX, m_PreviewRotationY);
			return true;
		}
		return false;
	}
	
	void UpdateRotation(int mouse_x, int mouse_y, bool is_dragging){
		vector o = m_PreviewOrientation;
		o[0] = o[0] + (m_PreviewRotationY - mouse_y);
		o[1] = o[1] - (m_PreviewRotationX - mouse_x);
		m_ItemPreview.SetModelOrientation(o);
		if (!is_dragging){
			m_PreviewOrientation = o;
		}
	}
	
	override bool OnClick(Widget w, int x, int y, int button){
		if (w == m_Buy && m_Parent && m_Entry){
			m_Parent.OnBuyItem(m_Entry);
			return true;
		}
		if (w == m_Cancel && m_Parent){
			m_Parent.CloseViewItem();
			return true;
		}
		return super.OnClick(w, x, y, button);
	}
	
	// Periodically dim Buy button if player can't afford item
	void PMOnUpdate(){
		if (!m_Parent || !m_Entry || !m_Buy) return;
		
		int balance = m_Parent.GetPlayerBalance(m_Entry.m_CurrencyUsed);
		int totalPrice = m_Entry.GetPriceWithTax();
		
		if (balance < totalPrice){
			m_Buy.SetAlpha(0.3);
		} else {
			m_Buy.SetAlpha(1);
		}
	}
}
