#pragma once

class CFade
{
public:
	CFade();
	virtual ~CFade();
	void Update();
	void Draw();

	void Start(bool isIn);
	void SetFade(float time, bool isIn);
	bool IsFinish() { return m_time <= 0.0f; }
	bool IsFadeIn() { return m_isFadeIn; }
	bool IsFadeOut() { return !m_isFadeIn; }

	float GetRate();
	float GetAlpha();


protected:
	virtual void DrawFadeIn(float alpha);
	virtual void DrawFadeOut(float alpha);


private:
	bool m_isFadeIn; // フェードインフラグ 
	float m_time;   // 現在のフェードの経過時間 
	float m_maxTime;  // フェードの最大時間
};