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
		attachments[]=
		{
			""
		};
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
	class PM_GA_PapperSign : PM_BlankPaperSign
	{
		scope = 2;
		displayName = "Guns & Ammo Paper sign";
		descriptionShort = "Guns & Ammo Paper Sign For Market Stall";
		hiddenSelectionsTextures[]=
		{
			"PlayerMarkets\data\Signs\Tier1\Data\Tier_1_Paper_GA_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"PlayerMarkets\data\Signs\Tier1\Data\empty textures\tier_1_paper.rvmat"
		};
	}
};