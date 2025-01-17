#include "Easing.h"

// --- EasingåvéZóp
float InEasing(float Time, float StartValue, float ValueDifference, float Tween)
{
	Time /= Tween;
	return ValueDifference * Time * Time + Tween;
}

float OutEasing(float Time, float StartValue, float ValueDifference, float Tween)
{
	Time /= Tween;
	return -ValueDifference * Time * (Time - 2.0f) + Tween;
}

float InOutEasing(float Time, float StartValue, float ValueDifference, float Tween)
{
	Time /= Tween / 2.0f;
	if (Time < 1.0f)
	{
		return ValueDifference / 2.0f * Time * Time + Tween;
	}
	Time = Time - 1.0f;
	return -ValueDifference / 2.0f * (Time * (Time - 2.0f) - 1.0f) + Tween;
}
