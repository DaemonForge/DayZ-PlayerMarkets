// Singleton manager for market areas - finds stalls in areas and provides aggregated data
class MarketAreaManager {
	
	private static ref MarketAreaManager s_Instance;
	
	static MarketAreaManager GetInstance(){
		if (!s_Instance){
			s_Instance = new MarketAreaManager();
		}
		return s_Instance;
	}
	
	// Find which market area a position is inside of (returns NULL if none)
	PMMarketArea GetMarketAreaAtPosition(vector pos){
		if (!GetPMConfig() || !GetPMConfig().MarketAreas) return NULL;
		
		for (int i = 0; i < GetPMConfig().MarketAreas.Count(); i++){
			PMMarketArea area = GetPMConfig().MarketAreas.Get(i);
			if (area && area.IsPositionInside(pos)){
				return area;
			}
		}
		return NULL;
	}
	
	// Get all market stands within a market area
	array<MarketStandBase> GetStallsInArea(PMMarketArea area){
		array<MarketStandBase> stalls = new array<MarketStandBase>;
		if (!area) return stalls;
		
		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition(area.CenterPosition, area.Radius, objects, proxyCargos);
		
		foreach (Object obj : objects){
			MarketStandBase stall = MarketStandBase.Cast(obj);
			if (stall && stall.IsBuilt() && area.IsPositionInside(stall.GetPosition())){
				stalls.Insert(stall);
			}
		}
		return stalls;
	}
	
	// Get the tax modifier for a stall based on its position
	float GetTaxModifierForPosition(vector pos){
		PMMarketArea area = GetMarketAreaAtPosition(pos);
		if (area){
			return area.TaxModifier;
		}
		return 0.0;
	}
	
	// Build the full concierge listing for an area (server-side only)
	array<ref PMConciergeEntry> BuildConciergeListings(PMMarketArea area){
		array<ref PMConciergeEntry> entries = new array<ref PMConciergeEntry>;
		if (!area) return entries;
		
		array<MarketStandBase> stalls = GetStallsInArea(area);
		
		foreach (MarketStandBase stall : stalls){
			if (!stall.IsBuilt()) continue;
			
			stall.InitStandData();
			
			array<autoptr PlayerMarketItemDetails> itemsArray = stall.GetItemsArray();
			if (!itemsArray || itemsArray.Count() == 0) continue;
			
			// Get stall persistent ID
			int sb1, sb2, sb3, sb4;
			stall.GetPersistentID(sb1, sb2, sb3, sb4);
			
			string stallName = stall.GetStandName();
			string currency = stall.GetCurrencyUsed();
			float taxMod = area.TaxModifier;
			
			for (int i = 0; i < itemsArray.Count(); i++){
				PlayerMarketItemDetailsBase detail = itemsArray.Get(i);
				if (!detail) continue;
				
				EntityAI item = detail.GetItem();
				int category = PMItemCategory.OTHER;
				if (item){
					category = PMItemCategoryHelper.GetItemCategory(item);
				} else {
					category = PMItemCategoryHelper.GetItemCategoryByClassName(detail.GetItemClass());
				}
				
				int ib1, ib2, ib3, ib4;
				detail.GetIds(ib1, ib2, ib3, ib4);
				
				PMConciergeEntry entry = new PMConciergeEntry();
				entry.SetStallInfo(sb1, sb2, sb3, sb4, stallName, currency, taxMod);
				entry.SetItemInfo(ib1, ib2, ib3, ib4, detail.GetItemClass(), detail.GetItemName(), detail.GetPrice(), category);
				
				// Populate item condition details
				int healthLevel = -1;
				float qty = 0;
				float qtyMax = 0;
				float weight = 0;
				int liquidType = 0;
				if (item){
					healthLevel = item.GetHealthLevel("");
					weight = item.GetWeight();
					ItemBase itemBase;
					if (Class.CastTo(itemBase, item)){
						if (itemBase.HasQuantity()){
							qty = itemBase.GetQuantity();
							qtyMax = itemBase.GetQuantityMax();
						}
						liquidType = itemBase.GetLiquidType();
					}
					Magazine mag;
					if (Class.CastTo(mag, item)){
						qty = mag.GetAmmoCount();
						qtyMax = mag.GetAmmoMax();
					}
				}
				entry.SetItemDetails(healthLevel, qty, qtyMax, weight, liquidType);
				
				// Store network ID for client-side entity lookup
				if (item){
					int netLow, netHigh;
					item.GetNetworkID(netLow, netHigh);
					entry.SetItemNetworkID(netLow, netHigh);
				}
				
				// Collect attachment class names for preview
				if (item){
					TStringArray attachNames = new TStringArray;
					for (int a = 0; a < item.GetInventory().AttachmentCount(); a++){
						EntityAI att = item.GetInventory().GetAttachmentFromIndex(a);
						if (att){
							attachNames.Insert(att.GetType());
						}
					}
					if (attachNames.Count() > 0){
						entry.SetAttachments(attachNames);
					}
				}
				
				entries.Insert(entry);
			}
		}
		return entries;
	}
	
	// Server-side: find a stall by its persistent ID
	MarketStandBase FindStallByPersistentID(PMMarketArea area, int b1, int b2, int b3, int b4){
		if (!area) return NULL;
		
		array<MarketStandBase> stalls = GetStallsInArea(area);
		foreach (MarketStandBase stall : stalls){
			int sb1, sb2, sb3, sb4;
			stall.GetPersistentID(sb1, sb2, sb3, sb4);
			if (sb1 == b1 && sb2 == b2 && sb3 == b3 && sb4 == b4){
				return stall;
			}
		}
		return NULL;
	}
	
	// Client-side: find the actual item entity for a concierge entry using network ID
	EntityAI FindActualItemForEntry(PMConciergeEntry entry){
		if (!entry) return NULL;
		if (entry.m_ItemNetLow == 0 && entry.m_ItemNetHigh == 0) return NULL;
		
		Object obj = GetGame().GetObjectByNetworkId(entry.m_ItemNetLow, entry.m_ItemNetHigh);
		if (obj){
			return EntityAI.Cast(obj);
		}
		return NULL;
	}
}

static MarketAreaManager GetMarketAreaManager(){
	return MarketAreaManager.GetInstance();
}
