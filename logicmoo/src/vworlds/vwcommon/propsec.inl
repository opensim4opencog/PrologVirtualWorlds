private:
	// Migrated from IObjectProperty
	IWorld* m_pWorld;
	IPropertySecurity *m_pSecurity;

	IThing* m_pParent;
	CComBSTR m_bstrPropertyName;

public:

	STDMETHOD(get_World)(IWorld** ppWorld) 
	{ 
		*ppWorld = m_pWorld; 
		SAFEADDREF(*ppWorld); 
 
		return S_OK; 
	}; 
 
	STDMETHOD(put_World)(IWorld* pworld); 
 
	STDMETHOD(get_Parent)(IThing** ppThing) 
	{ 
		*ppThing = m_pParent; 
		SAFEADDREF(*ppThing); 

		return S_OK; 
	}; 
 
	STDMETHOD(put_Parent)(IThing* pthing) 
	{ 
		SAFERELEASE(m_pParent); 
		m_pParent = pthing; 
		SAFEADDREF(m_pParent); 
 
		return S_OK; 
	}; 

	STDMETHOD(get_Security)(IPropertySecurity** ppSecurity) 
	{ 
		*ppSecurity = m_pSecurity; 
		SAFEADDREF(*ppSecurity); 
		return S_OK; 
	}; 
 
	STDMETHOD(put_Security)(IPropertySecurity* pSecurity) 
	{ 
		SAFERELEASE(m_pSecurity); 
		m_pSecurity = pSecurity; 
		SAFEADDREF(m_pSecurity); 
 
		return S_OK; 
	}; 
 
	STDMETHOD(get_PropertyName)(BSTR* pbstr)  
	{ 
		*pbstr = m_bstrPropertyName.Copy(); 
		return S_OK; 
	}; 
 
	STDMETHOD(put_PropertyName)(BSTR bstr) 
	{ 
		m_bstrPropertyName = bstr; 
		return S_OK; 
	}; 
