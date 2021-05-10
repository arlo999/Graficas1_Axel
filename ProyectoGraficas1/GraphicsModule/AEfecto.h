#pragma once
#include <vector>
#include "ATecnicta.h"
class AEfecto
{
private:
	AEfecto();
	void setListTecnicas(){
		
	};
	std::vector<ATecnicta> getListTecnicas(){
	return m_listaTecnicas;
	};
public:
	std::vector<ATecnicta>m_listaTecnicas;
};

