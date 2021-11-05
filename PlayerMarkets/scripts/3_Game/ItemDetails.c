class PlayerMarketItemDetails extends Managed {

	protected string m_ItemClass = "";
	protected string m_ItemName = "";
	
	protected int m_Price = -1;
	
	
	protected string m_ListerGUID = "";
	protected autoptr TStringIntMap m_BuyerGUIDs;
	
	protected int m_ViewTimes = 0;
	
	protected int m_QuanitySold = 0;
	
	EntityAI GetItem(){
		return NULL;
	}
}

class PlayerMarketSoloItemDetails extends PlayerMarketItemDetails {
	
	protected int m_b1;
	protected int m_b2;
	protected int m_b3;
	protected int m_b4;
	
	
	[NonSerialized()]
	protected EntityAI m_Item;
	
	void SetItem(EntityAI item){
		item.GetPersistentID(m_b1,m_b2,m_b3,m_b4);
		
		Class.CastTo(m_Item, item);
		m_ItemClass = m_Item.GetType();
		m_ItemName = Widget.TranslateString(m_Item.GetDisplayName());
		Print(m_ItemName);
	}
	
	bool CheckAndSetItem(EntityAI item){
		if (item.GetType() != m_ItemClass) {return false;}
		int b1,b2,b3,b4;
		item.GetPersistentID(b1,b2,b3,b4);
		if (b1 == m_b1 && b2 == m_b2 && b3 == m_b3 && b4 == m_b4){
			if (Class.CastTo(m_Item, item)){
				return true;
			}
		}
		return false;
	}
	
	override EntityAI GetItem(){
		return m_Item;
	}
}

class PlayerMarketBulkItemDetails extends PlayerMarketItemDetails {
	
	protected autoptr TIntArray m_b1;
	protected autoptr TIntArray m_b2;
	protected autoptr TIntArray m_b3;
	protected autoptr TIntArray m_b4;
	
	protected int m_Quantiy = 1;
	
	[NonSerialized()]
	protected autoptr array<EntityAI> m_Items;
	
	bool AddItem(EntityAI item){
		if (!m_Items){
			m_Items = new array<EntityAI>; }
		if (!m_b1){
			m_b1 = new TIntArray; }
		if (!m_b2){
			m_b2 = new TIntArray; }
		if (!m_b3){
			m_b3 = new TIntArray; }
		if (!m_b4){
			m_b4 = new TIntArray; }
		if (m_ItemClass == ""){
			m_ItemClass = item.GetType();
			m_ItemName = item.GetDisplayName();
		} else if (m_ItemClass != item.GetType()) {
			return false;
		}
		int b1,b2,b3,b4;
		item.GetPersistentID(b1,b2,b3,b4);
		m_b1.Insert(b1);
		m_b2.Insert(b2);
		m_b3.Insert(b3);
		m_b4.Insert(b4);
		m_Items.Insert(item);
		return true;
	}
	
	bool CheckAndSetItem(EntityAI item){
		if (item.GetType() != m_ItemClass) {return false;}
		int b1,b2,b3,b4;
		item.GetPersistentID(b1,b2,b3,b4);
		for (int i = 0; i < m_b1.Count(); i++){
			if (m_b1.Get(i) == b1 && m_b2.Get(i) == b2 && m_b3.Get(i) == b3 && m_b4.Get(i) == b4){
				m_Items.Set(i,item);
				return true;
			}
		}
		return false;
	}
	
	override EntityAI GetItem(){
		if (!m_Items) {return NULL;}
		return m_Items.Get(0);
	}
}