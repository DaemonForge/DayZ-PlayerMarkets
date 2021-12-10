class CfgPatches
{
	class PM_Coins
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = 
		{
			"DZ_Data"
		};
	};
};
class cfgvehicles
{
	class Inventory_Base;
	class PM_CopperCoin: Inventory_Base
	{
		scope=2;
		model="PlayerMarkets\data\Coins\CopperCoin.p3d";
		itemSize[]={1,1};
		displayName="Copper Coin";
		descriptionShort="A Copper Coin for buying stuff";
		varQuantityInit = 1;
		varQuantityMin = 0;
		varQuantityMax = 100;
        canBeSplit=1;
        varQuantityDestroyOnMin=1;
	};
	class PM_SilverCoin: Inventory_Base
	{
		scope=2;
		model="PlayerMarkets\data\Coins\SilverCoin.p3d";
		itemSize[]={1,1};
		displayName="Silver Coin";
		descriptionShort="A Silver Coin for buying stuff";
		varQuantityInit = 1;
		varQuantityMin = 0;
		varQuantityMax = 50;
        canBeSplit=1;
        varQuantityDestroyOnMin=1;
	};
	class PM_GoldCoin: Inventory_Base
	{
		scope=2;
		model="PlayerMarkets\data\Coins\GoldCoin.p3d";
		itemSize[]={1,1};
		displayName="Gold Coin";
		descriptionShort="A Gold Coin for buying stuff";
		varQuantityInit = 1;
		varQuantityMin = 0;
		varQuantityMax = 20;
        canBeSplit=1;
        varQuantityDestroyOnMin=1;
	};
};