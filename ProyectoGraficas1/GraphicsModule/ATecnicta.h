#pragma once
#include <vector>
#include "APase.h"
class ATecnicta
{
public:
	ATecnicta();

	void setListPases(std::vector<APase>_listpase) {
		m_listPase=_listpase;
	};
	std::vector<APase> getListPases(){
		return m_listPase;
	};
private:
	
	std::vector<APase> m_listPase;


};

