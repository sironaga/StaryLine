#include "Easing.h"
#include "Defines.h"

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

float Easing38(float t, float c, float b)
{
	return 1 + c * pow(t - 1, 3) + b * pow(t - 1, 2);
}

float Easing39(float t, float d, float p)
{
	if (t == 0) return 0;
	if (t == 1) return 1;
	double s = p / (2 * PI) * asin(1);
	return 1 - pow(2, -10 * t) * sin((t - s) * (2 * PI) / p);
}