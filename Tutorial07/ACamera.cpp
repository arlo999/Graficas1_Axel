#include "ACamera.h"

ACamera::ACamera()
{
}

XMMATRIX ACamera::viewMLookL(AVector x, AVector y, AVector z)
{

    AVector zaxis= (x-y);
    zaxis.normalize();
    AVector xaxis=(up.ProductoCruz(zaxis));
    xaxis.normalize();
    AVector yaxis=(zaxis.ProductoCruz(xaxis));
    

    XMMATRIX m_View(
        XMVectorSet(xaxis.getX() , yaxis.getX(), zaxis.getX(),0),
        XMVectorSet(xaxis.getY(), yaxis.getY(), zaxis.getY(),0),
        XMVectorSet(xaxis.getZ(), yaxis.getZ(), zaxis.getZ(),0),
        XMVectorSet(xaxis.ProductoPunto(x)*(-1),- yaxis.ProductoPunto(x) * (-1),-zaxis.ProductoPunto(x) * (-1),1));
   
    return m_View;
    
    
}

XMMATRIX ACamera::ViewPerspective(float fov, float aspectRatio, float cerca, float lejos)
{
    float height = cos(fov * .5) / sin(fov * .5);
    float width = height / aspectRatio;

    XMMATRIX M_VIEWPerspective(
        XMVectorSet(width, 0, 0, 0),
        XMVectorSet(0, height, 0, 0),
        XMVectorSet(0, 0, lejos/(lejos-cerca), 1),
        XMVectorSet(0, 0, (-lejos / (lejos - cerca))*cerca, 0)
    );

    return M_VIEWPerspective;
}

XMMATRIX ACamera::ViewOrtographic(float viewWidth, float viewHeigth, float cerca, float lejos)
{
    XMMATRIX ortographic(
    
        XMVectorSet((2.0/ viewWidth), 0, 0, 0),
        XMVectorSet(0, (2/viewHeigth), 0, 0),
        XMVectorSet(0, 0, 1 / (lejos - cerca), 0),
        XMVectorSet(0, 0, (-1 / (lejos - cerca)) * cerca, 1)
    
    );
    return ortographic;
}
