#include <vector>
#include <DxLib.h>

using namespace std;

static class SE
{
private:
	static vector<int> SE_Files;
	static int Volume;
public:
	int Register(const string FileName);
	void Play(int Index);
	void Stop(int Index,bool StopAllSE = false);
	void Volume_Change(int InputVolume);
}SEManager;