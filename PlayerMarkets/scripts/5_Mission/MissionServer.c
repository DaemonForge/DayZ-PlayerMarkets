modded class MissionServer extends MissionBase {

	override void OnMissionStart()
	{
		super.OnMissionStart();
		//string ModFile = "Crypto_types.xml";
		//string Path = "Crypto\\xmls\\";
		//CopyXmlFile(Path + ModFile, ModFile);
		Print("[PlayerMarkets] OnInit");
		GetPMConfig();
		GetRPCManager().AddRPC( "PlayerMarkets", "RPCPlayerMarketsConfig", this, SingeplayerExecutionType.Both );
	}
	
	
	
	
	void RPCPlayerMarketsConfig( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target ) {
		Print("RPCPlayerMarketsConfig");
		PlayerIdentity RequestedBy = PlayerIdentity.Cast(sender); 
		if (RequestedBy){
			GetRPCManager().SendRPC("PlayerMarkets", "RPCPlayerMarketsConfig", new Param1< PlayerMarketsConfig >( GetPMConfig() ), true, RequestedBy);
		}
	}
	
	
	/*
		Copy Xml Files to Mission director and add entry to cfg econmoy core
	
		PathToFile - Is the path to the file inside your mod structure 
	                 remember to add *.xml to the copy directly in addon buiilder
		
		FileName - This is the file name to save the xml as in the folder, recommended to use modname_type.xml
	    
		Type - The type of xml this is ("types","spawnabletypes","events")
		
	*/
	private void CopyXmlFile(string PathToFile, string FileName, string type = "types"){
		if (!MakeDirectory("$mission:\\ModTypes")) {
			Error2("", "Couldn't Make mod types folder");
			return;
		}		
		int i = 0;
		string cfgecPath =  "$mission:\\cfgeconomycore.xml";
		if (!FileExist("$mission:\\ModTypes\\" + FileName) && FileExist(PathToFile)){
			CopyFile(PathToFile,"$mission:\\ModTypes\\" + FileName);
		}
		string xmldata;
		if (!FileExist(cfgecPath)) {
			Error2("", "File At cfgeconomycore.xml could not be found");
			return;
		}
		FileHandle fhr = OpenFile(cfgecPath, FileMode.READ);
		string error;
		if (fhr) {
			string line;
			while (FGets(fhr, line) > 0) {
				xmldata = xmldata + "\n" + line;
			}
			CloseFile(fhr);
		} else {
			Error2("", "File At cfgeconomycore.xml could not be opened");
		}
		if (!xmldata.Contains(FileName )){
			TStringArray newXmlLines = new TStringArray;
			TStringArray xmlLines = new TStringArray;
			xmldata.Split("\n", xmlLines);
			bool Found = false;
			bool ChangeMade = false;
			for (i = 0; i < xmlLines.Count(); i++){
				if (xmlLines.Get(i).Contains("<ce folder=\"ModTypes\">")){
					Found = true;
					ChangeMade = true;
					newXmlLines.Insert(xmlLines.Get(i));
					newXmlLines.Insert("        <file name=\"" + FileName + "\" type=\"" + type + "\" />");
				} else if ((xmlLines.Get(i).Contains("</economycore>") || xmlLines.Get(i) == "</economycore>") && !Found){
					ChangeMade = true;
					newXmlLines.Insert(" ");
					newXmlLines.Insert(" ");
					newXmlLines.Insert("    <!-- Created by script its advised not to edit this manually -->");
					newXmlLines.Insert("    <ce folder=\"ModTypes\">");
					newXmlLines.Insert("        <file name=\"" + FileName + "\" type=\"" + type + "\" />");
					newXmlLines.Insert("    </ce>");
					newXmlLines.Insert(" ");
					newXmlLines.Insert(xmlLines.Get(i));
				} else {
					newXmlLines.Insert(xmlLines.Get(i));
				}
			}
			if (ChangeMade){
				FileHandle fhw = OpenFile(cfgecPath, FileMode.WRITE);
				if (!fhw) {
					Print("File At" + cfgecPath + " could not be updated");
					return;
				} 
				if (newXmlLines && newXmlLines.Count() > 0) {
					for (i = 0; i < newXmlLines.Count(); i++){
						FPrintln(fhw, newXmlLines.Get(i));
					}
				}
				CloseFile(fhw);
			}
		}
	}
}