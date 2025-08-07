class MarketStallSetPriceWidget extends ScriptedWidgetEventHandler {
	protected const autoptr TStringArray ITEM_LAYOUT_PATH = {"PlayerMarkets/gui/layout/SimpleSetPrice.layout","PlayerMarkets/gui/layout/modern/SimpleSetPrice.layout"};
	
	protected autoptr MarketStallSellerMenu m_parent;
	protected Widget m_LayoutRoot;
	
	protected EditBoxWidget m_Price;
	protected ButtonWidget m_Set;
	protected ButtonWidget m_Close;
	protected TextWidget m_Tax;
	
	protected EntityAI m_Item
	
	
	void MarketStallSetPriceWidget(Widget parent, EntityAI item, MarketStallSellerMenu menu ){
		m_LayoutRoot = Widget.Cast(GetGame().GetWorkspace().CreateWidgets(ITEM_LAYOUT_PATH[GetPMConfig().GUIOption],parent));
		m_parent = MarketStallSellerMenu.Cast(menu);
		Class.CastTo(m_Item,item);
		
		m_Price 	= EditBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("Price"));
		m_Set 		= ButtonWidget.Cast(m_LayoutRoot.FindAnyWidget("Set"));
		m_Close 	= ButtonWidget.Cast(m_LayoutRoot.FindAnyWidget("Close"));
		m_Tax 		= TextWidget.Cast(m_LayoutRoot.FindAnyWidget("Tax"));
		
		if (GetPMConfig().SaleTaxAmount > 0 && m_Tax){
			m_Tax.Show(true);
			int tax = Math.Round(GetPMConfig().SaleTaxAmount * 100);
			m_Tax.SetText("+" + tax.ToString() + "% Tax");
		}
		
		m_LayoutRoot.SetHandler(this);
	}
	
	void ~MarketStallSetPriceWidget(){
		delete m_LayoutRoot;
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button){
		Print(m_Item);
		if (w == m_Set && m_parent && m_parent.GetStand() && m_Item){
			string number = CheckInput(m_Price.GetText());
			m_parent.GetStand().RequestList(new PlayerMarketItemDetails(m_Item, number.ToInt(), NULL));
			
			Close();
			return true;
		}
		if (w == m_Close && m_parent){
			Close();
			return true;
		}
		return super.OnClick(w,x,y,button);
	
	}
	
	
	override bool OnChange(Widget w, int x, int y, bool finished){
		
		if (w == m_Price){
			string number = CheckInput(m_Price.GetText());
			m_Price.SetText(number);
		}
		
		return super.OnChange(w,x,y,finished);
	}
	
	
	void Close() {
		m_LayoutRoot.Show(false);
		m_parent.CloseSetPrice();
	}
	
	
	string CheckInput(string input) {
		string amount = "";
		for (int i = 0; i < input.Length(); i++){
			if (ValidInput(input.Get(i))){
				amount = amount + input.Get(i);
			}
		}
		return amount;
    }
	
	bool ValidInput(string input)
    {
		if (input.Contains("0") || input.Contains("1") || input.Contains("2") || input.Contains("3") || input.Contains("4") || input.Contains("5") || input.Contains("6") || input.Contains("7") || input.Contains("8") || input.Contains("9") || input.Contains(".")){
			return true;
		}
		return false;
    }
	
}