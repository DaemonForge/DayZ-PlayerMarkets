class CfgPatches
{
	class PlayerMarketSigns
	{
		requiredVersion=0.1;
		requiredAddons[]=
		{ 
			"DZ_Data"
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class PM_BlankPaperSign: Inventory_Base
	{
		scope = 2;
		displayName = "Blank Paper sign";
		descriptionShort = "Paper Sign For Market Stall";
		model = "PlayerMarkets\data\Signs\Tier1\PaperSignModel.p3d";
		rotationFlags = 17;
		itemSize[] = {1,2};
		weight = 280;
		itemBehaviour=1;
		inventorySlot[]={ "Merchant_Sign1","Merchant_Sign2","Merchant_Sign3" };
		hiddenSelections[]=
		{
			"Paper"
		};
		hiddenSelectionsTextures[]=
		{
			"PlayerMarkets\data\Signs\Tier1\Data\Empty Textures\Tier_1_Paper_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"playermarkets\data\signs\tier1\data\empty textures\tier_1_paper.rvmat"
		};
	};
	class PM_GA_PaperSign : PM_BlankPaperSign
	{
		scope = 2;
		displayName = "Guns & Ammo Paper sign";
		descriptionShort = "Guns & Ammo Paper Sign For Market Stall";
		hiddenSelectionsTextures[]=
		{
			"PlayerMarkets\data\Signs\Tier1\Data\Default Signs\Tier_1_Paper_GA_co.paa"
		};
	};
	class PM_BuildingSupplies_PaperSign : PM_BlankPaperSign
	{
		scope = 2;
		displayName = "Building Supplies sign";
		descriptionShort = "Building Supplies Sign For Market Stall";
		hiddenSelectionsTextures[]=
		{
			"PlayerMarkets\data\Signs\Tier1\Data\Default Signs\BuildingSupplies_PaperSign_co.paa"
		};
	};
	class PM_FoodWater_PaperSign : PM_BlankPaperSign
	{
		scope = 2;
		displayName = "Food & Water sign";
		descriptionShort = "Food & Water Sign For Market Stall";
		hiddenSelectionsTextures[]=
		{
			"PlayerMarkets\data\Signs\Tier1\Data\Default Signs\FoodWater_PaperSign_co.paa"
		};

	};
	class PM_Medical_PaperSign : PM_BlankPaperSign
	{
		scope = 2;
		displayName = "Medical Equipment sign";
		descriptionShort = "Medical Equipment Sign For Market Stall";
		hiddenSelectionsTextures[]=
		{
			"PlayerMarkets\data\Signs\Tier1\Data\Default Signs\Medical_PaperSign_co.paa"
		};
	};
	class PM_Clothing_PaperSign : PM_BlankPaperSign
	{
		scope = 2;
		displayName = "Clothing & Gear sign";
		descriptionShort = "Clothing & Gear Sign For Market Stall";
		hiddenSelectionsTextures[]=
		{
			"PlayerMarkets\data\Signs\Tier1\Data\Default Signs\GearClothing_PaperSign_co.paa"
		};
	};
	
};