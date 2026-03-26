// Market Area definition - defines a geographic zone with tax modifiers and concierge support
class PMMarketArea extends Managed {
	string Name = "Market District";
	vector CenterPosition = "0 0 0";
	float Radius = 50.0;
	float TaxModifier = 0.0; // Negative = tax reduction, Positive = extra tax. e.g. -0.02 = 2% discount, 0.05 = 5% extra
	vector ConciergePosition = "0 0 0"; // If non-zero, auto-spawn a concierge here
	string NPCClassName = "PM_ConciergeM_Mirek"; // Class name for the NPC model (see config.cpp for available classes)
	ref TStringArray NPCGear = {"DressShirt_Beige", "SlacksPants_DarkGrey", "DressShoes_Black", "FlatCap_Grey"};
	float ConciergeDirection = 225.0; // Compass heading the NPC faces (0=North, 90=East, 180=South, 270=West)
	
	bool IsPositionInside(vector pos){
		float dist = vector.Distance(CenterPosition, pos);
		return (dist <= Radius);
	}
	
	float GetEffectiveSaleTax(){
		float baseTax = GetPMConfig().SaleTaxAmount;
		return Math.Clamp(baseTax + TaxModifier, 0.0, 1.0);
	}
}

// Item category enum for filtering in the concierge menu
enum PMItemCategory {
	ALL = 0,
	WEAPONS = 1,
	AMMO = 2,
	CLOTHING = 3,
	FOOD = 4,
	MEDICAL = 5,
	CONTAINERS = 6,
	ATTACHMENTS = 7,
	TOOLS = 8,
	MELEE = 9,
	OTHER = 10
}

class PMItemCategoryHelper {
	
	static string GetCategoryName(int category){
		switch(category){
			case PMItemCategory.ALL: return "All";
			case PMItemCategory.WEAPONS: return "Weapons";
			case PMItemCategory.AMMO: return "Ammo";
			case PMItemCategory.CLOTHING: return "Clothing";
			case PMItemCategory.FOOD: return "Food & Drink";
			case PMItemCategory.MEDICAL: return "Medical";
			case PMItemCategory.CONTAINERS: return "Containers";
			case PMItemCategory.ATTACHMENTS: return "Attachments";
			case PMItemCategory.TOOLS: return "Tools";
			case PMItemCategory.MELEE: return "Melee";
			case PMItemCategory.OTHER: return "Other";
		}
		return "Unknown";
	}
	
	static int GetCategoryCount(){
		return 11;
	}
	
