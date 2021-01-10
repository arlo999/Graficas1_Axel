#include<iostream>
using namespace std;

class Cadena
{

public:
	Cadena(int x, int y);
	~Cadena();
	int getP() { return p; }
	int getX() { return x; }
	int getY() { return y; }
	void setP(int P) { p = sizeof(char) * P; }
	void CambiarEspacio(char,int,int );
	void rellenar();
	void setX(int X) { x = X; }
	void setY(int Y) { y = Y; }
	void printArray();
private:

	int x, y, p;
	int result;
	char** cadena;


};

Cadena::Cadena(int x, int y)
{
	setX(x);
	setY(y);
	setP(x);
	rellenar();
}

Cadena::~Cadena()
{
}

void Cadena::CambiarEspacio(char c,int x, int y)
{
	setX(x);
	setY(y);
	setP(x);
	result = (getP() * getY() + (sizeof(char) * getX()));

	cadena[getP()][result] = c;
	
}

void Cadena::rellenar()
{
	cadena = new char* [x];
	for (unsigned int i = 0; i != x; ++i)
	{
		cadena[i] = new char[x];
		for (unsigned int  j = 0; j < y; ++j)
		{
			cadena[i][j] = '0' + j;
			cout << cadena[i][j] << endl;
		}
		
		
	}

}

void Cadena::printArray()
{
	for (unsigned int i = 0; i != x; ++i)
	{
	
		for (unsigned int j = 0; j < y; ++j)
		{
			
			cout << cadena[i][j] << endl;
		}


	}
}

int main() {
	


	Cadena cadenaran(3, 3);

	cadenaran.CambiarEspacio('a', 1, 1);
	cadenaran.printArray();
	system("Pause");

}
