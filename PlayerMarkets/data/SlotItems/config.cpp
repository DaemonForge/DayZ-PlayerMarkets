class CfgPatches
{
	class PlayerMarkets_Slots
	{
		requiredVersion=0.1;
		requiredAddons[]={ 
			"DZ_Gear_Camping",
			"DZ_Weapons_Melee",
			"DZ_Weapons_Magazines",
			"UniversalApi",
		};
	};
};
class CfgVehicles
{
	
	class Inventory_Base;
	class Bottle_Base;
	class PM_Merchant_Base: Inventory_Base
	{
		scope = 2;
		displayName  = "Merchant Item Slot";
		model = "PlayerMarkets\data\SlotItems\GunSlot.p3d";
		descriptionShort = "This item is only meant to be created on to a market stand";
		itemSize[] = {11,11};
		weight = 280;
		itemBehaviour = 1;
		inventorySlot[] = {"Merchant_SlotLarge1","Merchant_SlotLarge2","Merchant_SlotLarge3","Merchant_SlotLarge4","Merchant_SlotLarge5","Merchant_SlotLarge6","Merchant_SlotLarge7","Merchant_SlotSmall1","Merchant_SlotSmall2","Merchant_SlotSmall3","Merchant_SlotSmall4","Merchant_SlotSmall5","Merchant_SlotSmall6","Merchant_SlotSmall7","Merchant_SlotSmall8","Merchant_SlotSmall9","Merchant_SlotSmall10"};
	};
	class PM_Merchant_Guns: PM_Merchant_Base
	{
		scope = 2;
		displayName  = "Merchant Item Slot Guns";
		model = "PlayerMarkets\data\SlotItems\GunSlot.p3d";
		attachments[] = { "Shoulder1" };
		inventorySlot[] = {"Merchant_SlotLarge1","Merchant_SlotLarge2","Merchant_SlotLarge3","Merchant_SlotLarge4","Merchant_SlotLarge5","Merchant_SlotLarge6","Merchant_SlotLarge7"};
	};
	class PM_Merchant_Melee: PM_Merchant_Base
	{
		scope = 2;
		displayName  = "Merchant Item Slot Guns";
		model = "PlayerMarkets\data\SlotItems\Melee.p3d";
		attachments[] = { "Melee" };
		inventorySlot[] = {"Merchant_SlotLarge1","Merchant_SlotLarge2","Merchant_SlotLarge3","Merchant_SlotLarge4","Merchant_SlotLarge5","Merchant_SlotLarge6","Merchant_SlotLarge7"};
	};
	class PM_Merchant_Pistol: PM_Merchant_Base
	{
		scope = 2;
		displayName  = "Merchant Item Slot Pistol";
		descriptionShort = "A kit to place a player market";
		model = "PlayerMarkets\data\SlotItems\PistolSlot.p3d";
		attachments[] = { "Pistol1" };

	};
	class PM_Merchant_Knife: PM_Merchant_Base
	{
		scope = 2;
		displayName  = "Merchant Item Slot Knife";
		descriptionShort = "";
		model = "PlayerMarkets\data\SlotItems\KnifeSlot.p3d";
		attachments[] = { "Knife" };
	};
	class PM_Merchant_Headgear: PM_Merchant_Base
	{
		scope = 2;
		displayName  = "Merchant Item Slot Headgear";
		descriptionShort = "";
		model = "PlayerMarkets\data\SlotItems\HeadgearSlot.p3d";
		attachments[] = { "Headgear" };
	};
	class PM_Merchant_Magazine: PM_Merchant_Base
	{
		scope = 2;
		displayName  = "Merchant Item Slot Mag";
		descriptionShort = "";
		model = "PlayerMarkets\data\SlotItems\MagSlot.p3d";
		attachments[] = { "StallMag" };
	};
	class PM_Merchant_StallFood: PM_Merchant_Base
	{
		scope = 2;
		displayName  = "Merchant Item Slot Food";
		descriptionShort = "";
		model = "PlayerMarkets\data\SlotItems\Food.p3d";
		attachments[] = { "StallFood" };
	};
	class PM_Merchant_StallFoodAlt: PM_Merchant_Base
	{
		scope = 2;
		displayName  = "Merchant Item Slot Food";
		descriptionShort = "";
		model = "PlayerMarkets\data\SlotItems\FoodAlt.p3d";
		attachments[] = { "StallFoodAlt" };
	};
	class PM_Merchant_StallCanteen: PM_Merchant_Base
	{
		scope = 2;
		displayName  = "Merchant Item Slot Food";
		descriptionShort = "";
		model = "PlayerMarkets\data\SlotItems\beltleft.p3d";
		attachments[] = { "Belt_Left" };
	};
	class PM_Merchant_Grenade: PM_Merchant_Base
	{
		scope = 2;
		displayName  = "Merchant Item Slot Food";
		descriptionShort = "";
		model = "PlayerMarkets\data\SlotItems\grenade.p3d";
		attachments[] = { "tripWireAttachment" };
	};
	class PM_Merchant_StallWater: PM_Merchant_Base
	{
		scope = 2;
		displayName  = "Merchant Item Slot Water";
		descriptionShort = "";
		model = "PlayerMarkets\data\SlotItems\Bottle.p3d";
		attachments[] = { "StallWater" };
	};
	
	
	class PM_MarketStorage : PM_Merchant_Base 
	{
		scope = 2;
		displayName = "(DEBUG)Market Storage";
		model = "\DZ\gear\containers\ChristmasBox1.p3d";
		descriptionShort = "This item is only meant to be created on to a market stand";
		weight = 999999;
		inventorySlot[] = {"Merchant_Storage" };
		class Cargo
		{
			itemsCargoSize[] = {10,100};
			openable = 0;
			allowOwnedCargoManipulation = 1;
		};
	};
	
	
	
