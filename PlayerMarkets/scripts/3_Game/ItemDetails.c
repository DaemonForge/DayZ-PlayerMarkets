class PlayerMarketItemDetailsBase extends Managed {

	protected int m_b1;
	protected int m_b2;
	protected int m_b3;
	protected int m_b4;
	
	protected string m_ItemClass = "";
	protected string m_ItemName = "";
	
	
	protected int m_Price = -1;
	protected int m_MaxPrice = -1;
	
	
	protected string m_ListerGUID = "";
	
	bool Is(int b1, int b2, int b3, int b4){
		return (b1 == m_b1 && b2 == m_b2 && b3 == m_b3 && b4 == m_b4);
	}
	
	void GetIds(out int b1,out int b2,out int b3,out int b4){
		b1 = m_b1;
		b2 = m_b2;
		b3 = m_b3;
		b4 = m_b4;
	}
	
	int GetPrice(){
		return m_Price;
	}
	
	void SetPrice(int price){
		m_Price = price;
	}
	
	string GetLister(){
		return m_ListerGUID;
	}
	
	EntityAI GetItem(){
		return NULL;
	}
}