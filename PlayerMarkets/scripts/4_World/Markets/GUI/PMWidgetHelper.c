class PMWidgetHelper {
	
	static void UpdateHealthState(Widget itemStateFrame, Widget itemState, EntityAI item){
		if (!itemStateFrame) return;
		if (!itemState){
			itemStateFrame.Show(false);
			return;
		}
		int healthLevel = item.GetHealthLevel("");
		switch (healthLevel) {
			case GameConstants.STATE_RUINED:
				itemStateFrame.Show(true);
				itemState.SetColor(Colors.COLOR_RUINED);
				itemState.SetAlpha(1);
			break;
			case GameConstants.STATE_BADLY_DAMAGED:
				itemStateFrame.Show(true);
				itemState.SetColor(Colors.COLOR_BADLY_DAMAGED);
				itemState.SetAlpha(1);
			break;
			case GameConstants.STATE_DAMAGED:
				itemStateFrame.Show(true);
				itemState.SetColor(Colors.COLOR_DAMAGED);
				itemState.SetAlpha(1);
			break;
			case GameConstants.STATE_WORN:
				itemStateFrame.Show(true);
				itemState.SetColor(Colors.COLOR_WORN);
				itemState.SetAlpha(1);
			break;
			case GameConstants.STATE_PRISTINE:
				itemStateFrame.Show(true);
				itemState.SetColor(Colors.COLOR_PRISTINE);
				itemState.SetAlpha(1);
			break;
			default:
				itemStateFrame.Show(false);
			break;
		}
	}
	
	static void UpdateQuantity(TextWidget quantityWidget, Widget quantityFrame, TextWidget quantityAmount, TextWidget quantityMax, EntityAI item){
		if (quantityWidget) quantityWidget.Show(false);
		if (quantityFrame) quantityFrame.Show(false);
		
		Magazine mag;
		if (Class.CastTo(mag, item)){
			if (quantityWidget){
				quantityWidget.Show(true);
				quantityWidget.SetText( mag.GetAmmoCount().ToString() + "/" + mag.GetAmmoMax().ToString() );
			}
			return;
		}
		
		ItemBase itemB;
		if (!Class.CastTo(itemB, item) || !itemB.HasQuantity()){
			return;
		}
		string text = itemB.GetQuantity().ToString();
		string units = itemB.ConfigGetString("stackedUnit");
		if ((units == "percent" || units == "w") && itemB.GetQuantityMax() > 0){
			float number = itemB.GetQuantity() / itemB.GetQuantityMax();
			int num = number * 100;
			text = num.ToString() + "%";
			if (quantityWidget){
				quantityWidget.Show(true);
				quantityWidget.SetText( text );
			}
		} else if (units != ""){
			if (quantityFrame) quantityFrame.Show(true);
			if (quantityAmount) quantityAmount.SetText(itemB.GetQuantity().ToString() + units);
			text = itemB.GetQuantityMax().ToString() + units;
			if (quantityMax) quantityMax.SetText(text);
		}
	}
	
	static void UpdateItemPreview(ItemPreviewWidget preview, EntityAI item){
		InventoryItem iItem = InventoryItem.Cast(item);
		if (iItem && preview){
			preview.SetItem( iItem );
			preview.SetModelPosition( Vector(0,0,0) );
			preview.SetModelOrientation( Vector(0,0,0) );
			preview.SetPos( 0, 0);
			preview.SetSize( 1, 1);
			preview.SetView( 0 );
		}
	}
}
