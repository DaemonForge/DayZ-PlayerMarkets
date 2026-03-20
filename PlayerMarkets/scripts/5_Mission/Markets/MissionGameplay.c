modded class MissionGameplay 
{
	void MissionGameplay()
	{
		GetDayZGame().Event_OnRPC.Insert(HandlePMRPC);
	}

	void ~MissionGameplay()
	{
		GetDayZGame().Event_OnRPC.Remove(HandlePMRPC);
	}

	override void OnMissionStart(){
		super.OnMissionStart();
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.RequestPMConfig, Math.RandomInt(200,600));
	}
	
	void RequestPMConfig(){
		Print("[PlayerMarkets][Client] Requesting Config From Server");
		GetGame().RPCSingleParam(NULL, PLAYER_MARKET_CONFIG, new Param1<bool>(true), true, NULL);
	}
	
	void HandlePMRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		if (rpc_type != PLAYER_MARKET_CONFIG || !g_Game.IsClient()) {
			return;
		}

		Param1< PlayerMarketsConfig > data;
		if ( !ctx.Read( data ) ) return;
		Print("[PlayerMarkets][Client] Received Config From Server");
		PlayerMarketsConfig.m_Config = data.param1;
		PlayerMarketsConfig.m_Config.LoadCurrencys();
	}
	
}