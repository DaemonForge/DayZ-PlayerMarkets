modded class MissionBase extends MissionBaseWorld
{
	override UIScriptedMenu CreateScriptedMenu(int id) {
        UIScriptedMenu menu = NULL;
        menu = super.CreateScriptedMenu(id);
        if (!menu) {
            switch (id) {
                case PLAYER_MARKET_MENU_BUY:
                    menu = new MarketStallMenu;
                    break;
            }
            if (menu) {
                menu.SetID(id);
            }
        }
        return menu;
    }
}