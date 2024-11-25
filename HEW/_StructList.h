#pragma once
/* 構造体リスト */

// === Float_Struct
struct FLOAT2
{
	float X;
	float Y;
};

struct FLOAT3
{
	float X;
	float Y;
	float Z;
};

// === UDP_Conecting_System_Struct

#define IPV4_LEN (20)

enum CSEC
{
	CSEC_OK,
	CSEC_ERROR,
};

enum SPR
{
	SAVER_MODE_1,
	SAVER_MODE_2,
};

enum STM
{
	TIME_OUT_NOPROSESS,
	TIME_OUT_CALLING,
	TIME_OUT_RESPONSE,
};

/* ユーザーの個人データ */
struct Personal_Information_Data
{
	int nConnctinKey;				/* ユーザー認証キー */
	char IPv4[IPV4_LEN];			/* IPv4アドレス */
};

/* システムデータ */
struct Connecting_System_Data
{
	bool bUse = false;				/* 使用判定 */
	SPR eProsessMode;				/* 処理モードの選択 */
	STM eTimeOutProsess;			/* 接続確認処理 */
	int nTimeOutTImer;				/* 接続確認用タイマー */
	int nCalling_Number;			/* 呼出番号 */


};

struct Communication_Data
{
	Personal_Information_Data PersonalData;
	Connecting_System_Data ConectionData;
};