	class Edible_Base: Inventory_Base
	{
		inventorySlot[] = {"StallFood"};
	};
	class Rice: Edible_Base
	{
		inventorySlot[] = {"StallFoodAlt"};
	};
	class WaterBottle: Bottle_Base
	{
		inventorySlot[] = {"StallWater"};
	}
	
	class Vodka: Bottle_Base
	{
		inventorySlot[] = {"StallWater"};
	}
	class SodaCan_ColorBase: Edible_Base
	{
		inventorySlot[] = {"StallFoodAlt"};
	};
	
	
};

class CfgNonAIVehicles
{
    class ProxyAttachment;
    class Proxymosin9130 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Shoulder1";
        model="\DZ\weapons\firearms\mosin9130\mosin9130.p3d";
    }; 
	class ProxyBoonieHat_g : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Headgear";
        model="\DZ\characters\headgear\BoonieHat_g.p3d";
    };
	class Proxykitchenknife : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Knife";
        model="\DZ\gear\tools\kitchenknife.p3d";
    };
    class Proxy1911 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Pistol1";
        model="\DZ\weapons\pistols\1911\1911.p3d";
    };
    class Proxyfood_can : ProxyAttachment
    {
        scope = 2;
        InventorySlot="StallFood";
        model="\DZ\gear\food\food_can.p3d";
    };
    class Proxywaterbottle : ProxyAttachment
    {
        scope = 2;
        InventorySlot="StallWater";
        model="\DZ\gear\drinks\waterbottle.p3d";
    };
    class Proxysodacan : ProxyAttachment
    {
        scope = 2;
        InventorySlot="StallFoodAlt";
        model="\DZ\gear\drinks\sodacan.p3d";
    };
    class Proxycanteen : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Belt_Left";
        model="\DZ\gear\drinks\canteen.p3d";
    };
    class Proxymagazine_ak74_30rnd : ProxyAttachment
    {
        scope = 2;
        InventorySlot="StallMag";
        model="\DZ\weapons\attachments\magazine\magazine_ak74_30rnd.p3d";
    };
	
	
    class Proxyfirefighteraxe : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Melee";
        model="\DZ\weapons\melee\blade\firefighteraxe.p3d";
    };
	
	
	
	
    class ProxyGunSlot : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotLarge1";
        model="\PlayerMarkets\data\SlotItems\GunSlot.p3d";
    };
    class ProxyGunSlot2 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotLarge2";
        model="\PlayerMarkets\data\SlotItems\GunSlot2.p3d";
    };
    class ProxyGunSlot3 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotLarge3";
        model="\PlayerMarkets\data\SlotItems\GunSlot3.p3d";
    };
    class ProxyGunSlot4 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotLarge4";
        model="\PlayerMarkets\data\SlotItems\GunSlot4.p3d";
    };
    class ProxyGunSlot5 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotLarge5";
        model="\PlayerMarkets\data\SlotItems\GunSlot5.p3d";
    };
    class ProxyGunSlot6 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotLarge6";
        model="\PlayerMarkets\data\SlotItems\GunSlot6.p3d";
    };
    class ProxyGunSlot7 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotLarge7";
        model="\PlayerMarkets\data\SlotItems\GunSlot7.p3d";
    };
    class ProxyPistolSlot : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotSmall1";
        model="\PlayerMarkets\data\SlotItems\PistolSlot.p3d";
    };
    class ProxyPistolSlot2 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotSmall2";
        model="\PlayerMarkets\data\SlotItems\PistolSlot2.p3d";
    };
    class ProxyPistolSlot3 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotSmall3";
        model="\PlayerMarkets\data\SlotItems\PistolSlot3.p3d";
    };
    class ProxyPistolSlot4 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotSmall4";
        model="\PlayerMarkets\data\SlotItems\PistolSlot4.p3d";
    };
    class ProxyPistolSlot5 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotSmall5";
        model="\PlayerMarkets\data\SlotItems\PistolSlot5.p3d";
    };
    class ProxyPistolSlot6 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotSmall6";
        model="\PlayerMarkets\data\SlotItems\PistolSlot6.p3d";
    };
    class ProxyPistolSlot7 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotSmall7";
        model="\PlayerMarkets\data\SlotItems\PistolSlot7.p3d";
    };
    class ProxyPistolSlot8 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotSmall8";
        model="\PlayerMarkets\data\SlotItems\KnifeSlot.p3d";
    };
    class ProxyPistolSlot9 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotSmall9";
        model="\PlayerMarkets\data\SlotItems\KnifeSlot2.p3d";
    };
    class ProxyPistolSlot10: ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_SlotSmall10";
        model="\PlayerMarkets\data\SlotItems\KnifeSlot3.p3d";
    };
	
	
	
    class ProxyPaperSignModel: ProxyAttachment
    {
        scope = 2;
        InventorySlot="Merchant_Sign1";
        model="\PlayerMarkets\data\Signs\Tier1\PaperSignModel.p3d";
    };
	
};
class CfgSlots
{
	class Slot_Merchant_Storage
	{
		name = "Merchant_Storage";
		displayName = "Market Storage";
		ghostIcon = "book";
	};
	class Slot_StallMag
	{
		name = "StallMag";
		displayName = "Market Storage";
		ghostIcon = "book";
	};
	class Slot_StallCanteen
	{
		name = "StallCanteen";
		displayName = "Market Storage";
		ghostIcon = "book";
	};
	class Slot_StallWater
	{
		name = "StallWater";
		displayName = "Market Storage";
		ghostIcon = "book";
	};
	class Slot_Pistol1
	{
		name = "Pistol1";
		displayName = "Pistol1";
		ghostIcon = "pistol";
	};
	class Slot_Shoulder1
	{
		name = "Shoulder1";
		displayName = "Shoulder1";
		ghostIcon = "shoulderleft";
	};
	class Slot_Merchant_SlotLarge1
	{
		name = "Merchant_SlotLarge1";
		displayName = "Merchant_Slot";
		ghostIcon = "shoulderleft";
	};
	class Slot_Merchant_SlotLarge2
	{
		name = "Merchant_SlotLarge2";
		displayName = "Merchant_Slot";
		ghostIcon = "shoulderleft";
	};
	class Slot_Merchant_SlotLarge3
	{
		name = "Merchant_SlotLarge3";
		displayName = "Merchant_Slot";
		ghostIcon = "shoulderleft";
	};
	class Slot_Merchant_SlotLarge4
	{
		name = "Merchant_SlotLarge4";
		displayName = "Merchant_Slot";
		ghostIcon = "shoulderleft";
	};
	class Slot_Merchant_SlotLarge5
	{
		name = "Merchant_SlotLarge5";
		displayName = "Merchant_Slot";
		ghostIcon = "shoulderleft";
	};
	class Slot_Merchant_SlotLarge6
	{
		name = "Merchant_SlotLarge6";
		displayName = "Merchant_Slot";
		ghostIcon = "shoulderleft";
	};
	class Slot_Merchant_SlotLarge7
	{
		name = "Merchant_SlotLarge7";
		displayName = "Merchant_Slot";
		ghostIcon = "shoulderleft";
	};
	class Slot_Merchant_SlotSmall1
	{
		name = "Merchant_SlotSmall1";
		displayName = "Merchant_Slot";
		ghostIcon = "book";
	};
	class Slot_Merchant_SlotSmall2
	{
		name = "Merchant_SlotSmall2";
		displayName = "Merchant_Slot";
		ghostIcon = "book";
	};
	class Slot_Merchant_SlotSmall3
	{
		name = "Merchant_SlotSmall3";
		displayName = "Merchant_Slot";
		ghostIcon = "book";
	};
	class Slot_Merchant_SlotSmall4
	{
		name = "Merchant_SlotSmall4";
		displayName = "Merchant_Slot";
		ghostIcon = "book";
	};
	class Slot_Merchant_SlotSmall5
	{
		name = "Merchant_SlotSmall5";
		displayName = "Merchant_Slot";
		ghostIcon = "book";
	};
	class Slot_Merchant_SlotSmall6
	{
		name = "Merchant_SlotSmall6";
		displayName = "Merchant_Slot";
		ghostIcon = "book";
	};
	class Slot_Merchant_SlotSmall7
	{
		name = "Merchant_SlotSmall7";
		displayName = "Merchant_Slot";
		ghostIcon = "book";
	};
	class Slot_Merchant_SlotSmall8
	{
		name = "Merchant_SlotSmall8";
		displayName = "Merchant_Slot";
		ghostIcon = "book";
	};
	class Slot_Merchant_SlotSmall9
	{
		name = "Merchant_SlotSmall9";
		displayName = "Merchant_Slot";
		ghostIcon = "book";
	};
	class Slot_Merchant_SlotSmall10
	{
		name = "Merchant_SlotSmall10";
		displayName = "Merchant_Slot";
		ghostIcon = "book";
	};
	class Slot_StallFood
	{
		name = "StallFood";
		displayName = "Stall Food";
		ghostIcon = "book";
	};
	class Slot_StallFoodAlt
	{
		name = "StallFoodAlt";
		displayName = "Stall Food";
		ghostIcon = "book";
	};
	class Slot_Merchant_Sign1
	{
		name = "Merchant_Sign1";
		displayName = "Merchant Sign";
		ghostIcon = "book";
	};
	class Slot_Merchant_Sign2
	{
		name = "Merchant_Sign2";
		displayName = "Merchant Sign";
		ghostIcon = "book";
	};
	class Slot_Merchant_Sign3
	{
		name = "Merchant_Sign3";
		displayName = "Merchant Sign";
		ghostIcon = "book";
	};
};

