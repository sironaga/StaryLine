#pragma once
struct Acquired_Data
{
	int Stage_Number;
	int nScore;
	bool bNew;
};
void Conect_Init();
void SetIP(const char* IP);
void SendData(Acquired_Data InData);