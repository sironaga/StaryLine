#pragma once
#include "Scene.h"
#include "Texture.h"
#include <array>
#include "Field.h"
#include "FieldVertex.h"
#include "Battle.h"
#include "BackGround.h"
#include "StarLine.h"
#include "Player.h"

enum class TutorialSection
{
	Section1,
	Section2,
	Section3,
	Section4,
	Section5,
	Section6,
	Section7,

	Max
};

class CSceneTutorial : public CScene
{
public:
	CSceneTutorial(StageType StageNum);
	~CSceneTutorial();
	void Update()override;
	void Draw()override;

	void SetInstance();
	void NextPage();
	void BackPage();
private:
	bool m_bEnd;
	TutorialSection m_eSection;
	int m_nCurrentPage;
	std::array<std::vector<Texture*>, (int)TutorialSection::Max> m_pTextureArray;
	SpriteParam m_tTextParam;
	Texture* m_pBackGround;
	SpriteParam m_tBackParam;
	Texture* m_pExplanationScreen;
	SpriteParam m_tExplanationParam;

	//セクション５の画像描画フラグ
	bool m_bExplanationDraw;

	// 移動する前の頂点番号
	int m_nBeforeVertex;
	// 生成処理のエフェクト描画フラグ
	bool m_bSpownEffectDraw;

	Field* m_pField;
	CFieldVertex* m_pFieldVertex;
	CBattle* m_pBattle;
	CBackGround* m_pBackGround2;
	StarLine* m_pStarLine;
	CPlayer* m_pPlayer;

	void UpdateSection1();
	void UpdateSection2();
	void UpdateSection3();
	void UpdateSection4();
	void UpdateSection5();
	void UpdateSection6();
	void UpdateSection7();

	float m_fTime;
};
