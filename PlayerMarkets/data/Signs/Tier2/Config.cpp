class CfgPatches
{
	class PlayerMarketSignsTier2
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
	class PM_BlankWoodSign: Inventory_Base
	{
		scope = 2;
		displayName = "Blank Wood sign";
		descriptionShort = "Paper Sign For Market Stall";
		model = "PlayerMarkets\data\Signs\Tier2\Tier2Sign.p3d";
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
			"PlayerMarkets\data\Signs\Tier2\Data\Empty Textures\Boards_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"playermarkets\data\signs\Tier2\data\Empty Textures\Boards.rvmat"
		};
	};
	class PM_GA_WoodSign : PM_BlankPaperSign
	{
		scope = 2;
		displayName = "Guns & Ammo Paper sign";
		descriptionShort = "Guns & Ammo Paper Sign For Market Stall";
		hiddenSelectionsTextures[]=
		{
			"PlayerMarkets\data\Signs\Tier2\Data\Guns&Ammo_Boards_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"PlayerMarkets\data\Signs\Tier2\Data\empty textures\tier_1_paper.rvmat"
		};
	};
};