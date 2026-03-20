class CfgPatches
{
	class PlayerMarkets_Proxies
	{
		requiredVersion=0.1;
		requiredAddons[]={ 
			"PlayerMarkets_Slots"
		};
	};
};
class CfgVehicles
{
	
	class Inventory_Base;
	class Bottle_Base;
	class Edible_Base: Inventory_Base
	{
		inventorySlot[] += {"StallFood"};
	};
	class Rice: Edible_Base
	{
		inventorySlot[] += {"StallFoodAlt"};
	};
	class WaterBottle: Bottle_Base
	{
		inventorySlot[] += {"StallWater"};
	};
	class Vodka: Bottle_Base
	{
		inventorySlot[] += {"StallWater"};
	};
	class SodaCan_ColorBase: Edible_Base
	{
		inventorySlot[] += {"StallFoodAlt"};
	};
};

class CfgWeapons
{
	class RifleCore;
	class Rifle_Base: RifleCore
	{
		inventorySlot[] += {"PM_Weapon"};
	};
	class PistolCore;
	class Pistol_Base: PistolCore
	{
		inventorySlot[] += {"PM_Pistol"};
	};
};
class CfgMagazines
{
	class DefaultMagazine;
	class Magazine_Base: DefaultMagazine
	{
		inventorySlot[] += {"StallMag"};
	};
};
class CfgNonAIVehicles
{
    class ProxyAttachment;
    class ProxyPMWeapon01 : ProxyAttachment
    {
        scope = 2;
        inventorySlot[] += {"PM_Weapon"};
        model="\PlayerMarkets\data\Proxies\PMWeapon01.p3d";
    }; 
	class Proxykitchenknife : ProxyAttachment
    {
        scope = 2;
        inventorySlot[] += {"Knife"};
        model="\DZ\gear\tools\kitchenknife.p3d";
    };
    class ProxyPMPistol01 : ProxyAttachment
    {
        scope = 2;
        inventorySlot[] += {"PM_Pistol"};
        model="\PlayerMarkets\data\Proxies\PMPistol01.p3d";
    };
	class ProxyBoonieHat_g : ProxyAttachment
    {
        scope = 2;
        inventorySlot[] += {"Headgear"};
        model="\DZ\characters\headgear\BoonieHat_g.p3d";
    };
    class Proxyfood_can : ProxyAttachment
    {
        scope = 2;
        inventorySlot[] += {"StallFood"};
        model="\DZ\gear\food\food_can.p3d";
    };
    class Proxywaterbottle : ProxyAttachment
    {
        scope = 2;
        inventorySlot[] += {"StallWater"};
        model="\DZ\gear\drinks\waterbottle.p3d";
    };
    class Proxysodacan : ProxyAttachment
    {
        scope = 2;
        inventorySlot[] += {"StallFoodAlt"};
        model="\DZ\gear\drinks\sodacan.p3d";
    };
    class Proxycanteen : ProxyAttachment
    {
        scope = 2;
        inventorySlot[] += {"Belt_Left"};
        model="\DZ\gear\drinks\canteen.p3d";
    };
    class Proxymagazine_ak74_30rnd : ProxyAttachment
    {
        scope = 2;
        inventorySlot[] += {"StallMag"};
        model="\DZ\weapons\attachments\magazine\magazine_ak74_30rnd.p3d";
    };
    class Proxyfirefighteraxe : ProxyAttachment
    {
        scope = 2;
        inventorySlot[] += {"Melee"};
        model="\DZ\weapons\melee\blade\firefighteraxe.p3d";
    };	
};
class CfgSlots
{
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
	class Slot_PM_Pistol
	{
		name = "PM_Pistol";
		displayName = "Pistol1";
		ghostIcon = "pistol";
	};
	class Slot_PM_Weapon
	{
		name = "PM_Weapon";
		displayName = "PM_Weapon";
		ghostIcon = "rifle";
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
};