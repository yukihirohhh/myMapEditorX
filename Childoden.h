#pragma once
#include "Engine/GameObject.h"

class Fbx;

class Childoden :
    public GameObject
{
    Fbx* pFbx;
    int hModel_;//���f���ԍ�
public:
    //�R���X�g���N�^
    Childoden(GameObject* parent);

    //�f�X�g���N�^
    ~Childoden();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};

