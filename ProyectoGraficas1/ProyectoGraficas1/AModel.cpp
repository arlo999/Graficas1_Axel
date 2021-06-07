#include "AModel.h"

unsigned int AModel::TextureFromFile(const char* path, const string& directory, bool gamma)
{
	string filename = string(path);
	string nameOfFile = string(path);
	filename = directory + "\\" + filename;
#if defined(DX11)

	/// esto lo saque de internet para obtener los raw datas

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP* dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename.c_str(), 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename.c_str());
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
	{
		dib = FreeImage_Load(fif, filename.c_str());
		dib = FreeImage_ConvertTo32Bits(dib);

	}

	//if the image failed to load, return failure
	if (!dib)
		return 0;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		return 0;


	// Set primitive topology

	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	if (triangles == true) {
		testObj.g_pImmediateContext.A_IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	else if (wire == true) {
		testObj.g_pImmediateContext.A_IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	}
	else if (point == true) {
		testObj.g_pImmediateContext.A_IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	}
	else {
		testObj.g_pImmediateContext.A_IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}





	D3D11_TEXTURE2D_DESC tdesc;
	ZeroMemory(&tdesc, sizeof(tdesc));
	tdesc.Width = width;
	tdesc.Height = height;
	tdesc.MipLevels = 0;
	tdesc.ArraySize = 1;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tdesc.SampleDesc.Count = 1;
	if (bgr == true) {

		tdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	}
	else if (rgb == true) {
		tdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	}
	else {
		tdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	}

	tdesc.SampleDesc.Quality = 0;
	tdesc.CPUAccessFlags = 0;
	tdesc.MiscFlags = 0;


	ID3D11Texture2D* texture;
	testObj.g_pd3dDevice.A_CreateTexture2D(&tdesc, NULL, &texture);
	testObj.g_pImmediateContext.A_UpdateSubresource(texture, 0, NULL, bits, FreeImage_GetPitch(dib), 0);

	D3D11_SHADER_RESOURCE_VIEW_DESC Sdesc = {};
	Sdesc.Format = tdesc.Format;
	Sdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	Sdesc.Texture2D.MostDetailedMip = 0;
	Sdesc.Texture2D.MipLevels = 1;
	if (nameOfFile == "diffuse.jpg"|| nameOfFile=="base_albedo.jpg") {
		testObj.g_pd3dDevice.A_CreateShaderResourceView(texture, &Sdesc, &g_pTextureRV);
	}
	else if (nameOfFile == "normal.png"|| nameOfFile == "base_normal.jpg") {

		testObj.g_pd3dDevice.A_CreateShaderResourceView(texture, &Sdesc, &g_NormalMap);
	}
	else if (nameOfFile == "specular.jpg"|| nameOfFile == "base_AO.jpg") {
		testObj.g_pd3dDevice.A_CreateShaderResourceView(texture, &Sdesc, &g_Specular);

	}
	else {

		std::cout << "erro to load terture";
		testObj.g_pd3dDevice.A_CreateShaderResourceView(texture, &Sdesc, &g_pTextureRV);
		testObj.g_pd3dDevice.A_CreateShaderResourceView(texture, &Sdesc, &g_NormalMap);
		testObj.g_pd3dDevice.A_CreateShaderResourceView(texture, &Sdesc, &g_Specular);
	}

	FreeImage_Unload(dib);

#endif

	unsigned int textureID = 0;
#if defined(OGL)
	int width, height, nrComponents;

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, STBI_rgb);


	glGenTextures(1, &textureID);



	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		if (bgr == true) {

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
		}
		else if (rgb == true) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (triangles == true) {

			glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
		}
		else if (wire == true) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (point == true) {
			glPolygonMode(GL_POINTS, GL_FILL);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
		}


		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

#endif
	return textureID;

}

void AModel::Draw(AShader& shader)
{
#if defined(OGL)
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(transform.traslation[0], transform.traslation[1], transform.traslation[2]));
	model = glm::scale(model, glm::vec3(transform.scale[0], transform.scale[1], transform.scale[2]));

	model = glm::rotate(model, glm::radians(transform.rotation[0] * 180 / 3.1415f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(transform.rotation[1] * 180 / 3.1415f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(transform.rotation[2] * 180 / 3.1415f), glm::vec3(0, 0, 1));
	shader.setMat4("model", model);
#endif
#if defined(DX11)
	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	testObj.cb.mWorld = XMMatrixTranslation(transform.traslation[0], transform.traslation[1], transform.traslation[2]);
	testObj.cb.mWorld = XMMatrixMultiply(XMMatrixScaling(transform.scale[0], transform.scale[1], transform.scale[2]), testObj.cb.mWorld);
	testObj.cb.mWorld = XMMatrixMultiplyTranspose(XMMatrixRotationRollPitchYaw(transform.rotation[0], transform.rotation[1], transform.rotation[2]), testObj.cb.mWorld);

	testObj.g_pImmediateContext.A_UpdateSubresource(testObj.m_pCBChangesEveryFrame.getBufferDX11(), 0, NULL, &testObj.cb, 0, 0);
	testObj.g_pImmediateContext.A_PSSetShaderResources(0, 1, &g_pTextureRV);
	testObj.g_pImmediateContext.A_PSSetShaderResources(1, 1, &g_NormalMap);
	testObj.g_pImmediateContext.A_PSSetShaderResources(2, 1, &g_Specular);

#endif	
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader, point);
	}
}

