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
	void Render();
	void InitForward();
	void InitDefered();
private:
	std::vector<ATecnica*>m_TecnicaList;

};

