class MarketStallItemView  extends ScriptedWidgetEventHandler {
	protected const autoptr TStringArray ITEM_LAYOUT_PATH = {"PlayerMarkets/gui/layout/MarketStallItemView.layout","PlayerMarkets/gui/layout/modern/MarketStallItemView.layout"};
	protected autoptr PlayerMarketItemDetails m_ItemDetails;
	
	private vector              m_PreviewWidgetOrientation;	
    private int                 m_PreviewWidgetRotationX;
	private int                 m_PreviewWidgetRotationY;
	
	protected autoptr MarketStallMenu m_parent;
	protected Widget m_LayoutRoot;
	
	protected ItemPreviewWidget m_ItemPreview;
	protected TextWidget m_DisplayName;
	protected MultilineTextWidget m_Description;
	protected Widget m_QuanityFrame;
	protected TextWidget m_QuanityAmount;
	protected TextWidget m_QuanityMax;
	protected TextWidget m_Quanity;
	protected TextWidget m_State;
	protected TextWidget m_Price;
	protected TextWidget m_Weight;
	protected TextWidget m_LiquidType;
	protected Widget m_ItemStateFrame;
	protected Widget m_ItemState;
	
	protected ButtonWidget m_Buy;
	protected ButtonWidget m_Cancel;
	
