#ifndef EASING_FUNCTIONS_H
#define EASING_FUNCTIONS_H

#include <cmath> // For sin, cos, pow

// M_PI が定義されていない場合は手動で定義
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Easying
{

public:
/// <summary>
/// Ease-in Sine 関数
/// </summary>
/// <param name="x">0.0から1.0までの入力値</param>
 static float easeInSine(float x) {
return 1 - cos((x * M_PI) / 2);
}

 /// <summary>
 /// Ease-out Sine 関数
 /// </summary>
 /// <param name="x">0.0から1.0までの入力値</param>
 static float easeOutSine(float x) {
 return sin((x * M_PI) / 2);
 }

  /// <summary>
  /// Ease-in-out Sine 関数
  /// </summary>
  /// <param name="x">0.0から1.0までの入力値</param>
  static float easeInOutSine(float x) {
  return -(cos(M_PI * x) - 1) / 2;
  }

   /// <summary>
   /// Ease-in Quad 関数
   /// </summary>
   /// <param name="x">0.0から1.0までの入力値</param>
 static float easeInQuad(float x) {
   return x * x;
   }

/// <summary>
/// Ease-out Quad 関数
/// </summary>
/// <param name="x">0.0から1.0までの入力値</param>
 static float easeOutQuad(float x) {
return 1 - (1 - x) * (1 - x);
}

 /// <summary>
 /// Ease-in-out Quad 関数
 /// </summary>
 /// <param name="x">0.0から1.0までの入力値</param>
 static float easeInOutQuad(float x) {
 return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
 }

  /// <summary>
  /// Ease-in Cubic 関数
  /// </summary>
  /// <param name="x">0.0から1.0までの入力値</param>
 static  float easeInCubic(float x) {
  return x * x * x;
  }

   /// <summary>
   /// Ease-out Cubic 関数
   /// </summary>
   /// <param name="x">0.0から1.0までの入力値</param>
static float easeOutCubic(float x) {
   return 1 - pow(1 - x, 3);
   }

/// <summary>
/// Ease-in-out Cubic 関数
/// </summary>
/// <param name="x">0.0から1.0までの入力値</param>
 static float easeInOutCubic(float x) {
return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
}

 /// <summary>
 /// Ease-in Quart 関数
 /// </summary>
 /// <param name="x">0.0から1.0までの入力値</param>
 static float easeInQuart(float x) {
 return x * x * x * x;
 }

  /// <summary>
  /// Ease-out Quart 関数
  /// </summary>
  /// <param name="x">0.0から1.0までの入力値</param>
 static float easeOutQuart(float x) {
  return 1 - pow(1 - x, 4);
  }

   /// <summary>
   /// Ease-in-out Quart 関数
   /// </summary>
   /// <param name="x">0.0から1.0までの入力値</param>
 static float easeInOutQuart(float x) {
   return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
   }

/// <summary>
/// Ease-in Quint 関数
/// </summary>
/// <param name="x">0.0から1.0までの入力値</param>
 static float easeInQuint(float x) {
return x * x * x * x * x;
}

 /// <summary>
 /// Ease-out Quint 関数
 /// </summary>
 /// <param name="x">0.0から1.0までの入力値</param>
 static float easeOutQuint(float x) {
 return 1 - pow(1 - x, 5);
 }

  /// <summary>
  /// Ease-in-out Quint 関数
  /// </summary>
  /// <param name="x">0.0から1.0までの入力値</param>
  static float easeInOutQuint(float x) {
  return x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2;
  }

   /// <summary>
   /// Ease-in Expo 関数
   /// </summary>
   /// <param name="x">0.0から1.0までの入力値</param>
  static float easeInExpo(float x) {
   return x == 0 ? 0 : pow(2, 10 * x - 10);
   }

/// <summary>
/// Ease-out Expo 関数
/// </summary>
/// <param name="x">0.0から1.0までの入力値</param>
 static float easeOutExpo(float x) {
return x == 1 ? 1 : 1 - pow(2, -10 * x);
}

 /// <summary>
 /// Ease-in-out Expo 関数
 /// </summary>
 /// <param name="x">0.0から1.0までの入力値</param>
 static float easeInOutExpo(float x) {
 return x == 0 ? 0 : x == 1 ? 1 : x < 0.5 ? pow(2, 20 * x - 10) / 2 : (2 - pow(2, -20 * x + 10)) / 2;
 }

  /// <summary>
  /// Ease-in Circ 関数
  /// </summary>
  /// <param name="x">0.0から1.0までの入力値</param>
  static float easeInCirc(float x) {
  return 1 - sqrt(1 - pow(x, 2));
  }

   /// <summary>
   /// Ease-out Circ 関数
   /// </summary>
   /// <param name="x">0.0から1.0までの入力値</param>
 static float easeOutCirc(float x) {
   return sqrt(1 - pow(x - 1, 2));
   }

/// <summary>
/// Ease-in-out Circ 関数
/// </summary>
/// <param name="x">0.0から1.0までの入力値</param>
 static float easeInOutCirc(float x) {
return x < 0.5 ? (1 - sqrt(1 - pow(2 * x, 2))) / 2 : (sqrt(1 - pow(-2 * x + 2, 2)) + 1) / 2;
}

 /// <summary>
 /// Ease-in Back 関数
 /// </summary>
 /// <param name="x">0.0から1.0までの入力値</param>
  static float easeInBack(float x) {
 const float c1 = 1.70158;
 const float c3 = c1 + 1;
 return c3 * x * x * x - c1 * x * x;
 }

  /// <summary>
  /// Ease-out Back 関数
  /// </summary>
  /// <param name="x">0.0から1.0までの入力値</param>
  static float easeOutBack(float x) {
  const float c1 = 1.70158;
  const float c3 = c1 + 1;
  return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
  }

   /// <summary>
   /// Ease-in-out Back 関数
   /// </summary>
   /// <param name="x">0.0から1.0までの入力値</param>
 static float easeInOutBack(float x) {
   const float c1 = 1.70158;
   const float c2 = c1 * 1.525;
   return x < 0.5
   ? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
   : (pow(2 * x - 2, 2) * ((c2 + 1) * (2 * x - 2) + c2) + 2) / 2;
   }

/// <summary>
/// Ease-in Elastic 関数
/// </summary>
/// <param name="x">0.0から1.0までの入力値</param>
 static float easeInElastic(float x) {
const float c4 = (2 * M_PI) / 3;
return x == 0 ? 0 : x == 1 ? 1 : -pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * c4);
}

 /// <summary>
 /// Ease-out Elastic 関数
 /// </summary>
 /// <param name="x">0.0から1.0までの入力値</param>
 static float easeOutElastic(float x) {
 const float c4 = (2 * M_PI) / 3;
 return x == 0 ? 0 : x == 1 ? 1 : pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1;
 }

  /// <summary>
  /// Ease-in-out Elastic 関数
  /// </summary>
  /// <param name="x">0.0から1.0までの入力値</param>
  static float easeInOutElastic(float x) {
  const float c5 = (2 * M_PI) / 4.5;
  return x == 0 ? 0 : x == 1 ? 1 : x < 0.5
  ? -(pow(2, 20 * x - 10) * sin((20 * x - 11.125) * c5)) / 2
  : (pow(2, -20 * x + 10) * sin((20 * x - 11.125) * c5)) / 2 + 1;
  }

   /// <summary>
   /// Ease-in Bounce 関数
   /// </summary>
   /// <param name="x">0.0から1.0までの入力値</param>
 static float easeInBounce(float x) {
   return 1 - easeOutBounce(1 - x);
   }

/// <summary>
/// Ease-out Bounce 関数
/// </summary>
/// <param name="x">0.0から1.0までの入力値</param>
 static float easeOutBounce(float x) {
const float n1 = 7.5625;
const float d1 = 2.75;
if (x < 1 / d1) {
return n1 * x * x;
}
else if (x < 2 / d1) {
return n1 * (x -= 1.5 / d1) * x + 0.75;
}
else if (x < 2.5 / d1) {
return n1 * (x -= 2.25 / d1) * x + 0.9375;
}
else {
return n1 * (x -= 2.625 / d1) * x + 0.984375;
}
}

 /// <summary>
 /// Ease-in-out Bounce 関数
 /// </summary>
 /// <param name="x">0.0から1.0までの入力値</param>
 static float easeInOutBounce(float x) {
	 return x < 0.5
		 ? (1 - easeOutBounce(1 - 2 * x)) / 2
		 : (1 + easeOutBounce(2 * x - 1)) / 2;
 }

};


#endif // EASING_FUNCTIONS_H