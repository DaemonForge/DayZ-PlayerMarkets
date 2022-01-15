class PlayerMarketsConfig extends Managed {
	
	protected static string ConfigDIR = "$profile:PlayerMarkets";
	protected static string ConfigPATH = ConfigDIR + "\\Config.json";
	static ref PlayerMarketsConfig m_Config;
	string ConfigVersion = "0";
	
	ref array<ref PMCurrencyMap> Currencies = new array<autoptr PMCurrencyMap>;
	ref TStringArray BlackList = {"PM_GoldCoin", "PM_SilverCoin", "PM_CopperCoin"};
	int GUIOption = 0;
	int MarketTableSaleSlots = 8;
	int MarketStallLv1SaleSlots = 16;
	int MarketStallLv2SaleSlots = 32;
	string LoggingChannel = "";
	
	bool ManagedTypesFile = true;
	
	void Load(){
		if (GetGame().IsServer()){
			if (FileExist(ConfigPATH)){ // If config exist load File
			    UApiJSONHandler<PlayerMarketsConfig>.FromFile(ConfigPATH, this);
				if (!BlackList){
					BlackList = {"PM_GoldCoin", "PM_SilverCoin", "PM_CopperCoin"};
					Save();
				}
				LoadCurrencys();
				
			} else { // File does not exist create file	
				MakeDirectory(ConfigDIR);
				Currencies.Insert(new PMCurrencyMap);
				LoadCurrencys();
				Save();
			}
		}
	}
	
	void Save(){
		UApiJSONHandler<PlayerMarketsConfig>.ToFile(ConfigPATH, this);
	}
	
	void LoadCurrencys(){
		for (int i = 0; i < Currencies.Count(); i++){
			UCurrency.InsertNew(Currencies.Get(i).CurrencyName, Currencies.Get(i).Values);
		}
	}
	
	
	int GetCurrencyIdx(string currencyName){
		for (int i = 0; i < Currencies.Count(); i++){
			if (Currencies.Get(i).CurrencyName == currencyName){
				return i;
			}
		}
		return -1;
	}
	
}

class PMCurrencyMap extends Managed {
	
	string CurrencyName = "Coins";
	string CurrencyType = "coin";
	autoptr array<autoptr UCurrencyValue> Values = new array<autoptr UCurrencyValue>;
	
	void PMCurrencyMap(){
		Values = new UCurrency;
		Values.Insert(new UCurrencyValue("PM_GoldCoin", 1000));
		Values.Insert(new UCurrencyValue("PM_SilverCoin", 100));
		Values.Insert(new UCurrencyValue("PM_CopperCoin", 1));
	}
	
}

//Helper function to return Config
static PlayerMarketsConfig GetPMConfig()
{
	if ( GetGame().IsServer() ){
		if (!PlayerMarketsConfig.m_Config)
		{
			PlayerMarketsConfig.m_Config = new PlayerMarketsConfig;
			PlayerMarketsConfig.m_Config.Load();
		}
	}
	return PlayerMarketsConfig.m_Config;
};