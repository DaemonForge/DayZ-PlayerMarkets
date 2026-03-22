modded class PlayerBase extends ManBase
{
    override void Init()
    {
        if ( !g_Game.IsServer() || !g_Game.IsMultiplayer() ) 
	    {
	        DayzPlayerItemBehaviorCfg     toolsOneHanded = new DayzPlayerItemBehaviorCfg;
	        toolsOneHanded.SetToolsOneHanded();
			GetDayZPlayerType().AddItemInHandsProfileIK("PM_CopperCoin", "dz/anims/workspaces/player/player_main/props/player_main_1h_keys.asi", toolsOneHanded,	"dz/anims/anm/player/ik/gear/handcuff_keys.anm");
			GetDayZPlayerType().AddItemInHandsProfileIK("PM_SilverCoin", "dz/anims/workspaces/player/player_main/props/player_main_1h_keys.asi", toolsOneHanded,	"dz/anims/anm/player/ik/gear/handcuff_keys.anm");
			GetDayZPlayerType().AddItemInHandsProfileIK("PM_GoldCoin", "dz/anims/workspaces/player/player_main/props/player_main_1h_keys.asi", toolsOneHanded,	"dz/anims/anm/player/ik/gear/handcuff_keys.anm");
		}
        super.Init();
    }
	
	override void SetActions(out TInputActionMap InputActionMap){
		super.SetActions(InputActionMap);
		AddAction(ActionOpenConcierge, InputActionMap);
	}
}