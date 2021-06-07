#pragma once
#include <iostream>
#include <vector>
#include "AModel.h"
#include "ATecnica.h"


class AEfecto
{
public:
	AEfecto();
	~AEfecto();
	void Init();
	void Render(std::vector<AModel*>& _ModelList);

private:
	std::vector<ATecnica*>m_TecnicaList;

};

