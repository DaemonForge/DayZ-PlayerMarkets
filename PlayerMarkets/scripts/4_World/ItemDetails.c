class PlayerMarketItemDetails extends PlayerMarketItemDetailsBase {
	
	void PlayerMarketItemDetails(EntityAI item, int price, EntityAI stall){
		AddItem(item)
		SetPrice(price);
	}
	
	[NonSerialized()]
	protected EntityAI m_Item;
	
	bool AddItem(EntityAI item){
		if (m_ItemClass == ""){
			m_ItemClass = item.GetType();
			m_ItemName = item.GetDisplayName();
		} else if (m_ItemClass != item.GetType()) {
			return false;
		}
		int b1,b2,b3,b4;
		ItemBase itemB = ItemBase.Cast(item);
		if (itemB && GetGame().IsClient()){
			itemB.GetSyncedPersistantsId(b1,b2,b3,b4);
		} else {
			item.GetPersistentID(b1,b2,b3,b4);
		}
		m_b1 = b1;
		m_b2 = b2;
		m_b3 = b3;
		m_b4 = b4;
		Class.CastTo(m_Item,item);
		return true;
	}
		
	bool CheckAndSetItem(EntityAI item){
		if (item.GetType() != m_ItemClass) {return false;}
		ItemBase itemB = ItemBase.Cast(item);
		int b1,b2,b3,b4;
		if (itemB && GetGame().IsClient()){
			itemB.GetSyncedPersistantsId(b1,b2,b3,b4);
		} else {
			item.GetPersistentID(b1,b2,b3,b4);
		}
		if (m_b1 == b1 && m_b2 == b2 && m_b3 == b3 && m_b4 == b4){
			Class.CastTo(m_Item,item);
			Print(m_Item);
			return true;
		}
		return false;
	}
	
	override EntityAI GetItem(){
		if (!m_Item) {return NULL;}
		return m_Item;
	}
}