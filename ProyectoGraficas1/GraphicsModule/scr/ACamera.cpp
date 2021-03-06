#include "ACamera.h"

ACamera::ACamera()
{

}
/**
     * @brief   esta 
     * @param   #float: numero a summar.
     * @param   #float: numero a summar.
     * @bug     si metes negativos vale gorro :C.
     * @return  #float: el numero sumado
     */
void ACamera::setviewMLookL(AVector x, AVector y, AVector z)
{
    //frot
     zaxis= (y-x);
    zaxis.normalize();
    //right
    xaxis=(up.ProductoCruz(zaxis));
    xaxis.normalize();
   //up
    yaxis =(zaxis.ProductoCruz(xaxis));
    
#if defined(DX11)
    XMMATRIX m_View(
        XMVectorSet(xaxis.getX() , yaxis.getX(), zaxis.getX(),0),
        XMVectorSet(xaxis.getY(), yaxis.getY(), zaxis.getY(),0),
        XMVectorSet(xaxis.getZ(), yaxis.getZ(), zaxis.getZ(),0),
        XMVectorSet(-xaxis.ProductoPunto(x),- yaxis.ProductoPunto(x),-zaxis.ProductoPunto(x) ,1));
    viewMatrix = m_View;
   #endif
    
    
}
/**
     * @brief   Esta funcion sumara 2 numeros, e informara del resultado en el return de la funcion.
     *          Es compatible solo con numeros flotantes de 32 bits.
     * @param   #float: numero a summar.
     * @param   #float: numero a summar.
     * @bug     si metes negativos vale gorro :C.
     * @return  #float: el numero sumado
     */
float* ACamera::ViewPerspective(float fov, float aspectRatio, float cerca, float lejos)
{

    float height = cos(fov * .5) / sin(fov * .5);
    float width = height / aspectRatio;
	return  new float[16]{
	  width, 0.0f, 0.0f, 0.0f,
	  0.0f, height, 0.0f, 0.0f,
	  0.0f, 0.0f, lejos / (lejos - cerca), 1.0f,
	  0.0f, 0.0f, (-lejos / (lejos - cerca)) * cerca, 0.0f
	};
}

/**
     * @brief   Esta funcion sumara 2 numeros, e informara del resultado en el return de la funcion.
     *          Es compatible solo con numeros flotantes de 32 bits.
     * @param   #float: numero a summar.
     * @param   #float: numero a summar.
     * @bug     si metes negativos vale gorro :C.
     * @return  #float: el numero sumado
     */
float* ACamera::ViewOrtographic(float viewWidth, float viewHeigth, float cerca, float lejos)
{
    return  new float[16]{
    (2.0f / viewWidth), 0.0f, 0.0f, 0.0f,
        0.0f, (2.0f / viewHeigth), 0.0f, 0.0f,
        0.0f, 0.0f, (1.0f / (lejos - cerca)), 0.0f,
        0.0f, 0.0f, (-(1.0f / (lejos - cerca)))* cerca, 1.0f
    };
}



#if defined(DX11)
/**
     * @brief   Esta funcion sumara 2 numeros, e informara del resultado en el return de la funcion.
     *          Es compatible solo con numeros flotantes de 32 bits.
     * @param   #float: numero a summar.
     * @param   #float: numero a summar.
     * @bug     si metes negativos vale gorro :C.
     * @return  #float: el numero sumado
     */
XMMATRIX ACamera::ViewPerspectiveDirectX(float fov, float aspectRatio, float cerca, float lejos)
{
    float height = cos(fov * .5) / sin(fov * .5);
    float width = height / aspectRatio;

    XMMATRIX M_VIEWPerspective(
        XMVectorSet(width, 0, 0, 0),
        XMVectorSet(0, height, 0, 0),
        XMVectorSet(0, 0, lejos / (lejos - cerca), 1),
        XMVectorSet(0, 0, (-lejos / (lejos - cerca)) * cerca, 0)
    );

    return M_VIEWPerspective;
}
XMMATRIX ACamera::ViewOrtographicDirectX(float viewWidth, float viewHeigth, float cerca, float lejos)
{
   
    XMMATRIX ortographic(

        XMVectorSet((2.0 / viewWidth), 0, 0, 0),
        XMVectorSet(0, (2 / viewHeigth), 0, 0),
        XMVectorSet(0, 0, 1 / (lejos - cerca), 0),
        XMVectorSet(0, 0, (-1 / (lejos - cerca)) * cerca, 1)

    );
    return ortographic;
}
#endif




void ACamera::move(float x, float y, float z)
{
    //up
    AVector UPaxis = zaxis;
    //at
    AVector Ataxis = yaxis;
    //eye
    AVector Eyeaxis = xaxis;
    UPaxis *= z;
    Ataxis *= y;
    Eyeaxis *= x;
    
    AVector resul(0, 0, 0);
    resul += UPaxis;
    resul += Ataxis;
    resul += Eyeaxis;
    
   
    eye= getEye() + resul;
    at=getAt() + resul;
   
    
    setviewMLookL(eye,at,up);
   
}
/**
     * @brief   Esta funcion sumara 2 numeros, e informara del resultado en el return de la funcion.
     *          Es compatible solo con numeros flotantes de 32 bits.
     * @param   #float: numero a summar.
     * @param   #float: numero a summar.
     * @bug     si metes negativos vale gorro :C.
     * @return  #float: el numero sumado
     */
void ACamera::rotate(float x, float y, float z)
{
    if (frame == true) {
        x = x - m_fx;
        y = y - m_fy;
        x /=-100;
        y /= 100;

	    AVector Ataxis(x, y, z);


		at = Ataxis + at;
        setviewMLookL(eye, at, up);
        frame = false;
    }
    else  {
    
		m_fx = x;
		m_fy = y;
        frame = true;
    
    }
   
  
    

}
