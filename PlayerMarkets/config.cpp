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
	class BaseBuildingBase;
	class MarketStandBase: BaseBuildingBase {}
	
	class PM_MarketKit: Inventory_Base
	{
		scope = 2;
		displayName = "Market Stall Kit";
		descriptionShort = "Market Stall Kit, Place to make a stall";
		model = "PlayerMarkets\data\MarketStand\MarketStandKit.p3d";
		rotationFlags = 17;
		itemSize[] = {1,5};
		weight = 280;
		itemBehaviour=1;
		attachments[]=
		{
			"Rope"
		};
		class AnimationSources
		{
			class AnimSourceShown
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class AnimSourceHidden
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class Inventory: AnimSourceShown
			{
			};
			class Placing: AnimSourceHidden
			{
			};
		};	
	};
	class PM_MarketV2Kit: Inventory_Base
	{
		scope = 2;
		displayName = "Market Stall V2 Kit";
		descriptionShort = "Market Stall V2 Kit, Place to make a stall";
		model = "PlayerMarkets\data\MarketStand\MarketStandV2Kit.p3d";
		rotationFlags = 17;
		itemSize[] = {1,2};
		weight = 280;
		itemBehaviour=1;
		attachments[]=
		{
			"Rope"
		};
		class AnimationSources
		{
			class AnimSourceShown
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class AnimSourceHidden
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class Inventory: AnimSourceShown
			{
			};
			class Placing: AnimSourceHidden
			{
			};
		};	
	};
	class PM_MarketTableKit : PM_MarketKit
	{
		displayName = "Market Table Kit";
		descriptionShort = "Market Table Kit, Place to make a Table";
		model = "PlayerMarkets\data\MarketStand\MarketTableKit.p3d";
	};
	class PM_MaketStandPlacing : PM_MarketKit
	{
		displayName="This is a hologram";
		descriptionShort="Nothing to see here, move along";
		scope=2;
		model="PlayerMarkets\data\MarketStand\MarketStandKitHologram.p3d";
		storageCategory=10;
		hiddenSelections[]=
		{
			"placing"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\gear\consumables\data\pile_of_planks_co.tga"
		};
		hiddenSelectionsMaterials[]=
		{
			"dz\gear\camping\data\fence_pile_of_planks.rvmat"
		};
		hologramMaterial="tent_medium";
		hologramMaterialPath="dz\gear\camping\data";
		alignHologramToTerain=0;
		slopeTolerance=0.30000001;	
	}
	class PM_MarketStand : MarketStandBase 
	{
		scope = 2;
		displayName  = "Market Stand";
		descriptionShort = "A place where players can sell there items";
		model = "PlayerMarkets\data\MarketStand\MarketStand.p3d";
		bounding="BSphere";
		overrideDrawArea="3.0";
		forceFarBubble="true";
		handheld="false";
		lootCategory="Crafted";
		carveNavmesh=1;
		weight=10000;
		itemSize[]={2,3};
		physLayer="item_large";
		createProxyPhysicsOnInit="false";
		rotationFlags=2;
		createdProxiesOnInit[]=
		{
			"Deployed"
		};
		attachments[]=
		{
			"Material_Nails",
			"Material_WoodenLogs",
			"Material_WoodenPlanks",
			"Merchant_SlotLarge1",
			"Merchant_SlotLarge2",
			"Merchant_SlotLarge3",
			"Merchant_SlotLarge4",
			"Merchant_SlotSmall1",
			"Merchant_SlotSmall2",
			"Merchant_SlotSmall3",
			"Merchant_SlotSmall4",
			"Merchant_SlotSmall5",
			"Merchant_Storage"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
					healthLevels[]=
					{
						
						{
							1.0,
							
							{
								""
							}
						},
						
						{
							0.69999999,
							
							{
								""
							}
						},
						
						{
							0.5,
							
							{
								""
							}
						},
						
						{
							0.30000001,
							
							{
								""
							}
						},
						
						{
							0.0,
							
							{
								""
							}
						}
					};
				};
			};
			class GlobalArmor
			{
				class FragGrenade
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
			};
			class DamageZones
			{
				class Body
				{
					class Health
					{
						displayName="Table";
						hitpoints=1000;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1.0,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks_damage.rvmat"
								}
							},
							
							{
								0.0,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks_destruct.rvmat"
								}
							}
						};
					};
					class ArmorType
					{
						class Projectile
						{
							class Health
							{
								damage=0;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
						class Melee
						{
							class Health
							{
								damage=0;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
						class FragGrenade
						{
							class Health
							{
								damage=0;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
					};
					componentNames[]=
					{
						"body"
					};
					fatalInjuryCoef=-1;
				};
				class Wall_Base_Down
				{
					class Health
					{
						displayName="Posts";
						hitpoints=1500;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1.0,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks_worn.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"BasicDoors\Data\Window Barricade\data\dmg\Plank_with_nails_dmg.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"BasicDoors\Data\Window Barricade\data\dmg\Plank_with_nails_dmg.rvmat"
								}
							},
							
							{
								0.0,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks_bdamage.rvmat"
								}
							}
						};
					};
					class ArmorType
					{
						class Projectile
						{
							class Health
							{
								damage=1;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
						class Melee
						{
							class Health
							{
								damage=0.64999998;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
						class FragGrenade
						{
							class Health
							{
								damage=50;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
					};
					componentNames[]=
					{
						"wall_base_down"
					};
					fatalInjuryCoef=-1;
				};
				class Wall_Base_Up: Wall_Base_Down
				{
					displayName="Roof";
					componentNames[]=
					{
						"wall_base_up"
					};
				};
			};
		};
		class GUIInventoryAttachmentsProps
		{
			//class Base
			//{
			//	name="$STR_CfgVehicles_Fence_Att_Category_Base";
			//	description="";
			//	attachmentSlots[]=
			//	{
			//		"Material_WoodenLogs"
			//	};
			//	icon="set:dayz_inventory image:cat_bb_base";
			//	selection="wall";
			//};
			class Attachments
			{
				name="Merchant Slots";
				description="";
				attachmentSlots[]=
				{
					"Merchant_SlotLarge1",
					"Merchant_SlotLarge2",
					"Merchant_SlotLarge3",
					"Merchant_SlotLarge4",
					"Merchant_SlotSmall1",
					"Merchant_SlotSmall2",
					"Merchant_SlotSmall3",
					"Merchant_SlotSmall4",
					"Merchant_SlotSmall5",
					"Merchant_Storage"
				};
				icon="";
				selection="";
			};
			class Table
			{
				name="First Stage";
				description="";
				attachmentSlots[]=
				{
					"Material_Nails",
					"Material_WoodenPlanks"
				};
				icon="set:dayz_inventory image:cat_bb_material";
				selection="wall";
			};
			class Posts
			{
				name="Second Stage";
				description="";
				attachmentSlots[]=
				{
					"Material_Nails",
				    "Material_WoodenLogs"
				};
				icon="set:dayz_inventory image:cat_bb_material";
				selection="wall";
			};
			class Roof
			{
				name="Third Stage";
				description="";
				attachmentSlots[]=
				{
					"Material_Nails",
					"Material_WoodenPlanks"
				};
				icon="set:dayz_inventory image:cat_bb_material";
				selection="wall";
			};
		};
		class AnimationSources
		{
			class AnimSourceShown
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class AnimSourceHidden
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class AnimRotate
			{
				source="user";
				animPeriod=0.017999999;
				initPhase=0;
			};
			class Deployed: AnimSourceHidden
			{
			};
			class Base: AnimSourceHidden
			{
			};
			class Wall_Base_Down: AnimSourceHidden
			{
			};
			class Wall_Base_Up: AnimSourceHidden
			{
			};
		};
		class Construction
		{
			class wall
			{
				class base
				{
					name="Trader Table";
					is_base=1;
					id=1;
					required_parts[]={};
					conflicted_parts[]={};
					collision_data[]={};
					build_action_type=2;
					dismantle_action_type=2;
					material_type=1;
					class Materials
					{
						class Material1
						{
							type="WoodenPlank";
							slot_name="Material_WoodenPlanks";
							quantity=12;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=16;
						};
					};
				};
				class wall_base_down
				{
					name="Roof Posts";
					id=2;
					required_parts[]=
					{
						"base"
					};
					conflicted_parts[]={};
					collision_data[]={};
					build_action_type=2;
					dismantle_action_type=2;
					material_type=2;
					class Materials
					{
						class Material1
						{
							type="WoodenPlank";
							slot_name="Material_WoodenLogs";
							quantity=2;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=8;
						};
					};
				};
				class wall_base_up
				{
					name="Trader Roof";
					id=3;
					required_parts[]=
					{
						"wall_base_down"
					};
					conflicted_parts[]={};
					collision_data[]={};
					build_action_type=2;
					dismantle_action_type=2;
					material_type=2;
					class Materials
					{
						class Material1
						{
							type="WoodenPlank";
							slot_name="Material_WoodenPlanks";
							quantity=8;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=12;
						};
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
	};
	class PM_MarketStandV2 : MarketStandBase 
	{
		scope = 2;
		displayName  = "Market Stand V2";
		descriptionShort = "A place where players can sell there items";
		model = "PlayerMarkets\data\MarketStand\MarketStandV2.p3d";
		bounding="BSphere";
		overrideDrawArea="3.0";
		forceFarBubble="true";
		handheld="false";
		lootCategory="Crafted";
		carveNavmesh=1;
		weight=10000;
		itemSize[]={2,3};
		physLayer="item_large";
		createProxyPhysicsOnInit="false";
		rotationFlags=2;
		createdProxiesOnInit[]=
		{
			"Deployed"
		};
		attachments[]=
		{
			"Material_Nails",
			"Material_WoodenLogs",
			"Material_WoodenPlanks",
			"Merchant_SlotLarge1",
			"Merchant_SlotLarge2",
			"Merchant_SlotLarge3",
			"Merchant_SlotLarge4",
			"Merchant_SlotSmall1",
			"Merchant_SlotSmall2",
			"Merchant_SlotSmall3",
			"Merchant_SlotSmall4",
			"Merchant_SlotSmall5",
			"Merchant_Storage"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
					healthLevels[]=
					{
						
						{
							1.0,
							
							{
								""
							}
						},
						
						{
							0.69999999,
							
							{
								""
							}
						},
						
						{
							0.5,
							
							{
								""
							}
						},
						
						{
							0.30000001,
							
							{
								""
							}
						},
						
						{
							0.0,
							
							{
								""
							}
						}
					};
				};
			};
			class GlobalArmor
			{
				class FragGrenade
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
			};
			class DamageZones
			{
				class Base
				{
					class Health
					{
						displayName="Table";
						hitpoints=1000;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1.0,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks_damage.rvmat"
								}
							},
							
							{
								0.0,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks_destruct.rvmat"
								}
							}
						};
					};
					class ArmorType
					{
						class Projectile
						{
							class Health
							{
								damage=0;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
						class Melee
						{
							class Health
							{
								damage=0;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
						class FragGrenade
						{
							class Health
							{
								damage=0;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
					};
					componentNames[]=
					{
						"base"
					};
					fatalInjuryCoef=-1;
				};
				class Posts
				{
					class Health
					{
						displayName="Posts";
						hitpoints=1500;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1.0,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks_worn.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"BasicDoors\Data\Window Barricade\data\dmg\Plank_with_nails_dmg.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"BasicDoors\Data\Window Barricade\data\dmg\Plank_with_nails_dmg.rvmat"
								}
							},
							
							{
								0.0,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks_bdamage.rvmat"
								}
							}
						};
					};
					class ArmorType
					{
						class Projectile
						{
							class Health
							{
								damage=1;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
						class Melee
						{
							class Health
							{
								damage=0.64999998;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
						class FragGrenade
						{
							class Health
							{
								damage=50;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
					};
					componentNames[]=
					{
						"posts"
					};
					fatalInjuryCoef=-1;
				};
				class Roof: Posts
				{
					displayName="Roof";
					componentNames[]=
					{
						"roof"
					};
				};
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Attachments
			{
				name="Merchant Slots";
				description="";
				attachmentSlots[]=
				{
					"Merchant_SlotLarge1",
					"Merchant_SlotLarge2",
					"Merchant_SlotLarge3",
					"Merchant_SlotLarge4",
					"Merchant_SlotSmall1",
					"Merchant_SlotSmall2",
					"Merchant_SlotSmall3",
					"Merchant_SlotSmall4",
					"Merchant_SlotSmall5",
					"Merchant_Storage"
				};
				icon="set:dayz_inventory image:cat_bb_attachments";
			};
			class Table
			{
				name="First Stage";
				description="";
				attachmentSlots[]=
				{
					"Material_Nails",
					"Material_WoodenPlanks"
				};
				icon="set:dayz_inventory image:cat_bb_material";
				selection="wall";
			};
			class Posts
			{
				name="Second Stage";
				description="";
				attachmentSlots[]=
				{
					"Material_Nails",
				    "Material_WoodenLogs"
				};
				icon="set:dayz_inventory image:cat_bb_material";
				selection="wall";
			};
			class Roof
			{
				name="Third Stage";
				description="";
				attachmentSlots[]=
				{
					"Material_Nails",
					"Material_WoodenPlanks"
				};
				icon="set:dayz_inventory image:cat_bb_material";
				selection="wall";
			};
		};
		class AnimationSources
		{
			class AnimSourceShown
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class AnimSourceHidden
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class AnimRotate
			{
				source="user";
				animPeriod=0.017999999;
				initPhase=0;
			};
			class Deployed: AnimSourceHidden
			{
			};
			class Base: AnimSourceHidden
			{
			};
			class Posts: AnimSourceHidden
			{
			};
			class Roof: AnimSourceHidden
			{
			};
		};
		class Construction
		{
			class wall
			{
				class base
				{
					name="Trader Table";
					is_base=1;
					id=1;
					required_parts[]={};
					conflicted_parts[]={};
					collision_data[]={};
					build_action_type=2;
					dismantle_action_type=2;
					material_type=1;
					class Materials
					{
						class Material1
						{
							type="WoodenPlank";
							slot_name="Material_WoodenPlanks";
							quantity=16;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=32;
						};
					};
				};
				class posts
				{
					name="Roof Posts";
					id=2;
					required_parts[]=
					{
						"base"
					};
					conflicted_parts[]={};
					collision_data[]={};
					build_action_type=2;
					dismantle_action_type=2;
					material_type=2;
					class Materials
					{
						class Material1
						{
							type="WoodenPlank";
							slot_name="Material_WoodenLogs";
							quantity=2;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=8;
						};
					};
				};
				class roof
				{
					name="Trader Roof";
					id=3;
					required_parts[]=
					{
						"posts"
					};
					conflicted_parts[]={};
					collision_data[]={};
					build_action_type=2;
					dismantle_action_type=2;
					material_type=2;
					class Materials
					{
						class Material1
						{
							type="WoodenPlank";
							slot_name="Material_WoodenPlanks";
							quantity=12;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=24;
						};
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
	};
	class PM_TraderTable : MarketStandBase
	{
		scope = 2;
		displayName  = "Market Table";
		descriptionShort = "A place where players can sell there items";
		model = "PlayerMarkets\data\MarketStand\Trader_Table.p3d";
		bounding="BSphere";
		overrideDrawArea="3.0";
		forceFarBubble="true";
		handheld="false";
		lootCategory="Crafted";
		carveNavmesh=1;
		weight=10000;
		itemSize[]={2,3};
		physLayer="item_large";
		createProxyPhysicsOnInit="false";
		rotationFlags=2;
		createdProxiesOnInit[]=
		{
			"Deployed"
		};
		attachments[]=
		{
			"Material_Nails",
			"Material_WoodenPlanks",
			"Merchant_SlotLarge1",
			"Merchant_SlotLarge2",
			"Merchant_SlotLarge3",
			"Merchant_SlotLarge4",
			"Merchant_SlotSmall1",
			"Merchant_SlotSmall2",
			"Merchant_SlotSmall3",
			"Merchant_SlotSmall4",
			"Merchant_SlotSmall5",
			"Merchant_Storage"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
					healthLevels[]=
					{
						
						{
							1.0,
							
							{
								""
							}
						},
						
						{
							0.69999999,
							
							{
								""
							}
						},
						
						{
							0.5,
							
							{
								""
							}
						},
						
						{
							0.30000001,
							
							{
								""
							}
						},
						
						{
							0.0,
							
							{
								""
							}
						}
					};
				};
			};
			class GlobalArmor
			{
				class FragGrenade
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
			};
			class DamageZones
			{
				class Body
				{
					class Health
					{
						displayName="Base";
						hitpoints=1000;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1.0,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks_damage.rvmat"
								}
							},
							
							{
								0.0,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks_destruct.rvmat"
								}
							}
						};
					};
					class ArmorType
					{
						class Projectile
						{
							class Health
							{
								damage=0;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
						class Melee
						{
							class Health
							{
								damage=0;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
						class FragGrenade
						{
							class Health
							{
								damage=0;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
					};
					componentNames[]=
					{
						"UnAssembled"
					};
					fatalInjuryCoef=-1;
				};
				class Base
				{
					class Health
					{
						displayName="Table";
						hitpoints=1500;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1.0,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks_worn.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"BasicDoors\Data\Window Barricade\data\dmg\Plank_with_nails_dmg.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"BasicDoors\Data\Window Barricade\data\dmg\Plank_with_nails_dmg.rvmat"
								}
							},
							
							{
								0.0,
								
								{
									"DZ\gear\camping\data\fence_pile_of_planks_bdamage.rvmat"
								}
							}
						};
					};
					class ArmorType
					{
						class Projectile
						{
							class Health
							{
								damage=1;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
						class Melee
						{
							class Health
							{
								damage=0.64999998;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
						class FragGrenade
						{
							class Health
							{
								damage=50;
							};
							class Blood
							{
								damage=0;
							};
							class Shock
							{
								damage=0;
							};
						};
					};
					componentNames[]=
					{
						"Assembled"
					};
					fatalInjuryCoef=-1;
				};
			};
		};
		class GUIInventoryAttachmentsProps
		{
			//class Base
			//{
			//	name="$STR_CfgVehicles_Fence_Att_Category_Base";
			//	description="";
			//	attachmentSlots[]=
			//	{
			//		"Material_WoodenLogs"
			//	};
			//	icon="set:dayz_inventory image:cat_bb_base";
			//	selection="wall";
			//};
			class Attachments
			{
				name="Merchant Slots";
				description="";
				attachmentSlots[]=
				{
					"Merchant_SlotLarge1",
					"Merchant_SlotLarge2",
					"Merchant_SlotLarge3",
					"Merchant_SlotLarge4",
					"Merchant_SlotSmall1",
					"Merchant_SlotSmall2",
					"Merchant_SlotSmall3",
					"Merchant_SlotSmall4",
					"Merchant_SlotSmall5",
					"Merchant_Storage"
				};
				icon="";
				selection="";
			};
			class Table
			{
				name="First Stage";
				description="";
				attachmentSlots[]=
				{
					"Material_Nails",
					"Material_WoodenPlanks"
				};
				icon="set:dayz_inventory image:cat_bb_material";
				selection="wall";
			};
		};
		class AnimationSources
		{
			class AnimSourceShown
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class AnimSourceHidden
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class AnimRotate
			{
				source="user";
				animPeriod=0.017999999;
				initPhase=0;
			};
			class Deployed: AnimSourceHidden
			{
			};
			class Base: AnimSourceHidden
			{
			};
		};
		class Construction
		{
			class wall
			{
				class base
				{
					name="Trader Table";
					is_base=1;
					id=1;
					required_parts[]={};
					conflicted_parts[]={};
					collision_data[]={};
					build_action_type=2;
					dismantle_action_type=2;
					material_type=2;
					class Materials
					{
						class Material1
						{
							type="WoodenPlank";
							slot_name="Material_WoodenPlanks";
							quantity=6;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=20;
						};
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
	};
	class PM_Merchant_Base: Inventory_Base
	{
		scope = 2;
		displayName  = "Merchant Item Slot";
		model = "PlayerMarkets\data\SlotItems\GunSlot.p3d";
		descriptionShort = "This item is only meant to be created on to a market stand";
		itemSize[] = {11,11};
		weight = 280;
		itemBehaviour = 1;
		inventorySlot[] = {"Merchant_SlotLarge1","Merchant_SlotLarge2","Merchant_SlotLarge3","Merchant_SlotLarge4","Merchant_SlotSmall1","Merchant_SlotSmall2","Merchant_SlotSmall3","Merchant_SlotSmall4","Merchant_SlotSmall5","Merchant_SlotSmall6","Merchant_SlotSmall7" };
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
	}
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
        InventorySlot="Shoulder1";
        model="\DZ\weapons\firearms\mosin9130\mosin9130.p3d";
    };
    class Proxy1911 : ProxyAttachment
    {
        scope = 2;
        InventorySlot="Pistol1";
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
	class Slot_Merchant_Storage
	{
		name = "Merchant_Storage";
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
};

