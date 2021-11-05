class PlayerMarketItemDetails extends PlayerMarketItemDetailsBase {
	
	void PlayerMarketItemDetails(EntityAI item, int price, EntityAI stall){
		m_Items = new map<int, EntityAI>;
		EntityAI theItem = EntityAI.Cast(item);
		if (theItem){
			AddItem(theItem);
		}
	}
	
	[NonSerialized()]
	protected autoptr map<int, EntityAI> m_Items;
	
	bool AddItem(EntityAI item){
		if (!m_Items){
			m_Items = new map<int, EntityAI>; }
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
		int idx = m_b1.Insert(b1);
		m_b2.Insert(b2);
		m_b3.Insert(b3);
		m_b4.Insert(b4);
		m_Items.Insert(idx,item);
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
		return m_Items.GetElement(0);
	}
}