class PM_Merchant_Base extends ItemBase {
	
	protected autoptr map<string, int> m_MerchantSlots = new map<string,int>;
	
	void PM_Merchant_Base(){
		TStringArray attachmentSlots = new TStringArray;
		GetAttachmentSlots(this, attachmentSlots);
		for (int i = 0; i < attachmentSlots.Count(); i++){
			m_MerchantSlots.Set(attachmentSlots.Get(i), InventorySlots.GetSlotIdFromString(attachmentSlots.Get(i)));
		}
	}
	
	void ~PM_Merchant_Base(){
	}
	
	//--- ATTACHMENT SLOTS
	void GetAttachmentSlots( EntityAI entity, out array<string> attachment_slots )
	{
		string config_path = "CfgVehicles" + " " + entity.GetType() + " " + "attachments";
		if ( GetGame().ConfigIsExisting( config_path ) )
		{
			GetGame().ConfigGetTextArray( config_path, attachment_slots );
		}
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item,slot_name);
		
		bool shouldDelete = true;
		if (m_MerchantSlots.Count() > 1){
			for (int i = 0; i < m_MerchantSlots.Count(); i++){
				if (GetInventory().FindAttachment(m_MerchantSlots.GetElement(i)) != NULL){
					shouldDelete = false;
				}
			}
		}
		if (shouldDelete){
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().ObjectDelete,this);
		}
	}
}
 //m_OwnerGUID, m_StandName, m_MoneyBalance, m_AuthorizedSellers, m_ItemsArray,m_CurrencyUsed
