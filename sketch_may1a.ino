
#include <RPLidar.h>

// 1. تعريف كائن الليدار ومنافذ التحكم
RPLidar lidar;
#define RPLIDAR_MOTOR 5 // Pin التحكم في موتور الليدار (PWM)

void setup() {
  // قناة الاتصال مع الكمبيوتر - خلي السرعة 115200 دايماً
  Serial.begin(115200);   
  
  // قناة الاتصال مع الليدار (الـ Nano ESP32 بيستخدم Serial1 على Pins 0 و 1)
  Serial1.begin(115200);  
  lidar.begin(Serial1);
  
  // إعداد منفذ الموتور
  pinMode(RPLIDAR_MOTOR, OUTPUT);
  
  Serial.println("--- ASAR LiDAR System Initialized ---");
  delay(1000);
}

void loop() {
  // محاولة التقاط نقطة بيانات واحدة
  if (IS_OK(lidar.waitPoint())) {
    
    // سحب القيم الخام (Raw Data)
    float distance = lidar.getCurrentPoint().distance; // المسافة بالمليمتر
    float angle    = lidar.getCurrentPoint().angle;    // الزاوية (0-360)
    byte  quality  = lidar.getCurrentPoint().quality;  // جودة الإشارة (0-15)

    // تشغيل موتور الليدار بأقصى سرعة
    analogWrite(RPLIDAR_MOTOR, 255);

    // طباعة البيانات بشكل منظم (Tab-Separated) عشان تكون سهلة القراءة
    Serial.print("Angle: ");   Serial.print(angle, 2);
    Serial.print("\t | Distance: "); Serial.print(distance, 2);
    Serial.print(" mm\t | Quality: "); Serial.println(quality);

  } else {
    // في حالة فقدان الإشارة أو حدوث خطأ (Error Handling)
    handleError();
  }
}

// دالة التعامل مع الأخطاء وإعادة تشغيل الحساس تلقائياً
void handleError() {
  analogWrite(RPLIDAR_MOTOR, 0); // وقف الموتور لحظياً
  
  rplidar_response_device_info_t info;
  if (IS_OK(lidar.getDeviceInfo(info, 100))) {
     lidar.startScan(); // إعادة بدء المسح
     analogWrite(RPLIDAR_MOTOR, 255);
     delay(1000);
  }
}