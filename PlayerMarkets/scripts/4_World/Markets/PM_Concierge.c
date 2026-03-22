// NPC Concierge - a human NPC placed in a market area to provide browsing/buying interface
class PM_ConciergeNPC extends PlayerBase {
	
	protected ref PMMarketArea m_LinkedArea;
	protected ref array<ref PMConciergeEntry> m_ConciergeEntries;
	
	void PM_ConciergeNPC(){
		m_ConciergeEntries = new array<ref PMConciergeEntry>;
		SetAllowDamage(false);
	}
	
	// Prevent all damage
	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef){
		return false;
	}
	
	override void EEKilled(Object killer){
		// Do nothing - NPC cannot die
	}
	
	override bool CanBeTargetedByAI(EntityAI ai){
		return false;
	}
	
	override bool IsInventoryVisible(){
		return false;
	}
	
	// Prevent NPC from performing any player actions
	override void SetActions(out TInputActionMap InputActionMap){
		// Intentionally empty - NPC doesn't perform actions
	}
	
	PMMarketArea GetLinkedArea(){
		if (!m_LinkedArea){
			m_LinkedArea = GetMarketAreaManager().GetMarketAreaAtPosition(GetPosition());
		}
		return m_LinkedArea;
	}
	
	// Client-side: set entries received from server
	void SetConciergeEntries(array<ref PMConciergeEntry> entries){
		m_ConciergeEntries = entries;
	}
	
	array<ref PMConciergeEntry> GetConciergeEntries(){
		return m_ConciergeEntries;
	}
	
	// Server: handle concierge sync request
	void SyncConciergeData(PlayerIdentity player){
		if (!g_Game.IsServer() || !player) return;
		
		PMMarketArea area = GetLinkedArea();
		if (!area){
			Print("[PlayerMarkets] Concierge has no linked market area at position: " + GetPosition().ToString());
			return;
		}
		
		array<ref PMConciergeEntry> entries = GetMarketAreaManager().BuildConciergeListings(area);
		RPCSingleParam(PLAYER_MARKET_CONCIERGE_SYNC, new PM_RPCConciergeSyncData(entries), true, player);
	}
	
	// Server: handle concierge buy request
	void HandleConciergeBuy(PMConciergeBuyRequest buyReq, PlayerIdentity sender){
		if (!g_Game.IsServer() || !sender || !buyReq) return;
		
		PMMarketArea area = GetLinkedArea();
		if (!area) return;
		
		MarketStandBase stall = GetMarketAreaManager().FindStallByPersistentID(area, buyReq.m_StallB1, buyReq.m_StallB2, buyReq.m_StallB3, buyReq.m_StallB4);
		if (!stall){
			UUtil.SendNotification("Concierge", "That stall is no longer available", sender);
			SyncConciergeData(sender);
			return;
		}
		
		stall.InitStandData();
		PlayerMarketItemDetails details = stall.GetRightDetails(buyReq.m_ItemB1, buyReq.m_ItemB2, buyReq.m_ItemB3, buyReq.m_ItemB4);
		if (!details){
			UUtil.SendNotification("Concierge", "Item no longer available at " + stall.GetStandName(), sender);
			SyncConciergeData(sender);
			return;
		}
		
		PlayerBase player = PlayerBase.Cast(UUtil.FindPlayerByIdentity(sender));
		if (!player) return;
		
		if (stall.SellItem(details, player)){
			SyncConciergeData(sender);
		} else {
			SyncConciergeData(sender);
		}
	}
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx){
		super.OnRPC(sender, rpc_type, ctx);
		
		if (rpc_type == PLAYER_MARKET_CONCIERGE_SYNC && g_Game.IsClient()){
			PM_RPCConciergeSyncData syncData;
			if (ctx.Read(syncData)){
				m_ConciergeEntries = syncData.param1;
				// Notify the open menu to refresh
				if (m_ConciergeMenu){
					m_ConciergeMenu.OnDataReceived(m_ConciergeEntries);
				}
			}
			return;
		}
		
		if (rpc_type == PLAYER_MARKET_CONCIERGE_SYNC && g_Game.IsServer() && sender){
			SyncConciergeData(sender);
			return;
		}
		
		if (rpc_type == PLAYER_MARKET_CONCIERGE_BUY && g_Game.IsServer() && sender){
			PM_RPCConciergeBuyData buyData;
			if (ctx.Read(buyData)){
				HandleConciergeBuy(buyData.param1, sender);
			}
			return;
		}
	}
	
	// Client: request data from server
	void RequestConciergeSync(){
		if (g_Game.IsClient()){
			RPCSingleParam(PLAYER_MARKET_CONCIERGE_SYNC, NULL, true);
		}
	}
	
	// Client: send buy request
	void RequestConciergeBuy(PMConciergeEntry entry){
		if (g_Game.IsClient() && entry){
			PMConciergeBuyRequest req = new PMConciergeBuyRequest(entry.m_StallB1, entry.m_StallB2, entry.m_StallB3, entry.m_StallB4, entry.m_ItemB1, entry.m_ItemB2, entry.m_ItemB3, entry.m_ItemB4);
			RPCSingleParam(PLAYER_MARKET_CONCIERGE_BUY, new PM_RPCConciergeBuyData(req), true);
		}
	}
}

// Survivor model variants - config.cpp maps these to vanilla survivor models for appearance
class PM_ConciergeM_Mirek: PM_ConciergeNPC {};
class PM_ConciergeM_Denis: PM_ConciergeNPC {};
class PM_ConciergeM_Boris: PM_ConciergeNPC {};
class PM_ConciergeF_Eva: PM_ConciergeNPC {};
class PM_ConciergeF_Linda: PM_ConciergeNPC {};
class PM_ConciergeF_Frida: PM_ConciergeNPC {};

// Legacy backward-compat stub for existing world objects
class PM_Concierge extends ItemBase {
	override bool IsInventoryVisible(){
		return false;
	}
	override bool CanPutIntoHands(EntityAI parent){
		return false;
	}
	override bool CanPutInCargo(EntityAI parent){
		return false;
	}
}
