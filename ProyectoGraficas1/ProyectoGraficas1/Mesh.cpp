#include "Mesh.h"
#include <iostream>


Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::setMesh( AsimpleVertex* vertex, int numVertex)
{
    
    buffer = new AsimpleVertex[numVertex];
   
    memcpy(buffer, vertex, numVertex*sizeof(AsimpleVertex));
    
}

AsimpleVertex* Mesh::getMesh()
{


    return buffer ;
}

float* Mesh::getMatrixTransFormacion()
{
    return new float [16] {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

}

void Mesh::setMesh2(AsimpleVertexV2* vertex, int numVertex){

	m_buffer = new AsimpleVertexV2[numVertex];

	memcpy(m_buffer, vertex, numVertex * sizeof(AsimpleVertexV2));
}

AsimpleVertexV2* Mesh::getMesh2()
{
    return m_buffer;
}

void Mesh::setIndice(unsigned short* indice, int numIndices)
{

	m_Indice = new unsigned short[numIndices];

	memcpy(m_Indice, indice, numIndices * sizeof(unsigned short));

}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	// now that we have all the required data, set the vertex buffers and its attribute pointers.
	setupMesh();
}

void Mesh::Draw(AShader& shader, bool triangles)
{
	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
#if defined(OGL)
		glActiveTexture(GL_TEXTURE0 + i); 
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); 
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); 
		else if (name == "texture_height")
			number = std::to_string(heightNr++); 

		glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
		
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		#endif
	
	
	}


#if defined(OGL)
	// draw mesh
	glBindVertexArray(VAO);
	if(triangles){
		glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else if (triangles == false) {
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else{
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
#endif
#if defined(DX11)
	
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	

	
	//testObj.cb.mWorld = XMMatrixMultiplyTranspose(testObj.g_View, testObj.cb.mWorld);
	
	testObj.g_pImmediateContext.A_IASetVertexBuffers(0, 1, &m_pVertexBuffer.getBufferDX11(), &stride, &offset);
	testObj.g_pImmediateContext.A_IASetIndexBuffer(m_pIndexBuffer.getBufferDX11(), DXGI_FORMAT_R32_UINT, 0);

	testObj.g_pImmediateContext.A_DrawIndexed(indices.size(), 0, 0);
#endif

}

void Mesh::setupMesh()
{
#if defined(OGL)
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	

	glBindVertexArray(0);
#endif
#if defined(DX11)
	
	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem =vertices.data();
	testObj.g_pd3dDevice.A_CreateBuffer(&bd, &InitData, &m_pVertexBuffer.getBufferDX11());
	
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices.data();
	testObj.g_pd3dDevice.A_CreateBuffer(&bd, &InitData, &m_pIndexBuffer.getBufferDX11());
	
	
#endif




}
