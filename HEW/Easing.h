// EasingŒvZˆ—
#pragma once
#include <cmath>

float InEasing(float Time, float StartValue, float ValueDifference, float Tween);
float OutEasing(float Time, float StartValue, float ValueDifference, float Tween);
float InOutEasing(float Time, float StartValue, float ValueDifference, float Tween);
float Easing38(float Time, float c = 1.5, float b = 0.8);
float Easing39(float Time, float d = 0.3, float p = 0.4);