void AModel::Render()
{
	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);
	testObj.cb.mWorld = XMMatrixTranslation(transform.traslation[0], transform.traslation[1], transform.traslation[2]);
	testObj.cb.mWorld = XMMatrixMultiply(XMMatrixScaling(transform.scale[0], transform.scale[1], transform.scale[2]), testObj.cb.mWorld);
	testObj.cb.mWorld = XMMatrixMultiplyTranspose(XMMatrixRotationRollPitchYaw(transform.rotation[0], transform.rotation[1], transform.rotation[2]), testObj.cb.mWorld);

	testObj.g_pImmediateContext.A_UpdateSubresource(testObj.m_pCBChangesEveryFrame.getBufferDX11(), 0, NULL, &testObj.cb, 0, 0);
	testObj.g_pImmediateContext.A_PSSetShaderResources(0, 1, &g_pTextureRV);
	testObj.g_pImmediateContext.A_PSSetShaderResources(1, 1, &g_NormalMap);
	testObj.g_pImmediateContext.A_PSSetShaderResources(2, 1, &g_Specular);

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Render();
	}
}

void AModel::Rendersaq()
{
	auto& testObj = GraphicsModule::GetTestObj(g_hwnd);

	testObj.cb.mWorld = XMMatrixTranslation(transform.traslation[0], transform.traslation[1], transform.traslation[2]);
	testObj.cb.mWorld = XMMatrixMultiply(XMMatrixScaling(transform.scale[0], transform.scale[1], transform.scale[2]), testObj.cb.mWorld);
	testObj.cb.mWorld = XMMatrixMultiplyTranspose(XMMatrixRotationRollPitchYaw(transform.rotation[0], transform.rotation[1], transform.rotation[2]), testObj.cb.mWorld);

	testObj.g_pImmediateContext.A_UpdateSubresource(testObj.m_pCBChangesEveryFrame.getBufferDX11(), 0, NULL, &testObj.cb, 0, 0);
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Render();
	}
}

void AModel::loadModel(string const& path)
{

	transform.scale[0] = 1.0f;
	transform.scale[1] = 1.0f;
	transform.scale[2] = 1.0f;

	transform.traslation[0] = 0.0f;
	transform.traslation[1] = 0.0f;
	transform.traslation[2] = 0.0f;

	transform.rotation[0] = 0.0f;
	transform.rotation[1] = 0.0f;
	transform.rotation[2] = 0.0f;



	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}

	directory = path.substr(0, path.find_last_of(92));


	processNode(scene->mRootNode, scene);

}

void AModel::processNode(aiNode* node, const aiScene* scene)
{

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{

		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh AModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;


	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		AVector vector;

		vector.setX(mesh->mVertices[i].x);
		vector.setY(mesh->mVertices[i].y);
		vector.setZ(mesh->mVertices[i].z);
		vertex.Position[0] = vector.getX();
		vertex.Position[1] = vector.getY();
		vertex.Position[2] = vector.getZ();
		// normals
		if (mesh->HasNormals())
		{
			vector.setX(mesh->mNormals[i].x);
			vector.setY(mesh->mNormals[i].y);
			vector.setZ(mesh->mNormals[i].z);
			vertex.Normal[0] = vector.getX();
			vertex.Normal[1] = vector.getY();
			vertex.Normal[2] = vector.getZ();
		}
		// texture coordinates
		if (mesh->mTextureCoords[0])
		{
			AVector vec;

			vec.setX(mesh->mTextureCoords[0][i].x);
			vec.setY(mesh->mTextureCoords[0][i].y);
			vertex.TexCoords[0] = vec.getX();
			vertex.TexCoords[1] =1- vec.getY();

		}
		if(mesh->HasTangentsAndBitangents()){
			
			AVector binormal;
			binormal.setX(mesh->mBitangents[0].x);
			binormal.setY(mesh->mBitangents[1].y);
			binormal.setZ(mesh->mBitangents[2].z);
			vertex.Binormal[0]=binormal.getX();
			vertex.Binormal[1]=binormal.getY();
			vertex.Binormal[2]=binormal.getZ();
			AVector tanget;
			tanget.setX(mesh->mTangents[0].x);
			tanget.setY(mesh->mTangents[1].y);
			tanget.setZ(mesh->mTangents[2].z);
			vertex.Tangente[0]=tanget.getX();
			vertex.Tangente[0]=tanget.getY();
			vertex.Tangente[0]=tanget.getZ();
		
		}

		else {

			vertex.TexCoords[0] = 0.0f;
			vertex.TexCoords[1] =1- 0.0f;
		}
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];


	// 1. diffuse maps
	vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());


	// 2. specular maps

	vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());



	// 3. normal maps

	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());



	if (diffuseMaps.empty()) {
		Texture texture;
		texture.id = TextureFromFile("base_albedo.jpg", this->directory, false);
		texture.type = aiTextureType_DIFFUSE;
		texture.path = "base_albedo.jpg";
		textures.push_back(texture);
		textures_loaded.push_back(texture);
	}
#if defined(OGL)

#endif

	return Mesh(vertices, indices, textures);

}

vector<Texture> AModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it

			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory, false);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.

		}

	}

	return textures;
	}