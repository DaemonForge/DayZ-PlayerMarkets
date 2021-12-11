modded class ItemBase extends InventoryItem {
	
	protected int m_b1;
	protected int m_b2;
	protected int m_b3;
	protected int m_b4;
	
	void ItemBase(){
		RegisterNetSyncVariableInt("m_b1");
		RegisterNetSyncVariableInt("m_b2");
		RegisterNetSyncVariableInt("m_b3");
		RegisterNetSyncVariableInt("m_b4");
	}

	override void EEInit(){
		super.EEInit();
		if (GetGame().IsServer()){
			GetPersistentID(m_b1,m_b2,m_b3,m_b4);
			SetSynchDirty();
		}
	}
	
	override void EEOnAfterLoad(){
		super.EEOnAfterLoad();
		if (GetGame().IsServer()){
			GetPersistentID(m_b1,m_b2,m_b3,m_b4);
			SetSynchDirty();
		}
	}
	
	
	override void AfterStoreLoad()
	{    
		super.AfterStoreLoad();
		if (GetGame().IsServer()){
			GetPersistentID(m_b1,m_b2,m_b3,m_b4);
			SetSynchDirty();
		}
	}
	
	
	void GetSyncedPersistantsId(out int b1, out int b2, out int b3, out int b4){
		b1 = m_b1;
		b2 = m_b2;
		b3 = m_b3;
		b4 = m_b4;
	}
	
} 