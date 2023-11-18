
#include <PinChangeInt.h>    //外部中断
#include <MsTimer2.h>        //定时中断





/////////编码器引脚////////
#define ENCODER_L 8  //编码器采集引脚 每路2个 共4个
#define DIRECTION_L 4
#define ENCODER_R 7
#define DIRECTION_R 2


volatile long Velocity_L, Velocity_R ;   //左右轮编码器数据
int Velocity_Left, Velocity_Right = 0, Velocity, Turn;   //左右轮速度

/*********函数功能：5ms控制函数 核心代码 作者：平衡小车之家*******/
void control() {
  sei();//全局中断开启
  Velocity_Left = Velocity_L;    Velocity_L = 0;  //读取左轮编码器数据，并清零，这就是通过M法测速（单位时间内的脉冲数）得到速度。
  Velocity_Right = Velocity_R;    Velocity_R = 0; //读取右轮编码器数据，并清零

}

/***********函数功能：初始化 相当于STM32里面的Main函数 作者：平衡小车之家************/
void setup()   {


  pinMode(ENCODER_L, INPUT);       //编码器引脚
  pinMode(DIRECTION_L, INPUT);       //编码器引脚
  pinMode(ENCODER_R, INPUT);        //编码器引脚
  pinMode(DIRECTION_R, INPUT);       //编码器引脚

  delay(200);                      //延时等待初始化完成
  attachInterrupt(0, READ_ENCODER_R, CHANGE);           //开启外部中断 编码器接口1
  attachPinChangeInterrupt(4, READ_ENCODER_L, CHANGE);  //开启外部中断 编码器接口2
  MsTimer2::set(10, control);       //使用Timer2设置5ms定时中断
  MsTimer2::start();               //中断使能
}
/******函数功能：主循环程序体*******/
void loop(){
  printf("EncoderA: %d",Velocity_L);
  delay(50);

}
/*****函数功能：外部中断读取编码器数据，具有二倍频功能 注意外部中断是跳变沿触发********/
void READ_ENCODER_L() {
  if (digitalRead(ENCODER_L) == LOW) {     //如果是下降沿触发的中断
    if (digitalRead(DIRECTION_L) == LOW)      Velocity_L--;  //根据另外一相电平判定方向
    else      Velocity_L++;
  }
  else {     //如果是上升沿触发的中断
    if (digitalRead(DIRECTION_L) == LOW)      Velocity_L++; //根据另外一相电平判定方向
    else     Velocity_L--;
  }
}
/*****函数功能：外部中断读取编码器数据，具有二倍频功能 注意外部中断是跳变沿触发********/
void READ_ENCODER_R() {
  if (digitalRead(ENCODER_R) == LOW) { //如果是下降沿触发的中断
    if (digitalRead(DIRECTION_R) == LOW)      Velocity_R++;//根据另外一相电平判定方向
    else      Velocity_R--;
  }
  else {   //如果是上升沿触发的中断
    if (digitalRead(DIRECTION_R) == LOW)      Velocity_R--; //根据另外一相电平判定方向
    else     Velocity_R++;
  }
}


