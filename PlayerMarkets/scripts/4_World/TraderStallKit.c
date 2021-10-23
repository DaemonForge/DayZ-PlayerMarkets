class PM_MarketStand extends BaseBuildingBase
{
		int m_CurtainStage = -1;
	const float MAX_FLOOR_VERTICAL_DISTANCE 		= 0.5;
	
	
	const float MIN_ACTION_DETECTION_ANGLE_RAD 		= 0.35;		//0.35 RAD = 20 DEG
	const float MAX_ACTION_DETECTION_DISTANCE 		= 2.0;		//meters
	const float MAX_ACTION_DETECTION_ANGLE_RAD 		= 1.3;		//1.3 RAD = ~75 DEG
	
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;
		if (!ctx.Read(m_CurtainStage )) 
		{
			return false;
		}
		return true;
	}
	
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		ctx.Write(m_CurtainStage );
	}
	
	override void AfterStoreLoad()
	{    
		super.AfterStoreLoad();
		m_CurtainStage;
		UpdateVisuals();
	}
	
	void ND_WindowBarricade()
	{
		
		ShowSimpleSelection("Open_Curtain", 0);
		ShowSimpleSelection("Closed_Curtain", 0);
		
		RegisterNetSyncVariableInt("m_CurtainStage")
	}
	
	void CloseCurtain()
	{
		ShowSimpleSelection("Open_Curtain", 0);
		ShowSimpleSelection("Closed_Curtain", 1);
		
		m_CurtainStage = 0;
	}
	void OpenCurtain()
	{
		ShowSimpleSelection("Open_Curtain", 1);
		ShowSimpleSelection("Closed_Curtain", 0);
		
		m_CurtainStage = 1;
	}
	string GetCurtainStage()
	{
        if (m_CurtainStage == 0)
		{
          return "Close Curtain";
        }
		if (m_CurtainStage == 1)
		{
          return "Open Curtain";
        }
		return "";
    };
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item,slot_name);
		if ( item.IsKindOf("WindowBarricadeKit") && slot_name == "NDCurtain_Open") // any attachment but coal
		{
			OpenCurtain()
		}
		else if ( item.IsKindOf("WindowBarricadeKit") && slot_name == "NDCurtain_Closed") // any attachment but coal
		{
			CloseCurtain()
		};
	};
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		

		if ( item.IsKindOf("WindowBarricadeKit")) // any attachment but coal
		{
			ShowSimpleSelection("Open_Curtain", 0);
			ShowSimpleSelection("Closed_Curtain", 0);
		};

	};
	
	void ShowSimpleSelection(string selectionName, bool hide = false)
    {
        TStringArray selectionNames = new TStringArray;
        ConfigGetTextArray("simpleHiddenSelections",selectionNames);
        int selectionId = selectionNames.Find(selectionName);
        SetSimpleHiddenSelectionState(selectionId, hide);
    };
	
	
	ItemBase FoldBaseBuildingObject()
	{
		ItemBase item = CreateConstructionKit();
		DestroyConstruction();
		return item;
	}
	override int GetMeleeTargetType()
	{
		return EMeleeTargetType.NONALIGNABLE;
	}
	
	//--- CONSTRUCTION KIT
	ItemBase CreateConstructionKit()
	{
		ItemBase construction_kit = ItemBase.Cast( GetGame().CreateObject( GetConstructionKitType(), GetKitSpawnPosition() ) );
		if ( m_ConstructionKitHealth > 0 )
		{
			construction_kit.SetHealth( m_ConstructionKitHealth );
		}
		
		return construction_kit;
	}
	
	override vector GetKitSpawnPosition()
	{
		if ( MemoryPointExists( "kit_spawn_position" ) )
		{
			vector position;
			position = GetMemoryPointPos( "kit_spawn_position" );
			
			return ModelToWorld( position );
		}
		
		return GetPosition();
	}
	
	override string GetConstructionKitType()
	{
		return "WindowBarricadeKit";
	}
	
	override bool CanDisplayAttachmentSlot( string slot_name )
	{
		return true;
	}

	override bool CanDisplayAttachmentCategory( string category_name )
	{
		return true;
	}	
	
	//returns true if attachment slot position is within given range
	override bool CheckSlotVerticalDistance( int slot_id, PlayerBase player )
	{	
		return true;		
	}
	//--- ATTACHMENT & CONDITIONS
	override bool CanReceiveAttachment( EntityAI attachment, int slotId )
	{
		//Remove super
		//if ( !super.CanReceiveAttachment(attachment, slotId) )
		//	return false;
		
		//manage action initiator (AT_ATTACH_TO_CONSTRUCTION)
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			if ( player )
			{
				ConstructionActionData construction_action_data = player.GetConstructionActionData();
				
				//reset action initiator
				construction_action_data.SetActionInitiator( NULL );				
			}
		}
		
		//Remove conditions
		//conditions
		//if ( attachment.Type() != ATTACHMENT_WOODEN_LOG )
		//{
		//	if ( !HasBase() )
		//	{
		//		return false;
		//	}
		//}
		
		return true;
	}
	//can put into hands
	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}
	
	override bool CanBeRepairedToPristine()
	{
		return true;
	}
		//returns true if player->mem_point position is within given range
	override bool CheckMemoryPointVerticalDistance( float max_dist, string selection, PlayerBase player )
	{
		if ( player )
		{
			//check vertical distance
			vector player_pos = player.GetPosition();
			vector pos;
			
			if ( MemoryPointExists( selection ) )
			{
				pos = ModelToWorld( GetMemoryPointPos( selection ) );
			}
			
			if ( Math.AbsFloat( player_pos[1] - pos[1] ) <= max_dist )
			{
				return true;
			}
			else
			{
				return false;
			}
		}			

		return true;
	}	
	// ---	
	
	override void OnPartBuiltServer( notnull Man player, string part_name, int action_id )
	{
		super.OnPartBuiltServer( player, part_name, action_id );
		//update visuals (server)
		UpdateVisuals();
	}
	
	override void OnPartDismantledServer( notnull Man player, string part_name, int action_id )
	{
		super.OnPartDismantledServer( player, part_name, action_id );
		//update visuals (server)
		UpdateVisuals();
	}
	
	override void OnPartDestroyedServer( Man player, string part_name, int action_id, bool destroyed_by_connected_part = false )
	{
		super.OnPartDestroyedServer( player, part_name, action_id );
		//update visuals (server)
		UpdateVisuals();
	}
		//--- ACTION CONDITIONS
	//returns dot product of player->construction direction based on existing/non-existing reference point
	override bool IsFacingPlayer( PlayerBase player, string selection )
	{
		vector fence_pos = GetPosition();
		vector player_pos = player.GetPosition();
		vector ref_dir = GetDirection();
		
		//vector fence_player_dir = player_pos - fence_pos;
		vector fence_player_dir = player.GetDirection();
		fence_player_dir.Normalize();
		fence_player_dir[1] = 0; 	//ignore height
		
		ref_dir.Normalize();
		ref_dir[1] = 0;			//ignore height
		
		if ( ref_dir.Length() != 0 )
		{
			float angle = Math.Acos( fence_player_dir * ref_dir );
			
			if ( angle >= MAX_ACTION_DETECTION_ANGLE_RAD )
			{
				return true;
			}
		}
		
		return false;
	}
		
	override bool IsFacingCamera( string selection )
	{
		vector ref_dir = GetDirection();
		vector cam_dir = GetGame().GetCurrentCameraDirection();
		
		//ref_dir = GetGame().GetCurrentCameraPosition() - GetPosition();
		ref_dir.Normalize();
		ref_dir[1] = 0;		//ignore height
		
		cam_dir.Normalize();
		cam_dir[1] = 0;		//ignore height
		
		if ( ref_dir.Length() != 0 )
		{
			float angle = Math.Acos( cam_dir * ref_dir );
			
			if ( angle >= MAX_ACTION_DETECTION_ANGLE_RAD )
			{
				return true;
			}
		}

		return false;
	}
	override bool IsPlayerInside( PlayerBase player, string selection )
	{

		if ( !HasProperDistance( "center", player ) && !HasProperDistance( "center2", player ) && !HasProperDistance( "center3", player ) )
		{
			return false;
		}
		
		return true;
	}
	override bool HasProperDistance( string selection, PlayerBase player )
	{
		if ( MemoryPointExists( selection ) )
		{
			vector selection_pos = ModelToWorld( GetMemoryPointPos( selection ) );
			float distance = vector.Distance( selection_pos, player.GetPosition() );
			if ( distance >= 1.4 )
			{
				return false;
			}
		}
			
		return true;
	}
	bool HasProperDistanceDestory( string selection, PlayerBase player )
	{
		if ( MemoryPointExists( selection ) )
		{
			vector selection_pos = ModelToWorld( GetMemoryPointPos( selection ) );
			float distance = vector.Distance( selection_pos, player.GetPosition() );
			if ( distance >= 2 )
			{
				return false;
			}
		}
		return true;
	}
}
class PM_MarketKit extends ItemBase
{
	
	//================================================================
	// ADVANCED PLACEMENT
	//================================================================	
	
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0"  )
	{
		super.OnPlacementComplete( player, position, orientation );
		
		if ( GetGame().IsServer() )
		{
			PlayerBase player_base = PlayerBase.Cast( player );

			PM_MarketStand MarketStall = PM_MarketStand.Cast( GetGame().CreateObjectEx( "PM_MarketStand", GetPosition(), ECE_PLACE_ON_SURFACE ) );
			
			MarketStall.SetPosition( position);
			MarketStall.SetOrientation( orientation );
			
			//make the kit invisible, so it can be destroyed from deploy UA when action ends
			HideAllSelections();
			
			this.Delete();
			SetIsDeploySound( true );
		}	
	}
	
	override string GetPlaceSoundset()
	{
		return "seachest_drop_SoundSet";
	}
	
	override bool IsDeployable()
	{
		return true;
	}
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionDeployObject);
	}
};