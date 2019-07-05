#define BEATTIME1 500
#define BEATTIME2 300
#define BEATTIME3 100

const int xAxisPin = 1;  // x軸:6番
const int yAxisPin = 2;  // y軸:7番
const int zAxisPin = 3;  // z軸:8番
const int ledPin = 12;
const int spkPin = 13;

// 猫背と判断する閾値
const int thresholdStoopLow = 15;
const int thresholdStoopHigh = 50;

// 猫背と判断する閾値を超えていたらtrue，なければfalse
boolean stoopFlag = false;

// 猫背状態が次の時間続いたら猫背と判断
const int stoopTimer = 5000;

// 普通の状態から猫背状態になった時の時刻
unsigned long stoopTimePrev;

unsigned long timeInit, timeNow;

void setup() {
  Serial.begin(9600);
  pinMode(spkPin, OUTPUT);
  digitalWrite(spkPin, LOW);
  pinMode(ledPin, OUTPUT);
  timeInit = millis();
}

void loop() {
  // 現在の時刻を取得する
  timeNow = millis() - timeInit;

  // 加速度センサから生データ取得
  //int xAxisValue = analogRead(xAxisPin);
  //int yAxisValue = analogRead(yAxisPin);
  int zAxisValue = analogRead(zAxisPin);

  // 読み取った値を-1から1までの範囲にスケーリングしてsinθの値とする
  //float xAxisSinTheta = mapInFloat(xAxisValue, 265, 724, -1, 1);
  //float yAxisSinTheta = mapInFloat(yAxisValue, 267, 725, -1, 1);
  float zAxisSinTheta = mapInFloat(zAxisValue, 315, 715, -1, 1);

  // -1から1までの範囲に制限
  //xAxisSinTheta = constrain(xAxisSinTheta, -1, 1);
  //yAxisSinTheta = constrain(yAxisSinTheta, -1, 1);
  zAxisSinTheta = constrain(zAxisSinTheta, -1, 1);

  // 逆サインを求めた結果(単位:ラジアン)を度に変換
  //xAxisTilt = floor(asin(xAxisSinTheta) * 180 / PI);
  //yAxisTilt = floor(asin(yAxisSinTheta) * 180 / PI);
  float zAxisTilt = floor(asin(zAxisSinTheta) * 180 / PI);

  // センサから現在の傾き(-90から90)を読み取る
  int tilt = (int)zAxisTilt;

  // 猫背の角度になったら
  if ( thresholdStoopLow < tilt && tilt < thresholdStoopHigh){
    if (stoopFlag == false){
      stoopFlag = true;
      stoopTimePrev = timeNow;
    }
    
    // led点灯
    digitalWrite(ledPin, HIGH);
    
    // stoopTimer秒間経過したら警告
    if ( stoopTimer < timeNow - stoopTimePrev ){
      if ( stoopTimer < timeNow - stoopTimePrev && timeNow - stoopTimePrev < stoopTimer + 5000 ){
        tone(spkPin, 131, BEATTIME1);
        delay(BEATTIME1);
      }
      else if ( stoopTimer + 5000 < timeNow - stoopTimePrev && timeNow - stoopTimePrev < stoopTimer + 10000 ){
        tone(spkPin, 262, BEATTIME2);
        delay(BEATTIME2);
      }
      else {
        tone(spkPin, 523, BEATTIME3);
        delay(BEATTIME3);
      }
    }
  }
  else {
    stoopFlag = false;
    digitalWrite(spkPin, LOW);
    digitalWrite(ledPin, LOW);
  }
  
  //Serial.print(" X:");
  //Serial.print(xAxisTilt);
  //Serial.print(" Y:");
  //Serial.print(yAxisTilt);
  
  /*
  Serial.print("zValue");
  Serial.print(zAxisValue);
  Serial.print("  timeNow:");
  Serial.print(timeNow/1000);
  Serial.print("  stoopTimePrev:");
  Serial.print(stoopTimePrev/1000);
  Serial.print("  Z:");
  Serial.print(zAxisTilt);
  Serial.print("  tilt:");
  Serial.println(tilt);
  */
  
  delay(100);
}

// 標準のmap関数は引数と戻り値の型がlong
// 今回は-1から1までにスケーリングする必要があるためfloatで同じ計算をする
float mapInFloat(float x, float iMin, float iMax, float oMin, float oMax){
  return (x - iMin) * (oMax - oMin) / (iMax - iMin) + oMin;
}

