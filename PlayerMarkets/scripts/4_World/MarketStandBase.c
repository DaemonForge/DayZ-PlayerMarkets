class PM_Merchant_Base extends ItemBase {

}
 //m_OwnerGUID, m_StandName, m_AuthorizedSellers, m_ItemsArray
typedef Param4<string, string, TStringIntMap, array<autoptr PlayerMarketItemDetails>> PM_RPCSyncData;

typedef Param1<PlayerMarketItemDetails> PM_RPCItemData;

class PM_MarketStorage extends PM_Merchant_Base {
	void PM_Merchant_Base(){
		if (GetGame().IsClient()){ //Doing it based on the market storage cause it will ensure all the items are loaded client side first
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.InitMarketsClient);
		}
	}
	
	void InitMarketsClient(){
		MarketStandBase stand;
		if (Class.CastTo(stand, GetHierarchyParent())){
			stand.InitStandData();
		}
	}
}

class MarketStandBase extends BaseBuildingBase  {
	
	const float MAX_FLOOR_VERTICAL_DISTANCE 		= 0.5;
	
	
	const float MIN_ACTION_DETECTION_ANGLE_RAD 		= 0.35;		//0.35 RAD = 20 DEG
	const float MAX_ACTION_DETECTION_DISTANCE 		= 2.0;		//meters
	const float MAX_ACTION_DETECTION_ANGLE_RAD 		= 1.3;		//1.3 RAD = ~75 DEG
	
	
	protected bool m_IsSellerInterfaceOpen = false;
	
	
	protected string m_OwnerGUID = "";
	protected string m_StandName = "";
	
	protected autoptr TStringIntMap m_AuthorizedSellers;
	
	protected autoptr TStringMap m_Notes;
	
	protected autoptr array<autoptr PlayerMarketItemDetails> m_ItemsArray;
	protected autoptr TStringSet m_Visitors;
	protected int m_Vists;
	protected bool m_MarketIsInit = false;
	protected bool m_IsBuilt = false;
	protected bool m_IsInUse = false;
	
	void MarketStandBase(){
		RegisterNetSyncVariableBool("m_IsBuilt");
		RegisterNetSyncVariableBool("m_IsInUse");
	}
	
	/*
	--------------------------------------------------------------------------------------------	
	
	
	MARKET
	
	--------------------------------------------------------------------------------------------	
	*/	
	
	
	
	string GetStandName(){
		return m_StandName;
	}
	
	void SetStandName(string name){
		m_StandName = name;
	}
	
	
	void InitStandData(){
		if (m_MarketIsInit) return;
		m_MarketIsInit = true;
		Print("InitStandData");
		GetItemsForSale().Debug();
		GetItemsInCargo().Debug();
		SyncPMData();
	}
	array<EntityAI> GetItemsForSale(){
		array<EntityAI> items = new array<EntityAI>;
		GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		if (!items || items.Count() < 1) return NULL;
		EntityAI item;
		array<EntityAI> returnItems = new array<EntityAI>;
		for (int i = 0; i < items.Count(); i++){
			if (Class.CastTo(item, items.Get(i)) && item != this &&  GetInventory().HasAttachment(item.GetHierarchyParent())){
				returnItems.Insert(item);
			}
		}
		return returnItems;
	}
	
	void SyncPMData(){
	
	}
	
	void OnSyncClient(string owner, string standname, TStringIntMap sellers, array<autoptr PlayerMarketItemDetails> itemDetails){
		m_OwnerGUID = owner;
		m_StandName = standname;
		if (!m_AuthorizedSellers){ m_AuthorizedSellers = new TStringIntMap; }
		m_AuthorizedSellers.Copy(sellers);
		foreach(PlayerMarketItemDetails detail : itemDetails){
			m_ItemsArray.Insert(detail);
		}
	}
	
	array<EntityAI> GetItemsInCargo(){
		array<EntityAI> items = new array<EntityAI>;
		GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		if (!items || items.Count() < 1) return NULL;
		EntityAI item;
		array<EntityAI> returnItems = new array<EntityAI>;
		for (int i = 0; i < items.Count(); i++){
			if (Class.CastTo(item, items.Get(i)) && item.GetHierarchyParent() == this && !GetInventory().HasAttachment(item)){
				returnItems.Insert(item);
			}
		}
		return returnItems;
	}
	
	array<autoptr PlayerMarketItemDetails> GetItemsArray(){
		return m_ItemsArray;
	}
	
	bool AddItemForSale(EntityAI item, int price, PlayerBase player){
		if (!m_ItemsArray){
			m_ItemsArray = new array<autoptr PlayerMarketItemDetails>;
		}
		m_ItemsArray.Insert(new PlayerMarketItemDetails(EntityAI.Cast(item),price, this));
		return true;
	}
	
	void AddVisitor(string player){
		if (!m_Visitors){
			m_Visitors = new TStringSet;
		}
		m_Visitors.Insert(player);
		m_Vists++;
	}
	