	void MarketStallItemView(Widget parent, PlayerMarketItemDetails details, MarketStallMenu menu ){
		m_LayoutRoot = Widget.Cast(GetGame().GetWorkspace().CreateWidgets(ITEM_LAYOUT_PATH[GetPMConfig().GUIOption],parent));
		m_parent = MarketStallMenu.Cast(menu);
		m_ItemDetails = PlayerMarketItemDetails.Cast(details);
		
		m_ItemPreview = ItemPreviewWidget.Cast(m_LayoutRoot.FindAnyWidget("ItemPreveiw"));
		m_DisplayName = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("ItemName"));
		m_Description =  MultilineTextWidget.Cast(m_LayoutRoot.FindAnyWidget("Description"));
		m_QuanityFrame = Widget.Cast(m_LayoutRoot.FindAnyWidget("QuanityFrame"));
		m_QuanityAmount = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("QuanityAmount"));
		m_Quanity = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Quanity"));
		m_QuanityMax = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("QuanityMax"));
		m_State = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("DamageState"));
		m_Price = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Price"));
		m_Weight = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Weight"));
		m_ItemStateFrame = Widget.Cast(m_LayoutRoot.FindAnyWidget("ItemStateFrame"));
		m_ItemState = Widget.Cast(m_LayoutRoot.FindAnyWidget("ItemState"));
		m_LiquidType = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("LiquidType"));
		
		m_Buy = ButtonWidget.Cast(m_LayoutRoot.FindAnyWidget("Buy"));
		m_Cancel = ButtonWidget.Cast(m_LayoutRoot.FindAnyWidget("Cancel"));
		
		EntityAI item = m_ItemDetails.GetItem();
		//0 = pristine, 1 = worn, 2 = damaged, 3 = badly damaged, 4 = ruined
		m_DisplayName.SetText(item.GetDisplayName());
		
		int price = details.GetPrice();
		if (GetPMConfig().SaleTaxAmount > 0){
			price+= price * GetPMConfig().SaleTaxAmount;
		}
		m_Price.SetText("$" + UUtil.ConvertIntToNiceString(price));
		string description = "";
		UUtil.GetConfigString(item.GetType(),"descriptionShort",description );
		m_Description.SetText(description);
		int healthLevel = item.GetHealthLevel("");
		m_Weight.SetText(item.GetWeight().ToString() + "g");
		if (item.GetWeight() > 1450){
			float weight = Math.Round(item.GetWeight() / 100) / 10;
			m_Weight.SetText(weight.ToString() + "kg");
		}
		switch  (healthLevel) {
			case GameConstants.STATE_RUINED:
				m_ItemStateFrame.Show(true);
				m_ItemState.SetColor(Colors.COLOR_RUINED);
				m_ItemState.SetAlpha(1);
			break;
			case GameConstants.STATE_BADLY_DAMAGED:
				m_ItemStateFrame.Show(true);
				m_ItemState.SetColor(Colors.COLOR_BADLY_DAMAGED);
				m_ItemState.SetAlpha(1);
			break;
			case GameConstants.STATE_DAMAGED:
				m_ItemStateFrame.Show(true);
				m_ItemState.SetColor(Colors.COLOR_DAMAGED);
				m_ItemState.SetAlpha(1);
			break;
			case GameConstants.STATE_WORN:
				m_ItemStateFrame.Show(true);
				m_ItemState.SetColor(Colors.COLOR_WORN);
				m_ItemState.SetAlpha(1);
			break;
			case GameConstants.STATE_PRISTINE:
				m_ItemStateFrame.Show(true);
				m_ItemState.SetColor(Colors.COLOR_PRISTINE);
				m_ItemState.SetAlpha(1);
			break;
			default:
				m_ItemStateFrame.Show(false);
			break;
		}
		m_Quanity.Show(false);
		UpdateQuanity(item);
		UpdateItemPreviw(item);
		
		m_LayoutRoot.SetHandler(this);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.UpdateItemView,400, true);
	}
	
	
	void ~MarketStallItemView(){
		m_LayoutRoot.Show(false);
		GetGame().GetDragQueue().RemoveCalls(this);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.UpdateItemView);
		delete m_LayoutRoot;
	}
	
    override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonDown(w, x, y, button);
		
		if (w == m_ItemPreview)
		{
			GetGame().GetDragQueue().Call(this, "UpdateRotation");
			GetMousePos(m_PreviewWidgetRotationX, m_PreviewWidgetRotationY);
			return true;
		}
		return false;
	};
	
	void UpdateRotation(int mouse_x, int mouse_y, bool is_dragging)
	{
		vector o = m_PreviewWidgetOrientation;
		o[0] = o[0] + (m_PreviewWidgetRotationY - mouse_y);
		o[1] = o[1] - (m_PreviewWidgetRotationX - mouse_x);
		
		m_ItemPreview.SetModelOrientation( o );
		
		if (!is_dragging)
		{
			m_PreviewWidgetOrientation = o;
		};
	};
	
	override bool OnClick(Widget w, int x, int y, int button){
		
		if (w == m_Buy && m_parent && m_parent.GetStand() && m_parent.GetPlayerBalance() >= m_ItemDetails.GetPrice()){
			m_parent.GetStand().RequestBuy(m_ItemDetails);
			Close();
			return true;
		}
		if (w == m_Cancel){
			Close();
			return true;
		}
		
		return super.OnClick(w,x,y,button);
	
	}
	
	
	void Close(){
		m_parent.CloseViewItem();
		m_LayoutRoot.Show(false);
	}	
	
	void UpdateItemView()
	{
		if (m_parent){
			if (m_parent.GetPlayerBalance() < m_ItemDetails.GetPrice()){
				m_Buy.SetAlpha(0.3);
			} else {
				m_Buy.SetAlpha(1);
			}
		}
	}
	
	
	
	void UpdateItemPreviw(EntityAI item){
		InventoryItem iItem = InventoryItem.Cast(item);
		if (iItem){
			m_ItemPreview.SetItem( iItem );
			m_ItemPreview.SetModelPosition( Vector(0,0,0) );
			m_PreviewWidgetOrientation = Vector(0,0,0);
			m_ItemPreview.SetModelOrientation( m_PreviewWidgetOrientation );
			m_ItemPreview.SetPos( 0, 0);
			m_ItemPreview.SetSize( 1, 1);
			m_ItemPreview.SetView( 0 );
		}
	}
	
	
	void UpdateLiquidType(int type){		
		switch(type)
		{
			case LIQUID_WATER:
			{
				m_LiquidType.SetText("#inv_inspect_water");//Colors.COLOR_LIQUID);
				m_LiquidType.Show(true);	
				break;
			}
				
			case LIQUID_RIVERWATER:
			{
				m_LiquidType.SetText("#inv_inspect_river_water");//Colors.COLOR_LIQUID);
				m_LiquidType.Show(true);	
				break;
			}
				
			case LIQUID_VODKA:
			{
				m_LiquidType.SetText("#inv_inspect_vodka");//Colors.COLOR_LIQUID);
				m_LiquidType.Show(true);	
				break;
			}
			
			case LIQUID_BEER:
			{
				m_LiquidType.SetText("#inv_inspect_beer");//Colors.COLOR_LIQUID);
				m_LiquidType.Show(true);	
				break;
			}
		
			case LIQUID_GASOLINE:
			{
				m_LiquidType.SetText("#inv_inspect_gasoline");//Colors.COLOR_LIQUID);
				m_LiquidType.Show(true);	
				break;
			}
			
			case LIQUID_DIESEL:
			{
				m_LiquidType.SetText("#inv_inspect_diesel");//Colors.COLOR_LIQUID);
				m_LiquidType.Show(true);	
				break;
			}
			
			case LIQUID_DISINFECTANT:
			{
				m_LiquidType.SetText("#inv_inspect_disinfectant");// Colors.COLOR_LIQUID);
				m_LiquidType.Show(true);	
				break;
			}

			case LIQUID_SALINE:
			{
				m_LiquidType.SetText("#inv_inspect_saline");// Colors.COLOR_LIQUID);
				m_LiquidType.Show(true);	
				break;
			}
			
			default:
			{
				m_LiquidType.SetText("ERROR");// Colors.COLOR_LIQUID);
				break;
			}
		}
	}
	
	void UpdateQuanity(EntityAI item){
		Magazine mag;
		if (Class.CastTo(mag, item)){
			m_Quanity.Show(true);
			m_Quanity.SetText( mag.GetAmmoCount().ToString() + "/" + mag.GetAmmoMax().ToString() );
			return;
		}
		
		ItemBase itemB;
		if (!Class.CastTo(itemB, item) || !itemB.HasQuantity()){
			return;
		}
		UpdateLiquidType(itemB.GetLiquidType());
		string text = itemB.GetQuantity().ToString();
		string units = itemB.ConfigGetString("stackedUnit");
		if ((units == "percent" || units == "w") && itemB.GetQuantityMax() > 0){
			float number = itemB.GetQuantity() / itemB.GetQuantityMax();
			int num = number * 100;
			text = num.ToString() + "%";
			m_Quanity.Show(true);
			m_Quanity.SetText( text );
		} else if (units != ""){
			m_QuanityFrame.Show(true);
			m_QuanityAmount.SetText(itemB.GetQuantity().ToString() + units);
			text = itemB.GetQuantityMax().ToString() + units;
			m_QuanityMax.SetText(text);
		}
	}
	
		
}