	static int GetItemCategory(EntityAI item){
		if (!item) return PMItemCategory.OTHER;
		
		// Firearms (Weapon_Base only)
		if (item.IsWeapon()){
			return PMItemCategory.WEAPONS;
		}
		
		// Ammo & magazines
		if (item.IsAmmoPile() || item.IsKindOf("Magazine_Base")){
			return PMItemCategory.AMMO;
		}
		
		// Weapon attachments — before clothing (some attachable items could overlap)
		if (item.IsKindOf("ItemOptics") || item.IsKindOf("ItemSuppressor") || item.IsKindOf("HandGuard_Base") || item.IsKindOf("ButtStock_Base") || item.IsKindOf("Hndgrd_Base")){
			return PMItemCategory.ATTACHMENTS;
		}
		
		// Clothing — BEFORE melee (shoes/gloves have isMeleeWeapon=1 in config)
		if (item.IsClothing()){
			return PMItemCategory.CLOTHING;
		}
		
		// Food & drink
		if (item.IsKindOf("Edible_Base") || item.IsKindOf("Bottle_Base") || item.IsKindOf("Canteen_Base")){
			return PMItemCategory.FOOD;
		}
		
		// Medical items
		if (item.IsKindOf("Bandage_Basic") || item.IsKindOf("BandageDressing") || item.IsKindOf("Morphine") || item.IsKindOf("Epinephrine") || item.IsKindOf("BloodBagEmpty") || item.IsKindOf("BloodBagFull") || item.IsKindOf("SalineBag") || item.IsKindOf("Splint")){
			return PMItemCategory.MEDICAL;
		}
		if (item.IsKindOf("TetracyclineAntibiotics") || item.IsKindOf("CharcoalTablets") || item.IsKindOf("PainkillerTablets") || item.IsKindOf("VitaminBottle") || item.IsKindOf("DisinfectantSpray") || item.IsKindOf("IodineTincture") || item.IsKindOf("DisinfectantAlcohol") || item.IsKindOf("Thermometer") || item.IsKindOf("BloodTestKit")){
			return PMItemCategory.MEDICAL;
		}
		
		// Tools — BEFORE melee (axes/hammers have isMeleeWeapon=1)
		if (item.IsKindOf("Shovel") || item.IsKindOf("FarmingHoe") || item.IsKindOf("Pickaxe") || item.IsKindOf("Pliers") || item.IsKindOf("Hacksaw") || item.IsKindOf("HandSaw") || item.IsKindOf("Hammer") || item.IsKindOf("Wrench") || item.IsKindOf("Crowbar") || item.IsKindOf("SledgeHammer")){
			return PMItemCategory.TOOLS;
		}
		if (item.IsKindOf("Hatchet") || item.IsKindOf("WoodAxe") || item.IsKindOf("FirefighterAxe") || item.IsKindOf("CombinationLock") || item.IsKindOf("Lockpick") || item.IsKindOf("Screwdriver") || item.IsKindOf("CanOpener") || item.IsKindOf("SewingKit") || item.IsKindOf("LeatherSewingKit") || item.IsKindOf("Whetstone")){
			return PMItemCategory.TOOLS;
		}
		
		// Containers — items with cargo that aren't already categorized above
		if (item.GetInventory()){
			if (item.GetInventory().GetCargo()){
				return PMItemCategory.CONTAINERS;
			}
		}
		
		// Melee weapons — dedicated melee only (reached after clothing/tools/food are ruled out)
		if (item.IsMeleeWeapon()){
			return PMItemCategory.MELEE;
		}
		
		return PMItemCategory.OTHER;
	}
	
	// Categorize by class name when entity isn't available (client-side)
	static int GetItemCategoryByClassName(string className){
		if (className == "") return PMItemCategory.OTHER;
		
		// Firearms
		if (GetGame().IsKindOf(className, "Weapon_Base")){
			return PMItemCategory.WEAPONS;
		}
		
		// Ammo & magazines
		if (GetGame().IsKindOf(className, "Magazine_Base") || GetGame().IsKindOf(className, "Ammunition_Base")){
			return PMItemCategory.AMMO;
		}
		
		// Weapon attachments
		if (GetGame().IsKindOf(className, "ItemOptics") || GetGame().IsKindOf(className, "ItemSuppressor")){
			return PMItemCategory.ATTACHMENTS;
		}
		
		// Clothing — before melee
		if (GetGame().IsKindOf(className, "Clothing_Base")){
			return PMItemCategory.CLOTHING;
		}
		
		// Food & drink
		if (GetGame().IsKindOf(className, "Edible_Base") || GetGame().IsKindOf(className, "Bottle_Base")){
			return PMItemCategory.FOOD;
		}
		
		// Melee weapons (class inheritance — only catches dedicated MeleeWeapon subclasses)
		if (GetGame().IsKindOf(className, "MeleeWeapon")){
			return PMItemCategory.MELEE;
		}
		
		return PMItemCategory.OTHER;
	}
}

// Data entry for a single item in the concierge listing
class PMConciergeEntry extends Managed {
	int m_StallB1, m_StallB2, m_StallB3, m_StallB4;
	string m_StallName;
	string m_CurrencyUsed;
	float m_AreaTaxModifier;
	
