#pragma once
#include "Engine/GameObject.h"




//���������Ǘ�����N���X
class Controller : public GameObject
{

public:
    //�R���X�g���N�^
    Controller(GameObject* parent);

    //�f�X�g���N�^
    ~Controller();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};