	void AddAuthorizedSeller(string seller, int perm){
		if (!m_AuthorizedSellers){
			m_AuthorizedSellers = new TStringIntMap;
		}
		m_AuthorizedSellers.Insert(seller, perm);
	}
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		if (rpc_type == PLAYER_MARKET_SYNC && GetGame().IsClient()) {
			
		}
		if (rpc_type == PLAYER_MARKET_SYNC && GetGame().IsServer() && sender) {
			
		}
		if (rpc_type == PLAYER_MARKET_LIST &&  GetGame().IsServer() && sender) {
			
		}
		if (rpc_type == PLAYER_MARKET_DELIST &&  GetGame().IsServer() && sender) {
			
		}
		if (rpc_type == PLAYER_MARKET_BUY &&  GetGame().IsServer() && sender) {
			
		}
		if (rpc_type == PLAYER_MARKET_EDIT &&  GetGame().IsServer() && sender) {
			
		}
	}
	
	
	void SetIsInUse(bool state){
		m_IsInUse = state;
		SetSynchDirty();
	}
	
	bool IsInUse(){
		return m_IsInUse;
	}
	
	/*
	--------------------------------------------------------------------------------------------	
	
	
	DAYZ STORE
	
	--------------------------------------------------------------------------------------------	
	*/	
	
	
	
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if ( !super.OnStoreLoad( ctx, version ) ) {
			return false;
		}
		if (!ctx.Read( m_OwnerGUID )) {
			return false;
		}
		if (!ctx.Read( m_StandName )) {
			return false;
		}
		if (!ctx.Read( m_Notes )) {
			return false;
		}
		if (!ctx.Read( m_AuthorizedSellers )) {
			return false;
		}
		if (!ctx.Read( m_ItemsArray )) {
			return false;
		}
		if (!ctx.Read( m_Visitors )) {
			return false;
		}
		if (!ctx.Read( m_Vists )) {
			return false;
		}
		return true;
	}
	
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		ctx.Write( m_OwnerGUID );
		ctx.Write( m_StandName );
		ctx.Write( m_Notes );
		ctx.Write( m_AuthorizedSellers );
		ctx.Write( m_ItemsArray );
		ctx.Write( m_Visitors );
		ctx.Write( m_Vists );
	}
	
	override void AfterStoreLoad()
	{    
		super.AfterStoreLoad();
		
		UpdateVisuals();
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.InitStandData);
		if (IsBuilt()){
			m_IsBuilt = true;
		} else {
			m_IsBuilt = false;
		}
		SetSynchDirty();
	}
	
	
	
	/*
	--------------------------------------------------------------------------------------------	
	
	
	BASEBUILDING
	
	--------------------------------------------------------------------------------------------	
	*/	
	
	
	
	bool IsBuilt(){
		Construction construction = GetConstruction();
		map<string, ref ConstructionPart> parts = construction.GetConstructionParts();
		bool isBuilt = true;
		for (int i = 0; i < parts.Count(); i++){
			isBuilt = (isBuilt && construction.IsPartConstructed(parts.GetKey(i)));
		}
		
		return isBuilt;
	}
	
	override bool CanDisplayAttachmentCategory(string category_name) {
        if (category_name  == "Attachments" && GetGame().IsClient() ) {
            //return false;
		}
		if (category_name == "Material" && m_IsBuilt){
			return false;
		}
        return super.CanDisplayAttachmentCategory(category_name);
    }
	
	override bool CanDisplayCargo(){
		if ((!m_IsBuilt || IsInUse()) && GetGame().IsClient() ){
			return false;
		}
		return super.CanDisplayCargo();
	}
	
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
		return "PM_MarketKit";
	}
	
	override bool CanDisplayAttachmentSlot( string slot_name )
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
		if (IsBuilt()){
			m_IsBuilt = true;
			GetInventory().CreateAttachment("PM_MarketStorage");
		} else {
			m_IsBuilt = false;
		}
		SetSynchDirty();
		//update visuals (server)
		UpdateVisuals();
	}
	
	override void OnPartDismantledServer( notnull Man player, string part_name, int action_id )
	{
		super.OnPartDismantledServer( player, part_name, action_id );
		//update visuals (server)
		if (IsBuilt()){
			m_IsBuilt = true;
		} else {
			m_IsBuilt = false;
		}
		SetSynchDirty();
		UpdateVisuals();
	}
	
	override void OnPartDestroyedServer( Man player, string part_name, int action_id, bool destroyed_by_connected_part = false )
	{
		super.OnPartDestroyedServer( player, part_name, action_id );
		//update visuals (server)
		if (IsBuilt()){
			m_IsBuilt = true;
		} else {
			m_IsBuilt = false;
		}
		SetSynchDirty();
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
			if ( distance >= 0.4 )
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
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item,slot_name);
	};
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

	};
	
	void ShowSimpleSelection(string selectionName, bool hide = false)
    {
        TStringArray selectionNames = new TStringArray;
        ConfigGetTextArray("simpleHiddenSelections",selectionNames);
        int selectionId = selectionNames.Find(selectionName);
        SetSimpleHiddenSelectionState(selectionId, hide);
    };
	
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionFoldBaseBuildingObject);
	}
}

