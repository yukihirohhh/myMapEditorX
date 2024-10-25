#pragma once
#include "Engine/GameObject.h"
#include "windows.h"

namespace {
    const int MODEL_NUM{ 5 };
    const int XSIZE{ 15 };
    const int ZSIZE{ 15 };
    enum BLOCKTYPE
    {
        DEFAULT, BRICK, GRASS, SAND, WATER
    };
}

class Stage : public GameObject
{
    int hModel_[MODEL_NUM];    //モデル番号
    //int table_[XSIZE][ZSIZE];
    struct BLOCKDATA
    {
        int type;
        int height;
    } table_[15][15];

    int mode_;      //0:上げる　1：下げる　2：種類を変える
    int select_;    //種類


public:
    void SetBlock(int _x, int _z, BLOCKTYPE _type);
    void SetBlockHeight(int _x, int _z, int _height);
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
    void Save();
    void Open();
};