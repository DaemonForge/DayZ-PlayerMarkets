class PM_Merchant_Base extends ItemBase {

}
 //m_OwnerGUID, m_StandName, m_MoneyBalance, m_AuthorizedSellers, m_ItemsArray
typedef Param5<string, string,int, autoptr TStringIntMap, autoptr array<autoptr PlayerMarketItemDetails>> PM_RPCSyncData;

typedef Param1<PlayerMarketItemDetails> PM_RPCItemData;

class PM_MarketStorage extends PM_Merchant_Base {
	void PM_MarketStorage(){
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
	
	protected int m_MoneyBalance = 0;
	
	protected autoptr TStringIntMap m_AuthorizedSellers = new TStringIntMap;
	
	protected autoptr TStringMap m_Notes = new TStringMap;
	
	protected autoptr array<autoptr PlayerMarketItemDetails> m_ItemsArray = new array<autoptr PlayerMarketItemDetails>;
	protected autoptr TStringSet m_Visitors = new TStringSet;
	protected int m_Vists = 0;
	protected bool m_MarketIsInit = false;
	protected bool m_IsBuilt = false;
	protected bool m_IsInUse = false;
	protected autoptr map<string, int> m_MerchantSlots = new map<string,int>;
	
	void MarketStandBase(){
		RegisterNetSyncVariableBool("m_IsBuilt");
		RegisterNetSyncVariableBool("m_IsInUse");
		TStringArray attachmentSlots = new TStringArray;
		GetAttachmentSlots(this, attachmentSlots);
		
		for (int i = 0; i < attachmentSlots.Count(); i++){
			m_MerchantSlots.Set(attachmentSlots.Get(i), InventorySlots.GetSlotIdFromString(attachmentSlots.Get(i)));
		}
	}
	
	/*
	--------------------------------------------------------------------------------------------	
	
	
	MARKET
	
	--------------------------------------------------------------------------------------------	
	*/	
	
	
	
	bool CanOpenSellMenu(PlayerBase player){
		return IsBuilt() && IsPlayerInside(player,"") && IsOwner(player);
	}
	
	bool CanOpenBuyMenu(PlayerBase player){
		return IsBuilt();
	}
	
	string GetStandName(){
		return m_StandName;
	}
	
	void SetStandName(string name){
		m_StandName = name;
	}
	
	int GetMoneyBalance(){
		return m_MoneyBalance;
	}
	
	void WithdrawBalance(PlayerBase player){
		if (!player){return;}
		player.UAddMoney("Coins", m_MoneyBalance);
		ResetMoneyBalance();
	}
	
	void IncreaseMoneyBalance(int amount){
		m_MoneyBalance += amount;
		SyncPMData();
	}
	
	protected void ResetMoneyBalance(){
		m_MoneyBalance = 0;
		SyncPMData();
	}
	
	void InitStandData(){
		if (m_MarketIsInit) return;
		m_MarketIsInit = true;
		SyncPMData();
		
		array<EntityAI> items = GetItemsForSale();
		Print(items);
		Print(m_ItemsArray);
		if (GetGame().IsServer()){
			for (int i = 0; i < m_ItemsArray.Count(); i++){ 
				PlayerMarketItemDetails detail = PlayerMarketItemDetails.Cast(m_ItemsArray.Get(i));
				foreach (EntityAI item : items){
					Print(detail);
					Print(item);
					if (detail.CheckAndSetItem(item)){
						items.RemoveItem(item);//Remove it from array to improve performance on adding more items
						break;
					}
				}
			}
		}
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
		Print("GetItemsArray");
		return m_ItemsArray;
	}
	
	bool AddItemForSale(EntityAI item, int price, PlayerBase player){
		if (!IsOwner(player)){ return false; }
		if (!m_ItemsArray){
			m_ItemsArray = new array<autoptr PlayerMarketItemDetails>;
		}
		m_ItemsArray.Insert(new PlayerMarketItemDetails(EntityAI.Cast(item),price, this));
		if (GetMerchantStorage()){
			GetMerchantStorage().ServerTakeEntityToCargo(item);
		}
		SyncPMData();
		return true;
	}
	
	PlayerMarketItemDetails GetRightDetails(int b1,int b2,int b3,int b4){
		if (!m_ItemsArray){
			return NULL;
		}
		for (int i = 0; i < m_ItemsArray.Count(); i++){
			if (m_ItemsArray.Get(i).Is(b1,b2,b3,b4)){
				return m_ItemsArray.Get(i);
			}
		}
		return NULL;
	}
	
	bool DelistItem(PlayerMarketItemDetails item, PlayerBase player){
		if (!m_ItemsArray || !player || !item){
			return false;
		}
		if (!IsOwner(player)){
			return false;
		}
		int b1, b2, b3, b4;
		item.GetIds(b1, b2, b3, b4);
		PlayerMarketItemDetails details = GetRightDetails(b1, b2, b3, b4);
		if (!details){
			return false;
		}
		EntityAI entity = item.GetItem();
		if (entity.GetHierarchyRoot() == this){
			ServerTakeEntityToCargo(entity);
			if (entity.GetHierarchyParent() != this){
				this.ServerDropEntity(entity);
				entity.SetPosition(player.GetPosition() + "0 0.05 0");
				entity.PlaceOnSurface()
			}
		}
		if (entity.GetHierarchyRoot() != this){
			m_ItemsArray.RemoveItem(details);
			return true;
		}
		return false;
	}
	
	bool SellItem(PlayerMarketItemDetails item, PlayerBase player){
		if (!m_ItemsArray || !player || !item){
			return false;
		}
		int b1, b2, b3, b4;
		item.GetIds(b1, b2, b3, b4);
		PlayerMarketItemDetails details = GetRightDetails(b1, b2, b3, b4);
		if (!details){
			return false;
		}
		if (player.UGetPlayerBalance("Coins") < details.GetPrice()){
			return false;
		}
		EntityAI entity = item.GetItem();
		if (entity.GetHierarchyRoot() == this){
			player.ServerTakeEntityToInventory(FindInventoryLocationType.ANY, entity);
			if (entity.GetHierarchyRootPlayer() != player){
				this.ServerDropEntity(entity);
				entity.SetPosition(player.GetPosition() + "0 0.05 0");
				entity.PlaceOnSurface()
			}
		}
		if (entity.GetHierarchyRoot() != this){
			int price = details.GetPrice();
			player.URemoveMoney("Coins", price);
			m_ItemsArray.RemoveItem(details);
			IncreaseMoneyBalance(price);
			return true;
		}
		SyncPMData();
		return false;
	}
	
	PM_Merchant_Base GetMerchantStorage(){
		if (m_MerchantSlots.Get("Merchant_Storage") != -1){
			return PM_Merchant_Base.Cast(GetInventory().FindAttachment(m_MerchantSlots.Get("Merchant_Storage")));	
		}
		return NULL;
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
	
	
	bool IsInUse(){
		return m_IsInUse;
	}
	
	
	bool IsOwner(PlayerBase player){
		if (player.GetIdentity()){
			return (m_OwnerGUID == player.GetIdentity().GetId());
		}
		return (m_OwnerGUID == "");
	}
	bool IsOwner(PlayerIdentity player){
		if (player){
			return (m_OwnerGUID == player.GetId());
		}
		return (m_OwnerGUID == "");
	}
	/*
	--------------------------------------------------------------------------------------------
	
	RPC Functions
	
	--------------------------------------------------------------------------------------------
	*/	
	
	
	void RequestWithdraw(){
		if (GetGame().IsClient()){
			RPCSingleParam(PLAYER_MARKET_WITHDRAWN, NULL,true);
		}
	}
	
	void RequestBuy(PlayerMarketItemDetails details){
		if (GetGame().IsClient()){
			RPCSingleParam(PLAYER_MARKET_BUY, new PM_RPCItemData(details),true);
		}
	}
	
	void RequestDeList(PlayerMarketItemDetails details){
		if (GetGame().IsClient()){
			RPCSingleParam(PLAYER_MARKET_DELIST, new PM_RPCItemData(details),true);
		}
	}
	
	void RequestList(PlayerMarketItemDetails details){
		if (GetGame().IsClient()){
			RPCSingleParam(PLAYER_MARKET_LIST, new PM_RPCItemData(details),true);
		}
	}
		
	void SetIsInUse(bool state){
		if (GetGame().IsClient()){
			RPCSingleParam(PLAYER_MARKET_INUSE, new Param1<bool>(state),true);
		} else {
			m_IsInUse = state;
			SetSynchDirty();
		}
	}
	
	
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		PlayerBase player;
		if (rpc_type == PLAYER_MARKET_SYNC && GetGame().IsClient()) {
			PM_RPCSyncData syncData;
			if (ctx.Read(syncData)){
				OnSyncClient(syncData.param1, syncData.param2,syncData.param3, syncData.param4, syncData.param5);
			}
			return;
		}
		if (rpc_type == PLAYER_MARKET_SYNC && GetGame().IsServer() && sender) {
			SyncPMData(sender);
			return;
		}
		if (rpc_type == PLAYER_MARKET_WITHDRAWN &&  GetGame().IsServer() && sender) {
			if ( IsOwner(sender) && Class.CastTo(player,UUtil.FindPlayerByIdentity(sender)) ){
				WithdrawBalance(player);
			}
			return;
		}
		if (rpc_type == PLAYER_MARKET_INUSE &&  GetGame().IsServer() && sender) {
			Param1<bool> inuseSet;
			if (ctx.Read(inuseSet) && IsOwner(sender)){
				SetIsInUse(inuseSet.param1);
			}
			return;
		}
		PM_RPCItemData marketData;
		array<EntityAI> items;
		autoptr PlayerMarketItemDetails details;
		if (rpc_type == PLAYER_MARKET_LIST &&  GetGame().IsServer() && sender && ctx.Read(marketData)) {
			if (Class.CastTo(details, marketData.param1)){
				items = GetItemsInCargo();
				foreach (EntityAI itemL : items){
					if (details.CheckAndSetItem(itemL)){
						AddItemForSale(itemL, details.GetPrice(), PlayerBase.Cast(UUtil.FindPlayerByIdentity(sender)));
					}
				}
			}
		}
		if (rpc_type == PLAYER_MARKET_EDIT &&  GetGame().IsServer() && sender && ctx.Read(marketData)) {
			if (Class.CastTo(details, marketData.param1)){
				items = GetItemsForSale();
				foreach (EntityAI itemE : items){
					if (details.CheckAndSetItem(itemE)){
						break;
					}
				}
			}
			
		}
		if (rpc_type == PLAYER_MARKET_DELIST &&  GetGame().IsServer() && sender && ctx.Read(marketData)) {
			if (Class.CastTo(details, marketData.param1)){
				if (DelistItem(details, UUtil.FindPlayerByIdentity(sender))){
					
				} else {
					
				}
			}
		}
		if (rpc_type == PLAYER_MARKET_BUY &&  GetGame().IsServer() && sender && ctx.Read(marketData)) {
			if (Class.CastTo(details, marketData.param1)){
				if (SellItem(details, UUtil.FindPlayerByIdentity(sender))){
					
				} else {
					
				}
			}
		}
	}
	
	/*
	--------------------------------------------------------------------------------------------
	
	DataSyncing
	
	--------------------------------------------------------------------------------------------
	*/
	
	void OnSyncClient(string owner, string standname, int balance, TStringIntMap sellers, autoptr array<autoptr PlayerMarketItemDetails> itemDetails){
		Print("OnSyncClient");
		m_OwnerGUID = owner;
		m_StandName = standname;
		m_MoneyBalance = balance;
		if (!m_AuthorizedSellers){ m_AuthorizedSellers = new TStringIntMap; }
		
		m_AuthorizedSellers.Copy(sellers);
		array<EntityAI> items = GetItemsForSale();
		array<autoptr PlayerMarketItemDetails> idetals = array<autoptr PlayerMarketItemDetails>.Cast(itemDetails);
		if (!m_ItemsArray){
			m_ItemsArray = new array<autoptr PlayerMarketItemDetails>
		}
		foreach (PlayerMarketItemDetails detail : idetals){
			foreach (EntityAI item : items){
				if (detail.CheckAndSetItem(item)){
					items.RemoveItem(item);//Remove it from array to improve performance on adding more items
					break;
				}
			}
			m_ItemsArray.Insert(detail);
		}
		m_ItemsArray.Debug();
	}
	
	void SyncPMData(PlayerIdentity player = NULL){
		if (GetGame().IsClient()){
			RPCSingleParam(PLAYER_MARKET_SYNC, NULL, true);
		} else {
			RPCSingleParam(PLAYER_MARKET_SYNC, new PM_RPCSyncData(m_OwnerGUID,m_StandName, m_MoneyBalance, m_AuthorizedSellers,m_ItemsArray), true, player);
		}
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
		if (!ctx.Read( m_MoneyBalance )) {
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
		ctx.Write( m_MoneyBalance );
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
		m_ItemsArray.Debug();
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
			PlayerBase playerB = PlayerBase.Cast(player);
			if (playerB && playerB.GetIdentity()){
				m_OwnerGUID = playerB.GetIdentity().GetId();
				m_StandName = playerB.GetIdentity().GetName() + "'s Market";
			}
			SyncPMData();
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
			m_OwnerGUID = "";
			m_StandName = "";
			SyncPMData();
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
			m_OwnerGUID = "";
			m_StandName = "";
			SyncPMData();
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
			if ( distance >= 0.5 )
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
		AddAction(ActionOpenMarketStallBuy);
		AddAction(ActionOpenMarketStallSell);
	}
}

