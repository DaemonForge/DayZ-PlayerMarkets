modded class MissionGameplay 
{
	override void OnMissionStart(){
		super.OnMissionStart();
		GetRPCManager().AddRPC( "PlayerMarkets", "RPCPlayerMarketsConfig", this, SingeplayerExecutionType.Both );
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.RequestPMConfig, Math.RandomInt(200,600));
	}
	
	void RequestPMConfig(){
		Print("[PlayerMarkets][Client] Requesting Config From Server");
		GetRPCManager().SendRPC("PlayerMarkets", "RPCPlayerMarketsConfig", new Param1< PlayerMarketsConfig >( NULL ), true, NULL);
	}
	
	void RPCPlayerMarketsConfig( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target ) {
		Param1< PlayerMarketsConfig > data;
		if ( !ctx.Read( data ) ) return;
		Print("[PlayerMarkets][Client] Received Config From Server");
		PlayerMarketsConfig.m_Config = data.param1;
		PlayerMarketsConfig.m_Config.LoadCurrencys();
	}
	
}