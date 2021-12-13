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
		descriptionShort="A Copper Coin for buying stuff - 100 Copper coin equals 1 Silver Coin - 1000 Copper Coins equals 1 Gold Coins";
		varQuantityInit = 1;
		varQuantityMin = 0;
		varQuantityMax = 100;
        canBeSplit=1;
        varQuantityDestroyOnMin=1;
		weight=2;
	};
	class PM_SilverCoin: Inventory_Base
	{
		scope=2;
		model="PlayerMarkets\data\Coins\SilverCoin.p3d";
		itemSize[]={1,1};
		displayName="Silver Coin";
		descriptionShort="A Silver Coin for buying stuff - 100 Copper coin equals 1 Silver Coin - 10 Silver Coins is equal to 1 Gold Coin";
		varQuantityInit = 1;
		varQuantityMin = 0;
		varQuantityMax = 50;
        canBeSplit=1;
        varQuantityDestroyOnMin=1;
		weight=10;
	};
	class PM_GoldCoin: Inventory_Base
	{
		scope=2;
		model="PlayerMarkets\data\Coins\GoldCoin.p3d";
		itemSize[]={1,1};
		displayName="Gold Coin";
		descriptionShort="A Gold Coin for buying stuff - 1000 Copper coin equals 1 Gold Coin - 10 Silver Coins is equal to 1 Gold Coin";
		varQuantityInit = 1;
		varQuantityMin = 0;
		varQuantityMax = 20;
        canBeSplit=1;
        varQuantityDestroyOnMin=1;
		weight=40;
	};
};