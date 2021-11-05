class PlayerMarketItemDetailsBase extends Managed {

	protected autoptr TIntArray m_b1;
	protected autoptr TIntArray m_b2;
	protected autoptr TIntArray m_b3;
	protected autoptr TIntArray m_b4;
	
	protected string m_ItemClass = "";
	protected string m_ItemName = "";
	
	protected int m_Quantiy = 1;
	
	protected int m_Price = -1;
	
	
	protected string m_ListerGUID = "";
	protected autoptr TStringIntMap m_BuyerGUIDs;
	
	protected int m_ViewTimes = 0;
	
	protected int m_QuanitySold = 0;
	
	EntityAI GetItem(){
		return NULL;
	}
}