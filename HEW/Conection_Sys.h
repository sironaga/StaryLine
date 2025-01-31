#pragma once
#include<iostream>
struct Acquired_Data
{
	int Stage_Number;
	int nScore;
	bool bNew;
	std::string UserName;
};
void Conect_Init();
void SetIP(const char* IP);
void SendData(Acquired_Data InData);