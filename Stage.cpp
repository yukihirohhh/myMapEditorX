#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Engine/Fbx.h"
#include "resource.h"


void Stage::SetBlock(int _x, int _z, BLOCKTYPE _type)
{
	table_[_x][_z].type = _type;
}

void Stage::SetBlockHeight(int _x, int _z, int _height)
{
	table_[_x][_z].height = _height;
}

//�R���X�g���N�^
Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), mode_(0), select_(0)
{
	for (int i = 0; i < MODEL_NUM; i++) {
		hModel_[i] = -1;
	}
	for (int z = 0; z < ZSIZE; z++) {
		for (int x = 0; x < XSIZE; x++) {
			SetBlock(x, z, DEFAULT);
		}
	}
}

//�f�X�g���N�^
Stage::~Stage()
{
}

//������
void Stage::Initialize()
{
	string modelname[] = {
		"BoxDefault.fbx",
		"BoxBrick.fbx",
		"BoxGrass.fbx",
		"BoxSand.fbx",
		"BoxWater.fbx"
	};
	string fname_base = "assets/";
	//���f���f�[�^�̃��[�h
	for (int i = 0; i < MODEL_NUM; i++) {
		hModel_[i] = Model::Load(fname_base + modelname[i]);
		assert(hModel_[i] >= 0);
	}
	//table�Ƀu���b�N�̃^�C�v���Z�b�g���Ă�낤�I
	for (int z = 0; z < ZSIZE; z++) {
		for (int x = 0; x < XSIZE; x++) {
			SetBlock(x, z, (BLOCKTYPE)(0));
			SetBlockHeight(x, z, 0);
		}
	}

}

//�X�V
void Stage::Update()
{
	if (!Input::IsMouseButtonDown(0)) {
		return;
	}
	float w = (float)(Direct3D::scrWidth / 2.0f);
	float h = (float)(Direct3D::scrHeight / 2.0f);
	//Offsetx,y ��0
	//minZ =0 maxZ = 1

	XMMATRIX vp =
	{
		 w,  0,  0, 0,
		 0, -h,  0, 0,
		 0,  0,  1, 0,
		 w,  h,  0, 1
	};

	//�r���[�|�[�g
	XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
	//�v���W�F�N�V�����ϊ�
	XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
	//�r���[�ϊ�
	XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
	XMFLOAT3 mousePosFront = Input::GetMousePosition();
	mousePosFront.z = 0.0f;
	XMFLOAT3 mousePosBack = Input::GetMousePosition();
	mousePosBack.z = 1.0f;
	//�@�@mousePosFront���x�N�g���ɕϊ�
	XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront);
	//�A�@�@��invVP�AinvPrj�AinvView��������
	vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);
	//�B�@mousePosBack���x�N�g���ɕϊ�
	XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
	//�C�@�B��invVP�AinvPrj�AinvView��������
	vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);

	int bufX = -1, bufZ;
	float minDistance = 9999999;

	for (int x = 0; x < 15; x++)
	{
		for (int z = 0; z < 15; z++)
		{
			for (int y = 0; y < table_[x][z].height + 1; y++)
			{
				//�D�@�A����C�Ɍ������ă��C�����i�Ƃ肠�������f���ԍ���hModel_[0]�j
				RayCastData data;
				XMStoreFloat4(&data.start, vMouseFront);
				XMStoreFloat4(&data.dir, vMouseBack - vMouseFront);
				Transform trans;
				trans.position_.x = x;
				trans.position_.y = y;
				trans.position_.z = z;
				Model::SetTransform(hModel_[0], trans);

				Model::RayCast(hModel_[0], data);

				//�E�@���C������������u���[�N�|�C���g�Ŏ~�߂�
				if (data.hit)
				{
					if (minDistance > data.dist)
					{
						minDistance = data.dist;
						bufX = x;
						bufZ = z;
					}
				}

			}
		}
	}
	if (bufX >= 0)
	{
		switch (mode_)
		{
		case 0:
			table_[bufX][bufZ].height++;
			break;
		case 1:
			if (table_[bufX][bufZ].height > 0)
			{
				table_[bufX][bufZ].height--;
			}
			break;
		case 2:
			table_[bufX][bufZ].type = select_;
			break;
		}
	}
}

//�`��
void Stage::Draw()
{
	//Model::SetTransform(hModel_, transform_);
	//Model::Draw(hModel_);

	for (int x = 0; x < 15; x++)
	{
		for (int z = 0; z < 15; z++)
		{
			for (int y = 0; y < table_[x][z].height + 1; y++)
			{
				//table[x][z]����I�u�W�F�N�g�̃^�C�v�����o���ď����I
				int type = table_[x][z].type;
				Transform trans;
				trans.position_.x = x;
				trans.position_.y = y;
				trans.position_.z = z;
				Model::SetTransform(hModel_[type], trans);
				Model::Draw(hModel_[type]);
			}
		}
	}
}

//�J��
void Stage::Release()
{
}

BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"�f�t�H���g");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"�����K");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"����");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"���n");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"��");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_SETCURSEL, 0, 0);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wp))
		{
		case IDC_RADIO_UP:
			mode_ = 0;
			return TRUE;

		case IDC_RADIO_DOWN:
			mode_ = 1;
			return TRUE;

		case IDC_RADIO_CHANGE:
			mode_ = 2;
			return TRUE;

		case IDC_COMBO2:
			select_ = (int)SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0);
			return TRUE;
		}
		return FALSE;

	}
	return FALSE;
}

void Stage::Save()
{
	char fileName[MAX_PATH] = "����.map";  //�t�@�C����������ϐ�

	//�u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�
	OPENFILENAME ofn;                         	//���O�����ĕۑ��_�C�A���O�̐ݒ�p�\����
	ZeroMemory(&ofn, sizeof(ofn));            	//�\���̏�����
	ofn.lStructSize = sizeof(OPENFILENAME);   	//�\���̂̃T�C�Y
	ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.map)\0*.map\0")        //�����t�@�C���̎��
		TEXT("���ׂẴt�@�C��(*.*)\0*.*\0\0");     //����
	ofn.lpstrFile = fileName;               	//�t�@�C����
	ofn.nMaxFile = MAX_PATH;               	//�p�X�̍ő啶����
	ofn.Flags = OFN_OVERWRITEPROMPT;   		//�t���O�i�����t�@�C�������݂�����㏑���m�F�j
	ofn.lpstrDefExt = "map";                  	//�f�t�H���g�g���q

	//�u�t�@�C����ۑ��v�_�C�A���O
	BOOL selFile;
	selFile = GetSaveFileName(&ofn);

	//�L�����Z�������璆�f
	if (selFile == FALSE) return;



	HANDLE hFile;
	hFile = CreateFile(
		fileName,    //�t�@�C����
		GENERIC_WRITE,  //�A�N�Z�X���[�h
		0,
		NULL,
		CREATE_ALWAYS,     //�쐬���@
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	std::string data = "";



	//data.length()


	DWORD bytes = 0;
	WriteFile(
		hFile,              //�t�@�C���n���h��
		"ABCDEF",          //�ۑ�������������
		12,                  //�ۑ����镶����
		&bytes,             //�ۑ������T�C�Y
		NULL
	);



	CloseHandle(hFile);


}

