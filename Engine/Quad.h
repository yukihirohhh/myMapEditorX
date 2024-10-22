#pragma once

#include <DirectXMath.h>
#include "Direct3D.h"
#include "Texture.h"
#include <vector>
#include "Transform.h"

using namespace DirectX;

//�R���X�^���g�o�b�t�@�[
struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
	XMMATRIX	matNormal; //matW����Normal�p�ɉ���
};

struct VERTEX
{
	XMVECTOR postion;
	XMVECTOR uv;
	XMVECTOR normal;

};

class Quad
{
protected:
	int vertexNum_;
	std::vector<VERTEX> vertices_;
	int indexNum_;//�C���f�b�N�X��
	std::vector<int> index_;//�C���f�b�N�X���

	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;	//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
	Texture* pTexture_;
public:
	Quad();
	~Quad();
	HRESULT Initialize();
	void Draw(Transform& transform );
	void Release();

private:
	virtual void InitVertexData();
	HRESULT CreateVertexBuffer();
	virtual void InitIndexData();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();
	HRESULT LoadTexture();

	//---------Draw�֐�����Ă΂��֐�---------
	void PassDataToCB(Transform transform);	//�R���X�^���g�o�b�t�@�Ɋe�����n��
	void SetBufferToPipeline();
};