typedef Param6<string, string,autoptr TIntIntMap, autoptr TStringIntMap, autoptr array<autoptr PlayerMarketItemDetails>,string> PM_RPCSyncData;

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
	
	protected bool ENABLE_DEBUG_STALLS = false;
	
	const float MIN_ACTION_DETECTION_ANGLE_RAD 		= 0.35;		//0.35 RAD = 20 DEG
	const float MAX_ACTION_DETECTION_DISTANCE 		= 2.0;		//meters
	const float MAX_ACTION_DETECTION_ANGLE_RAD 		= 1.3;		//1.3 RAD = ~75 DEG
	
	
	protected bool m_IsSellerInterfaceOpen = false;
	
	
	protected string m_OwnerGUID = "";
	protected string m_StandName = "";
	protected string m_CurrencyUsed = "Coins";
	protected int m_LastTaxedDate = 0;
	
	protected autoptr TIntIntMap m_MoneyBalance = new TIntIntMap;
	
	protected autoptr TStringIntMap m_AuthorizedSellers = new TStringIntMap;
	
	protected autoptr TStringMap m_Notes = new TStringMap;
	
	protected autoptr array<autoptr PlayerMarketItemDetails> m_ItemsArray = new array<autoptr PlayerMarketItemDetails>;
	protected autoptr TStringSet m_Visitors = new TStringSet;
	protected int m_Vists = 0;
	protected bool m_MarketIsInit = false;
	protected bool m_IsBuilt = false;
	protected bool m_IsInUse = false;
	protected autoptr map<string, int> m_MerchantSlots = new map<string,int>;
	
	
	autoptr map<string,string> m_AvailableProxies = new map<string,string>;
	
	void MarketStandBase(){
		RegisterNetSyncVariableBool("m_IsBuilt");
		RegisterNetSyncVariableBool("m_IsInUse");
		InitMarketData();
	}
	
	protected void InitMarketData(){
		TStringArray attachmentSlots = new TStringArray;
		GetAttachmentSlots(this, attachmentSlots);
		
		for (int i = 0; i < attachmentSlots.Count(); i++){
			m_MerchantSlots.Set(attachmentSlots.Get(i), InventorySlots.GetSlotIdFromString(attachmentSlots.Get(i)));
		}
		
		RegisterProxyItem("Shoulder1", "PM_Merchant_Guns");
		RegisterProxyItem("Knife", "PM_Merchant_Knife");
		RegisterProxyItem("Headgear", "PM_Merchant_Headgear");
		RegisterProxyItem("Pistol", "PM_Merchant_Pistol");
		RegisterProxyItem("StallMag", "PM_Merchant_Magazine");
		RegisterProxyItem("StallFood", "PM_Merchant_StallFood");
		RegisterProxyItem("Belt_Left", "PM_Merchant_StallCanteen");
		RegisterProxyItem("tripWireAttachment", "PM_Merchant_Grenade");
		RegisterProxyItem("Melee", "PM_Merchant_Melee");
		RegisterProxyItem("StallWater", "PM_Merchant_StallWater");
		RegisterProxyItem("StallFoodAlt", "PM_Merchant_StallFoodAlt");
	} 
	
	
	/*
	--------------------------------------------------------------------------------------------	
	
	
	MARKET
	
	--------------------------------------------------------------------------------------------	
	*/	
	
	void RegisterProxyItem(string slot_name, string itemType){
		if (!m_AvailableProxies){m_AvailableProxies = new map<string,string>;}
		m_AvailableProxies.Set(slot_name, itemType);
	}
	
	
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
		if (!m_MoneyBalance || m_MoneyBalance.Count() == 0){
			return 0;
		}
		TIntArray balance = m_MoneyBalance.GetValueArray();
		int total;
		foreach (int subtotal : balance){
			total+= subtotal;
		}
		return total;
	}
	
	string GetCurrencyUsed(){
		return m_CurrencyUsed;
	}
	
	void SetCurrencyUsed(string currency){
		m_CurrencyUsed = currency;
		if (GetGame().IsClient()){
			RPCSingleParam(PLAYER_MARKET_EDIT,new Param1<string>(m_CurrencyUsed),true);
		} else {
			SyncPMData();
		}
	}
	
	void WithdrawBalance(PlayerBase player){
		if (!player){return;}
		player.UAddMoney(m_CurrencyUsed, GetMoneyBalance());
		ResetMoneyBalance();
	}
	
	void IncreaseMoneyBalance(int amount){
		int date = UUtil.GetUnixInt();
		int subtotal = 0;
		m_MoneyBalance.Find(date,subtotal);
		subtotal += amount;
		m_MoneyBalance.Set(date,subtotal);
		SyncPMData();
	}
	
	protected void ResetMoneyBalance(){
		m_MoneyBalance = new TIntIntMap;
		SyncPMData();
	}
	
	protected void DoTaxation(){
		int date = UUtil.GetUnixInt();
		if (m_LastTaxedDate >= date || m_MoneyBalance.Count() > 0) return;
		
		for (int i = 0; i < m_MoneyBalance.Count();i++){
			int theDate = m_MoneyBalance.GetKey(i);
			if ((theDate + GetPMConfig().FreeTaxDays) > date){
				int subtotal = m_MoneyBalance.GetElement(i);
				int tax = Math.Ceil(subtotal * GetPMConfig().DailyTaxAmmount);
				subtotal -= tax;
				if (subtotal > 0){
					m_MoneyBalance.Set(theDate,subtotal);
				} else {
					m_MoneyBalance.Remove(theDate);
				}
			}
		}
		
	}
	
	int GetItemsForSaleCount(){
		if (!GetItemsArray()){
			return 0;
		}
		return GetItemsArray().Count();
	}
	
	int GetMaxItemsForSale(){
		return -1;
	}
	
	void InitStandData(){
		if (m_MarketIsInit) return;
		m_MarketIsInit = true;
		SyncPMData();
		
		array<EntityAI> items = GetItemsForSale();
		if (GetGame().IsServer()){
			DoTaxation();
			TIntArray ruinedItemIndexes = new TIntArray;
			for (int i = 0; i < m_ItemsArray.Count(); i++){ 
				PlayerMarketItemDetails detail = PlayerMarketItemDetails.Cast(m_ItemsArray.Get(i));
				foreach (EntityAI item : items){
					if (detail.CheckAndSetItem(item)){
						if (item.IsRuined()){
							ruinedItemIndexes.Insert(i);
						}
						items.RemoveItem(item);//Remove it from array to improve performance on adding more items
						break;
					}
				}
			}
			if (ruinedItemIndexes.Count() > 0){
				int max = ruinedItemIndexes.Count() - 1;
				for (int j = max; max >= 0; j--){
					m_ItemsArray.RemoveOrdered(ruinedItemIndexes.Get(j));
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
		return m_ItemsArray;
	}
	
	
	bool FindAndAttachSuitableProxy(EntityAI item){
		TStringArray slots = new TStringArray;
		UUtil.GetConfigTStringArray(item.GetType(), "inventorySlot", slots);
		foreach (string slotname : slots){
			if (slotname == "Melee" && (!item.IsMeleeWeapon() || item.IsInherited(Weapon_Base))){ //Temp fix for guns not going in the right slot
				
			} else {
				string itemType = "";
				if (m_AvailableProxies.Find(slotname, itemType)){
					PM_Merchant_Base attachment = PM_Merchant_Base.Cast(GetInventory().CreateAttachment(itemType));
					if (attachment){
						if (attachment.GetInventory().TakeEntityToInventory(InventoryMode.SERVER,FindInventoryLocationType.ANY, item)){
							return true;
						} else {
							GetGame().ObjectDelete(attachment);
						}
					}
				}
			}
		}
		return false;
	}
	
	bool AddItemForSale(EntityAI item, int price, PlayerBase player){
		if (!IsOwner(player)){ return false; }
		if (GetItemsForSaleCount() >= GetMaxItemsForSale()) {return false;}
		if (!m_ItemsArray){
			m_ItemsArray = new array<autoptr PlayerMarketItemDetails>;
		}
		m_ItemsArray.Insert(new PlayerMarketItemDetails(EntityAI.Cast(item),price, this));
		if (!FindAndAttachSuitableProxy(item)){
			if (GetMerchantStorage()){
				GetMerchantStorage().ServerTakeEntityToCargo(item);
			}
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
			UUtil.SendNotification("Warning", "Is not owner", player.GetIdentity());
			return false;
		}
		int b1, b2, b3, b4;
		item.GetIds(b1, b2, b3, b4);
		PlayerMarketItemDetails details = GetRightDetails(b1, b2, b3, b4);
		if (!details){
			UUtil.SendNotification("Warning", "Error Getting Item Details from server", player.GetIdentity());
			return false;
		}
		EntityAI entity = details.GetItem();
		if (!entity){
			array<EntityAI> items = GetItemsForSale();
			foreach (EntityAI itemL : items){
				details.CheckAndSetItem(itemL);
			}
			entity = details.GetItem();
			if (!entity){
				UUtil.SendNotification("Warning", "Error Getting Item from server", player.GetIdentity());
				return false;
			}
		}
		if (entity.GetHierarchyRoot() == this){
			ServerTakeEntityToCargo(entity);
			if (entity.GetHierarchyParent() != this){
				this.GetInventory().DropEntity(InventoryMode.SERVER, player, entity);
				vector pos = player.GetPosition() + "0 0.05 0";
				entity.SetPosition(pos);
				entity.PlaceOnSurface();
				UUtil.SendNotification("Warning", entity.GetDisplayName() + " Placed on Ground", player.GetIdentity());
			}
		}
		if (entity.GetHierarchyParent() == this || entity.GetHierarchyRoot() == entity || entity.GetHierarchyRoot() != this){
			m_ItemsArray.RemoveItem(details);
			SyncPMData();
			return true;
		}
		return false;
	}
	
	bool SellItem(PlayerMarketItemDetails item, PlayerBase player){
		if (!m_ItemsArray || !player || !item){
			return false;
		}
		if (player.GetIdentity()){
			if (player.GetIdentity().GetId() == m_OwnerGUID){				
				UUtil.SendNotification("Warning", "#CANTBUYOWN", player.GetIdentity());
				return false;
			}
		}
		int b1, b2, b3, b4;
		item.GetIds(b1, b2, b3, b4);
		PlayerMarketItemDetails details = GetRightDetails(b1, b2, b3, b4);
		if (!details){
			UUtil.SendNotification("Warning", "Error Getting Item Details from server", player.GetIdentity());
			return false;
		}
		if (player.UGetPlayerBalance(m_CurrencyUsed) < details.GetPrice()){
			
			UUtil.SendNotification("Warning", "Not Enought Money", player.GetIdentity());
			return false;
		}
		EntityAI entity = details.GetItem();
		if (!entity){
			array<EntityAI> items = GetItemsForSale();
			foreach (EntityAI itemL : items){
				details.CheckAndSetItem(itemL);
			}
			entity = details.GetItem();
			if (!entity){
				UUtil.SendNotification("Warning", "Error Getting Item from server", player.GetIdentity());
				return false;
			}
		}
		if (entity.GetHierarchyRoot() == this){
			this.GetInventory().DropEntity(InventoryMode.SERVER, this, entity);
			player.GetHumanInventory().TakeEntityToInventory(InventoryMode.SERVER, FindInventoryLocationType.ANY, entity);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.CheckIfOnGround,350, false, entity,player);
		}
		if (entity.GetHierarchyRoot() == entity || entity.GetHierarchyRoot() == player){
			int pricewTax = details.GetPrice();
			int price = details.GetPrice();
			if (GetPMConfig().SaleTaxAmount > 0){
				pricewTax+= pricewTax * GetPMConfig().SaleTaxAmount;
			}
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(player.URemoveMoney, 200, false, m_CurrencyUsed, pricewTax);
			m_ItemsArray.RemoveItem(details);
			IncreaseMoneyBalance(price);
			SyncPMData();
			UUtil.SendNotification("Player Markets", entity.GetDisplayName() + " Bought", player.GetIdentity());
			OnItemSold(entity, price,player);
			return true;
		}
		return false;
	}
	
	void OnItemSold(EntityAI item, int price, PlayerBase player){
		string name =  Widget.TranslateString(item.GetDisplayName());
		UApi().ds().UserSend(m_OwnerGUID, "You Successfully Sold " + name + " for $" + price);
		if (GetPMConfig().LoggingChannel != "" && player && player.GetIdentity()){
			string adminMessage = GetStandName() + "(" + m_OwnerGUID + ") sold " + name + " for $" + price + " to " + player.GetIdentity().GetName() + " (" + player.GetIdentity().GetId() + ")";
			UApi().ds().ChannelSend(GetPMConfig().LoggingChannel, adminMessage); 
		}
	}
	
	void CheckIfOnGround(EntityAI entity, PlayerBase player){
		if (entity.GetHierarchyRoot() == entity){
			entity.SetPosition(player.GetPosition());
			entity.PlaceOnSurface();
			UUtil.SendNotification("Warning", entity.GetDisplayName() + " Placed on Ground", player.GetIdentity());
		}
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
	
	
	override bool CanReceiveItemIntoCargo( EntityAI item )
	{
		//Print("CanReceiveItemIntoCargo: " + item.GetType());
		if (GetPMConfig().BlackList.Find(item.GetType()) != -1){
			return false;
		}
		
		return super.CanReceiveItemIntoCargo( item );
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
				OnSyncClient(syncData.param1, syncData.param2,syncData.param3, syncData.param4, syncData.param5, syncData.param6);
			}
			return;
		}
		if (rpc_type == PLAYER_MARKET_SYNC && GetGame().IsServer() && sender) {
			SyncPMData(sender);
			return;
		}
		if (rpc_type == PLAYER_MARKET_WITHDRAWN && GetGame().IsServer() && sender) {
			if ( IsOwner(sender) && Class.CastTo(player,UUtil.FindPlayerByIdentity(sender)) ){
				WithdrawBalance(player);
			}
			return;
		}
		if (rpc_type == PLAYER_MARKET_INUSE && GetGame().IsServer() && sender) {
			Param1<bool> inuseSet;
			if (ctx.Read(inuseSet) && IsOwner(sender)){
				SetIsInUse(inuseSet.param1);
			}
			return;
		}
		if (rpc_type == PLAYER_MARKET_EDIT && GetGame().IsServer() && sender ) {
			Param1<string> editType;
			if (ctx.Read(editType) && IsOwner(sender)){
				SetCurrencyUsed(editType.param1);
			}
			
		}
		PM_RPCItemData marketData;
		array<EntityAI> items;
		autoptr PlayerMarketItemDetails details;
		if (rpc_type == PLAYER_MARKET_LIST && GetGame().IsServer() && sender && ctx.Read(marketData)) {
			if (Class.CastTo(details, marketData.param1)){
				items = GetItemsInCargo();
				foreach (EntityAI itemL : items){
					if (details.CheckAndSetItem(itemL)){
						AddItemForSale(itemL, details.GetPrice(), PlayerBase.Cast(UUtil.FindPlayerByIdentity(sender)));
					}
				}
			}
		}
		if (rpc_type == PLAYER_MARKET_DELIST && GetGame().IsServer() && sender && ctx.Read(marketData)) {
			if (Class.CastTo(details, marketData.param1)){
				if (DelistItem(details, PlayerBase.Cast(UUtil.FindPlayerByIdentity(sender)))){
					
				} else {
					
				}
			}
		}
		if (rpc_type == PLAYER_MARKET_BUY && GetGame().IsServer() && sender && ctx.Read(marketData)) {
			if (Class.CastTo(details, marketData.param1)){
				if ( SellItem(details, PlayerBase.Cast(UUtil.FindPlayerByIdentity(sender))) ){
					
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
	
	void OnSyncClient(string owner, string standname, TIntIntMap balance, TStringIntMap sellers, autoptr array<autoptr PlayerMarketItemDetails> itemDetails, string currencyUsed){
		m_OwnerGUID = owner;
		m_StandName = standname;
		m_MoneyBalance = balance;
		m_CurrencyUsed = currencyUsed;
		if (!m_AuthorizedSellers){ m_AuthorizedSellers = new TStringIntMap; }
		
		m_AuthorizedSellers.Copy(sellers);
		array<EntityAI> items = GetItemsForSale();
		array<autoptr PlayerMarketItemDetails> idetals = array<autoptr PlayerMarketItemDetails>.Cast(itemDetails);
		m_ItemsArray = new array<autoptr PlayerMarketItemDetails>;
		foreach (PlayerMarketItemDetails detail : idetals){
			foreach (EntityAI item : items){
				if (detail.CheckAndSetItem(item)){
					items.RemoveItem(item);//Remove it from array to improve performance on adding more items
					break;
				}
			}
			m_ItemsArray.Insert(detail);
		}
	}
	
	void SyncPMData(PlayerIdentity player = NULL){
		if (GetGame().IsClient()){
			RPCSingleParam(PLAYER_MARKET_SYNC, NULL, true);
		} else {
			RPCSingleParam(PLAYER_MARKET_SYNC, new PM_RPCSyncData(m_OwnerGUID,m_StandName, m_MoneyBalance, m_AuthorizedSellers,m_ItemsArray,m_CurrencyUsed), true, player);
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
		if (!ctx.Read( m_CurrencyUsed )) {
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
		if (!ctx.Read( m_LastTaxedDate )) {
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
		ctx.Write( m_CurrencyUsed );
		ctx.Write( m_Notes );
		ctx.Write( m_AuthorizedSellers );
		ctx.Write( m_ItemsArray );
		ctx.Write( m_Visitors );
		ctx.Write( m_Vists );
		ctx.Write( m_LastTaxedDate );
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
            return ENABLE_DEBUG_STALLS;
		}
        if (category_name  == "Signs" && GetGame().IsClient() ) {
            return true;
		}
		
		if ( category_name == "Table" && !HasBase() )
			return true;
		else if ( category_name == "Posts" && HasBase() && (!GetConstruction().IsPartConstructed("wall_base_down") && !GetConstruction().IsPartConstructed("posts")))
			return true;
		else if ( category_name == "Roof" && GetConstruction().IsPartConstructed("wall_base_down") && !GetConstruction().IsPartConstructed("wall_base_up") )
			return true;
		else if ( category_name == "Roof" && GetConstruction().IsPartConstructed("posts") && !GetConstruction().IsPartConstructed("roof") )
			return true;
		else
			return false;
		
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
			EntityAI attachment;
			if (m_MerchantSlots.Get("Material_WoodenPlanks") != -1){
				if ( Class.CastTo(attachment, GetInventory().FindAttachment(m_MerchantSlots.Get("Material_WoodenPlanks")) )) {
					
					this.GetInventory().DropEntity(InventoryMode.SERVER, player, attachment);
				}
			}
			if (m_MerchantSlots.Get("Material_Nails") != -1){
				if ( Class.CastTo(attachment, GetInventory().FindAttachment(m_MerchantSlots.Get("Material_Nails")) )) {
					
					this.GetInventory().DropEntity(InventoryMode.SERVER, player, attachment);
				}
			}
			if (m_MerchantSlots.Get("Material_WoodenLogs") != -1){
				if ( Class.CastTo(attachment, GetInventory().FindAttachment(m_MerchantSlots.Get("Material_WoodenLogs")) )) {
					
					this.GetInventory().DropEntity(InventoryMode.SERVER, player, attachment);
				}
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