	int m_ItemB1, m_ItemB2, m_ItemB3, m_ItemB4;
	string m_ItemClass;
	string m_ItemName;
	int m_Price;
	int m_Category;
	int m_HealthLevel;
	float m_Quantity;
	float m_QuantityMax;
	float m_Weight;
	int m_LiquidType;
	int m_ItemNetLow;
	int m_ItemNetHigh;
	ref TStringArray m_Attachments;
	
	void PMConciergeEntry(){
		m_Attachments = new TStringArray;
	}
	
	void SetStallInfo(int b1, int b2, int b3, int b4, string stallName, string currency, float taxMod){
		m_StallB1 = b1;
		m_StallB2 = b2;
		m_StallB3 = b3;
		m_StallB4 = b4;
		m_StallName = stallName;
		m_CurrencyUsed = currency;
		m_AreaTaxModifier = taxMod;
	}
	
	void SetItemInfo(int b1, int b2, int b3, int b4, string itemClass, string itemName, int price, int category){
		m_ItemB1 = b1;
		m_ItemB2 = b2;
		m_ItemB3 = b3;
		m_ItemB4 = b4;
		m_ItemClass = itemClass;
		m_ItemName = itemName;
		m_Price = price;
		m_Category = category;
	}
	
	void SetItemDetails(int healthLevel, float quantity, float quantityMax, float weight, int liquidType){
		m_HealthLevel = healthLevel;
		m_Quantity = quantity;
		m_QuantityMax = quantityMax;
		m_Weight = weight;
		m_LiquidType = liquidType;
	}
	
	void SetItemNetworkID(int low, int high){
		m_ItemNetLow = low;
		m_ItemNetHigh = high;
	}
	
	void SetAttachments(TStringArray attachments){
		if (attachments){
			m_Attachments = new TStringArray;
			for (int i = 0; i < attachments.Count(); i++){
				m_Attachments.Insert(attachments[i]);
			}
		}
	}
	
	string GetHealthName(){
		switch (m_HealthLevel){
			case GameConstants.STATE_PRISTINE: return "Pristine";
			case GameConstants.STATE_WORN: return "Worn";
			case GameConstants.STATE_DAMAGED: return "Damaged";
			case GameConstants.STATE_BADLY_DAMAGED: return "Badly Damaged";
			case GameConstants.STATE_RUINED: return "Ruined";
		}
		return "";
	}
	
	int GetPriceWithTax(){
		float effectiveTax = Math.Clamp(GetPMConfig().SaleTaxAmount + m_AreaTaxModifier, 0.0, 1.0);
		if (effectiveTax > 0){
			return m_Price + (m_Price * effectiveTax);
		}
		return m_Price;
	}
	
	bool IsStall(int b1, int b2, int b3, int b4){
		return (m_StallB1 == b1 && m_StallB2 == b2 && m_StallB3 == b3 && m_StallB4 == b4);
	}
	
	bool IsItem(int b1, int b2, int b3, int b4){
		return (m_ItemB1 == b1 && m_ItemB2 == b2 && m_ItemB3 == b3 && m_ItemB4 == b4);
	}
}

// RPC data types for concierge
typedef Param1<ref array<ref PMConciergeEntry>> PM_RPCConciergeSyncData;

class PMConciergeBuyRequest extends Managed {
	int m_StallB1, m_StallB2, m_StallB3, m_StallB4;
	int m_ItemB1, m_ItemB2, m_ItemB3, m_ItemB4;
	
	void PMConciergeBuyRequest(int sb1, int sb2, int sb3, int sb4, int ib1, int ib2, int ib3, int ib4){
		m_StallB1 = sb1;
		m_StallB2 = sb2;
		m_StallB3 = sb3;
		m_StallB4 = sb4;
		m_ItemB1 = ib1;
		m_ItemB2 = ib2;
		m_ItemB3 = ib3;
		m_ItemB4 = ib4;
	}
}

typedef Param1<ref PMConciergeBuyRequest> PM_RPCConciergeBuyData;
