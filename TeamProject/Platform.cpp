#include "Platform.h"

Platform::Platform()
{
	//이미지를 가져와서 텍스처에 붙이기
	//가장 바닥, 그라운드를 선언할 때 사용 x = 0, y = 0
}

Platform::Platform(double x, double y)
{
	//공중에 떠 있는 플랫폼들을 선언할 때 사용
	//x , y 는 따로 받아야함
}

Platform::~Platform()
{
	//사용한 자원 반환
}