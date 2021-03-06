#pragma once
void SetMultThreadFlag(bool flag);
bool GetMultThreadFlag(void);
void SetGameSceneFlag(bool flag);
bool GetGameSceneFlag(void);
int NetClientSocketCreate(void);
int NetClientSocketDelete(void);
void NetMatch(void);
void NetMyNumberGet(void);
void NetItemGet(void);
void NetCountdown(void);
void Packet(void);
void SendPacket(void);
void ReceivePacket(void);

enum
{
	SetEnumPos = 0,
	SetEnumHoudaiRot = 1,
	SetEnumHoutouRot = 2,
	SetEnumHousinRot = 3,

};

void MsgAnalys(char* argRMsg);

//RMsgBlockはデータのブロック　TypeはPosやRotなどの種類
void SetBuff(char* RMsgBlock,int Type, int PlayerNum);

void NetSetItem(D3DXVECTOR3 buff, int index, int type);
void NetSetTikeiSeed(int Seed, int PlayerNum);
void NetSetPos(D3DXVECTOR3 buff, int PlayerNum);
void NetSetHoudaiRot(D3DXVECTOR3 buff, int PlayerNum);
void NetSetHoutouRot(D3DXVECTOR3 buff, int PlayerNum);
void NetSetHousinRot(D3DXVECTOR3 buff, int PlayerNum);
void NetSetVital(int buff, int PlayerNum);
void NetSetMorphing(int PlayerNum);

void NetSetBulletType1(D3DXVECTOR3 buffpos, D3DXVECTOR3 buffmove, int PlayerNum);
void NetSetBulletType3(D3DXVECTOR3 buffpos, D3DXVECTOR3 *buffmove, int PlayerNum);





