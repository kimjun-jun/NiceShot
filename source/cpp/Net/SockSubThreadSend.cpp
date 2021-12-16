/**
* @file SockUpdate.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "../../h/main.h"
#include "../../h/Other/sound.h"
#include "../../h/Debug/debugproc.h"
#include "../../h/Other/input.h"
#include "../../h/Object/ObjectClass/Interface/interface.h"
#include "../../h/Object/Player/player.h"
#include "../../h/Object/Item/item.h"
#include "../../h/Object/Bullet/bullet.h"
#include "../../h/Object/Scene/Scene.h"
#include "../../h/Map/field.h"
#include "../../h/Net/sock.h"
#include "../../h/Net/SockSubThreadSend.h"

#pragma comment (lib, "Ws2_32.lib")

//���MMANAGER
void SOCK_SUBTHREAD_SEND::SendFunc(MySOCKET *MySockObj)
{
	this->SendPacket(MySockObj);
}

void SOCK_SUBTHREAD_SEND::SendPacket(MySOCKET *MySockObj)
{
	int MyNum = MySockObj->GetNetMyNumber();
	char PlayerSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
	char ItemSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
	char EndGameMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e

	//----------------�ύX�������m�F�@�ύX�ӏ��������send()����@���ݍ��W�͏펞�������Ă���
	//���W
	D3DXVECTOR3 Pos = MySockObj->pplayer->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 OldPos = MySockObj->pplayer->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].OldPos();
	//if (Pos != OldPos)
	{
		//g_SendMsgBuff.playerPos = Pos;
		//player[MyNum].NetChkPos = true;
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@P%d,Pos,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, Pos.x, Pos.y, Pos.z);
		sprintf_s(PlayerSMsg, "%s%s", PlayerSMsg, NewSMsg);
	}
	//��]
	D3DXVECTOR3 HoudaiRot = MySockObj->pplayer->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoudaiOldRot = MySockObj->pplayer->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].OldRot();
	//if (HoudaiRot != HoudaiOldRot)
	{
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@P%d,HoudaiRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HoudaiRot.x, HoudaiRot.y, HoudaiRot.z);
		sprintf_s(PlayerSMsg, "%s%s", PlayerSMsg, NewSMsg);
	}
	D3DXVECTOR3 HoutouRot = MySockObj->pplayer->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HoutouOldRot = MySockObj->pplayer->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUTOU].OldRot();
	//if (HoutouRot != HoutouOldRot)
	{
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@P%d,HoutouRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HoutouRot.x, HoutouRot.y, HoutouRot.z);
		sprintf_s(PlayerSMsg, "%s%s", PlayerSMsg, NewSMsg);
	}
	D3DXVECTOR3 HousinRot = MySockObj->pplayer->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();
	D3DXVECTOR3 HousinOldRot = MySockObj->pplayer->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUSIN].OldRot();
	//if (HousinRot != HousinOldRot)
	{
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@P%d,HousinRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HousinRot.x, HousinRot.y, HousinRot.z);
		sprintf_s(PlayerSMsg, "%s%s", PlayerSMsg, NewSMsg);
	}

	//----------------�o���b�g
	//�o���b�g�𔭎˂��Ă��Ȃ�0,�ʏ탂�f���̔���1,�A�^�b�N���f���̔���3
	switch (MySockObj->pplayer->PlayerPara[MyNum].BulletPara.GetNetBulletShotFlagOneFrame())
	{
	case 0:
		break;
	case 1:
	{
		D3DXVECTOR3 BPos = MySockObj->pplayer->PlayerPara[MyNum].BulletPara.GetBulletStartPos();
		D3DXVECTOR3 BMove = *MySockObj->pplayer->PlayerPara[MyNum].BulletPara.GetBulletMove();
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@P%d,BulletA,PX%4.3f,PY%4.3f,PZ%4.3f,MX%4.3f,MY%4.3f,MZ%4.3f&"
			, MyNum, BPos.x, BPos.y, BPos.z, BMove.x, BMove.y, BMove.z);
		sprintf_s(PlayerSMsg, "%s%s", PlayerSMsg, NewSMsg);
		break;
	}
	case 3:
	{
		D3DXVECTOR3 BPos = MySockObj->pplayer->PlayerPara[MyNum].BulletPara.GetBulletStartPos();
		D3DXVECTOR3 BMove[3];
		BMove[0] = *MySockObj->pplayer->PlayerPara[MyNum].BulletPara.GetBulletMove();
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg,
			"@P%d,BulletB,PX%4.3f,PY%4.3f,PZ%4.3f,MX%4.3f,MY%4.3f,MZ%4.3f,MX%4.3f,MY%4.3f,MZ%4.3f,MX%4.3f,MY%4.3f,MZ%4.3f&"
			, MyNum, BPos.x, BPos.y, BPos.z, BMove[0].x, BMove[0].y, BMove[0].z
			, BMove[1].x, BMove[1].y, BMove[1].z, BMove[2].x, BMove[2].y, BMove[2].z);
		sprintf_s(PlayerSMsg, "%s%s", PlayerSMsg, NewSMsg);
		break;
	}
	}

	//----------------�o�C�^��
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//if (MySockObj->pplayer->PlayerPara[CntPlayer].StandardPara.Vital != MySockObj->pplayer->PlayerPara[CntPlayer].StandardPara.OldVital)
		{
			int Vital = MySockObj->pplayer->PlayerPara[CntPlayer].StandardPara.Vital;
			//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
			char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
			sprintf_s(NewSMsg, "@P%d,Vital,%d&", CntPlayer, Vital);
			sprintf_s(PlayerSMsg, "%s%s", PlayerSMsg, NewSMsg);
		}
	}
	//----------------���[�t�B���O ���[�t�B���O�M��ON(�A�^�b�N���f���̎��A�펞ON)���b�Z�[�W���M�Breceive����1���󂯑�����B
	if (MySockObj->pplayer->PlayerPara[MyNum].MorphingPara.GetNetMorphingOneFrame() == true)//NetGetMorphing
	{
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@P%d,Morphing,%d&", MyNum, 1);
		sprintf_s(PlayerSMsg, "%s%s", PlayerSMsg, NewSMsg);
	}
	//--------------------------------�ύX���������ꍇsend()����@��{���
	if (PlayerSMsg[0] != NULL)
	{
		send(MySockObj->DstSocketFunc(), PlayerSMsg, BUFFER_SIZE_STRING, 0);
	}


	//----------------�A�C�e���擾�M��
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		if (MySockObj->pitem->ItemParaAll[CntItem].NetGetItemFlag == true &&
			MySockObj->pitem->ItemParaAll[CntItem].GetPlayerType == MyNum)
		{
			int ItemT = MySockObj->pitem->ItemParaAll[CntItem].eType;
			//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
			char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
			sprintf_s(NewSMsg, "@I,N%d,T%d&"
				, CntItem, ItemT);
			sprintf_s(ItemSMsg, "%s%s", ItemSMsg, NewSMsg);
		}
	}
	//--------------------------------�ύX���������ꍇsend()����@�A�C�e���擾���
	if (ItemSMsg[0] != NULL)
	{
		send(MySockObj->DstSocketFunc(), ItemSMsg, BUFFER_SIZE_STRING, 0);

		//�n�`�A�C�e�����擾���ꂽ�������ǉ��̃��b�Z�[�W�𑗐M����
		if (MySockObj->pfield->FieldPara.TikeiSeed != MySockObj->pfield->FieldPara.OldTikeiSeed)
		{
			char SpecialSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
			sprintf_s(SpecialSMsg, "@T,S%d,N%d&", MySockObj->pfield->FieldPara.TikeiSeed, MySockObj->pfield->FieldPara.ItemGetPlayerNum);
			send(MySockObj->DstSocketFunc(), SpecialSMsg, BUFFER_SIZE_STRING, 0);
		}
	}


	//----------------�Q�[���V�[���I�����b�Z�[�W
	int Puse = 0;
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (MySockObj->pplayer->iUseType[CntPlayer].Use() == NoUse) Puse++;
	}
	if (Puse >= 3)
	{
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@EndGame&");
		sprintf_s(EndGameMsg, "%s%s", EndGameMsg, NewSMsg);
	}
	//--------------------------------�ύX���������ꍇsend()����@�l�b�g�ΐ�I�����
	if (EndGameMsg[0] != NULL)
	{
		send(MySockObj->DstSocketFunc(), EndGameMsg, BUFFER_SIZE_STRING, 0);
	}
}
