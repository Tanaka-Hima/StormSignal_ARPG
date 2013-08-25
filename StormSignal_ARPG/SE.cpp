#include "SE.h"

vector<int> SE::SE_Files;
int SE::Volume = 70;

int SE::Register(const string FileName)
{//Ä¶‚·‚éSE‚ğ•ÏX
	SE_Files.push_back(LoadSoundMem(FileName.c_str()));
	int Index = SE_Files.size() - 1;
	ChangeVolumeSoundMem(2.55 * Volume,SE_Files[Index]);
	return Index;
}

void SE::Play(int Index)
{//SE‚ğÄ¶
	if(Index < 0 || SE_Files.size() <= Index)return;
	PlaySoundMem(SE_Files[Index],DX_PLAYTYPE_BACK);
}

void SE::Stop(int Index,bool StopAllSE)
{//SE‚ğ’â~‚·‚é
	if(StopAllSE)
	{
		for(int i=0;i<SE_Files.size();i++)
		{
			if(CheckSoundMem(SE_Files[i]))StopSoundMem(SE_Files[i]);
		}
		return;
	}
	if(Index < 0 || SE_Files.size() <= Index)return;
	if(CheckSoundMem(SE_Files[Index]))StopSoundMem(SE_Files[Index]);
}

void SE::Volume_Change(int InputVolume)
{//SE‚Ì‰¹—Ê‚ğ•ÏX(100 = 100% = 255)
	Volume = InputVolume;
	for(int i=0;i<SE_Files.size();i++)
	{
		ChangeVolumeSoundMem(2.55 * Volume,SE_Files[i]);
	}
}
