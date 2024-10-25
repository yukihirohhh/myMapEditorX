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

//デストラクタ
Stage::~Stage()
{
}

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
	for (int i = 0; i < MODEL_NUM; i++) {
		hModel_[i] = Model::Load(fname_base + modelname[i]);
		assert(hModel_[i] >= 0);
	}
	for (int z = 0; z < ZSIZE; z++) {
		for (int x = 0; x < XSIZE; x++) {
			SetBlock(x, z, (BLOCKTYPE)(0));
			SetBlockHeight(x, z, 0);
		}
	}

}

void Stage::Update()
{
	if (!Input::IsMouseButtonDown(0)) {
		return;
	}
	float w = (float)(Direct3D::scrWidth / 2.0f);
	float h = (float)(Direct3D::scrHeight / 2.0f);

	XMMATRIX vp =
	{
		 w,  0,  0, 0,
		 0, -h,  0, 0,
		 0,  0,  1, 0,
		 w,  h,  0, 1
	};

	//ビューポート
	XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
	//プロジェクション変換
	XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
	//ビュー変換
	XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
	XMFLOAT3 mousePosFront = Input::GetMousePosition();
	mousePosFront.z = 0.0f;
	XMFLOAT3 mousePosBack = Input::GetMousePosition();
	mousePosBack.z = 1.0f;
	//①　mousePosFrontをベクトルに変換
	XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront);
	//②　①にinvVP、invPrj、invViewをかける
	vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);
	//③　mousePosBackをベクトルに変換
	XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
	//④　③にinvVP、invPrj、invViewをかける
	vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);

	int bufX = -1, bufZ;
	float minDistance = 9999999;

	for (int x = 0; x < 15; x++)
	{
		for (int z = 0; z < 15; z++)
		{
			for (int y = 0; y < table_[x][z].height + 1; y++)
			{
				//⑤　②から④に向かってレイをうつ（とりあえずモデル番号はhModel_[0]）
				RayCastData data;
				XMStoreFloat4(&data.start, vMouseFront);
				XMStoreFloat4(&data.dir, vMouseBack - vMouseFront);
				Transform trans;
				trans.position_.x = x;
				trans.position_.y = y;
				trans.position_.z = z;
				Model::SetTransform(hModel_[0], trans);

				Model::RayCast(hModel_[0], data);

				//⑥　レイが当たったらブレークポイントで止める
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
				//table[x][z]からオブジェクトのタイプを取り出して書く！
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

void Stage::Release()
{

}


BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"デフォルト");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"レンガ");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"草原");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"砂地");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"水");
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
	char fileName[MAX_PATH] = "マップデータ.map";  // ファイル名を入れる変数

	// 「ファイルを保存」ダイアログの設定
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")
		TEXT("すべてのファイル(*.*)\0*.*\0\0");
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = "map";

	BOOL selFile = GetSaveFileName(&ofn);

	if (selFile == FALSE) return;

	// ファイルを開く（書き込み専用、常に新規作成または上書き）
	HANDLE hFile = CreateFile(
		fileName,                // ファイル名
		GENERIC_WRITE,           // 書き込みアクセスモード
		0,                       // 共有モードなし
		NULL,                    // セキュリティ属性
		CREATE_ALWAYS,           // 常に新しいファイルを作成
		FILE_ATTRIBUTE_NORMAL,   // 通常ファイル属性
		NULL
	);

	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, "失敗", "エラー", MB_OK | MB_ICONERROR);
		return;
	}

	DWORD bytesWritten = 0;

	// ブロックデータを書き込む
	for (int z = 0; z < ZSIZE; ++z) {
		for (int x = 0; x < XSIZE; ++x) {
			BLOCKDATA blockData = table_[x][z];  // 各ブロックのデータをコピー
			WriteFile(
				hFile,                         // ファイルハンドル
				&blockData,                    // 書き込むデータ
				sizeof(BLOCKDATA),             // ブロックデータのサイズ
				&bytesWritten,                 // 書き込んだバイト数
				NULL                           // オーバーラップ構造体（使用しない）
			);
		}
	}

	CloseHandle(hFile);
}


void Stage::Open()
{
	char fileName[MAX_PATH] = "マップデータ.map";

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")
		TEXT("すべてのファイル(*.*)\0*.*\0\0");
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = "map";

	BOOL selFile = GetOpenFileName(&ofn);

	// ファイルを開く (読み込み専用)
	HANDLE hFile = CreateFile(
		fileName,              // ファイル名
		GENERIC_READ,          // 読み込みアクセスモード
		0,                     // 共有モードなし
		NULL,                  // セキュリティ属性
		OPEN_EXISTING,         // 既存ファイルを開く
		FILE_ATTRIBUTE_NORMAL, // 通常ファイル属性
		NULL
	);

	DWORD fileSize = GetFileSize(hFile, NULL);

	DWORD bytesRead = 0;
	for (int z = 0; z < ZSIZE; ++z) {
		for (int x = 0; x < XSIZE; ++x) {
			BLOCKDATA blockData;
			ReadFile(
				hFile,                  // ファイルハンドル
				&blockData,             // 読み込むデータのバッファ
				sizeof(BLOCKDATA),      // 読み込むデータのサイズ
				&bytesRead,             // 実際に読み込んだバイト数
				NULL                    // オーバーラップ構造体（使用しない）
			);

			table_[x][z] = blockData;  // 読み込んだデータをテーブルに適用
		}
	}

	// ファイルを閉じる
	CloseHandle(hFile);

}

