class CfgPatches
{
	class PlayerMarkets
	{
		requiredVersion=0.1;
		requiredAddons[]={ 
			"DZ_Gear_Camping",
			"DZ_Weapons_Melee"
		};
	};
};

class CfgMods
{
	class PlayerMarkets
	{
		dir="PlayerMarkets";
        name="PlayerMarkets";
        credits="DaemonForge";
        author="DaemonForge";
        authorID="0";
        version="0.1";
        extra=0;
        type="mod";
	    dependencies[]={ "Game", "World", "Mission"};
	    class defs
	    {
			class gameScriptModule
            {
				value = "";
                files[]={
					"PlayerMarkets/scripts/3_Game"
					};
            };
			
			class worldScriptModule
            {
                value="";
                files[]={ 
					"PlayerMarkets/scripts/4_World" 
				};
            };
			
	        class missionScriptModule
            {
                value="";
                files[]={
					"PlayerMarkets/scripts/5_Mission" 
				};
            };
        };
    };
};
class CfgVehicles
{
	class Container_Base;
	class Inventory_Base;
	
	class PM_MarketKit: Inventory_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_WatchtowerKit0";
		descriptionShort = "$STR_CfgVehicles_WatchtowerKit1";
		model = "\DZ\gear\camping\watchtower_kit.p3d";
		rotationFlags = 17;
		itemSize[] = {1,5};
		weight = 280;
		itemBehaviour = 1;
		attachments[] = {"Rope"};
		
		class AnimationSources
		{
			class AnimSourceShown
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class AnimSourceHidden
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class Inventory: AnimSourceHidden{};
			class Placing: AnimSourceHidden{};
		};
		
		class AnimEvents
		{
			class SoundWeapon
			{
				class crafting_1
				{
					soundSet = "FenceKit_crafting_1_SoundSet";
					id = 1111;
				};
				class crafting_2
				{
					soundSet = "FenceKit_crafting_2_SoundSet";
					id = 1112;
				};
				class crafting_3
				{
					soundSet = "FenceKit_crafting_3_SoundSet";
					id = 1113;
				};
				class crafting_4
				{
					soundSet = "FenceKit_crafting_4_SoundSet";
					id = 1114;
				};
				class crafting_5
				{
					soundSet = "FenceKit_crafting_5_SoundSet";
					id = 1115;
				};
			};
		};
	};
	class PM_MarketStand : Inventory_Base {
		scope = 2;
		displayName  = "Market Stand";
		descriptionShort = "A place where players can sell there items";
		model = "PlayerMarkets\data\MarketStand\MarketStand.p3d";
		itemSize[] = {10,20};
		weight = 10000;
		physLayer = "item_large";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 30000;
				};
			};
			class GlobalArmor
			{
				class FragGrenade
				{
					class Health
					{
						damage = 8.0;
					};
					class Blood
					{
						damage = 8.0;
					};
					class Shock
					{
						damage = 8.0;
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,12};
			openable = 0;
			allowOwnedCargoManipulation = 1;
		};
		attachments[] = {"Merchant_SlotLarge1","Merchant_SlotLarge2","Merchant_SlotLarge3","Merchant_SlotLarge4","Merchant_SlotSmall1","Merchant_SlotSmall2","Merchant_SlotSmall3","Merchant_SlotSmall4","Merchant_SlotSmall5"};
	};
	class PM_Merchant_Base: Inventory_Base
	{
		scope = 2;
		displayName  = "Merchant Item Slot";
		descriptionShort = "";
		model = "PlayerMarkets\data\SlotItems\GunSlot.p3d";
		itemSize[] = {3,3};
		rotationFlags = 17;
		weight = 280;
		itemBehaviour = 1;
		attachments[] = { "Shoulder" };
		inventorySlot[] = {"Merchant_SlotLarge1","Merchant_SlotLarge2","Merchant_SlotLarge3","Merchant_SlotLarge4","Merchant_SlotSmall1","Merchant_SlotSmall2","Merchant_SlotSmall3","Merchant_SlotSmall4","Merchant_SlotSmall5","Merchant_SlotSmall6" };
	};
	class PM_Merchant_Guns: PM_Merchant_Base
	{
		scope = 2;
		displayName  = "Merchant Item Slot Guns";
		model = "PlayerMarkets\data\SlotItems\GunSlot.p3d";
		attachments[] = { "Shoulder1" };
		inventorySlot[] = {"Merchant_SlotLarge1","Merchant_SlotLarge2","Merchant_SlotLarge3","Merchant_SlotLarge4"};
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
};
class CfgWeapons
{
	class RifleCore;
	class Rifle_Base: RifleCore
	{
		inventorySlot[] = {"Shoulder","Melee","Shoulder1","Shoulder2","Shoulder3","Shoulder4","Shoulder5","Shoulder6","Shoulder7","Shoulder8","Shoulder9","Shoulder10","Shoulder11","Shoulder12","Shoulder13","Shoulder14","Shoulder15","Shoulder16","Shoulder17","Shoulder18","Shoulder19","Shoulder20","Shoulder21","Shoulder22","Shoulder23","Shoulder24","Shoulder25","Shoulder26","Shoulder27","Shoulder28","Shoulder29","Shoulder30"};
	};
	class PistolCore;
	class Pistol_Base: PistolCore
	{
		inventorySlot[] = {"Pistol","Pistol1","Pistol2","Pistol3","Pistol4"};
	};
};
class CfgNonAIVehicles
{
    class ProxyAttachment;
    class Proxymosin9130 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Shoulder";
        model="\DZ\weapons\firearms\mosin9130\mosin9130.p3d";
    };
    class Proxy1911 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Shoulder";
        model="\DZ\weapons\pistols\1911\1911.p3d";
    };
    class Proxysteak_knife : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Shoulder";
        model="\DZ\gear\tools\steak_knife.p3d";
    };
    class ProxyBoonieHat_g : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Shoulder";
        model="\DZ\characters\headgear\BoonieHat_g";
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
};
class CfgSlots
{
	
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
};

