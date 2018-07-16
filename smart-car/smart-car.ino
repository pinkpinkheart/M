#include <ESP8266WiFi.h>
#include "TSL2561.h"
#include <Wire.h>

/*以下4行位配置信息,需要修改,具体跟路由器,服务器有关*/

      const char *ssid     = "360WiFi-48681F";//这里是我的wifi，你使用时修改为你要连接的wifi ssid
      const char *password = "dianxin151";//你要连接的wifi密码
      const char *host = "121.42.180.30 ";//修改为Server服务端的IP地址，见https://www.bigiot.net/help/1.html 
      const int tcpPort = 8181;//修改为Server服务端的端口号

      WiFiClient client; // 实例化客户端对象
    
/*串口的初始化和连上WiFi的基本思路如下*/
long last_time = 0;
 int   value1=135;


void setup()        
{
    Wire.begin(0,2);
    Serial.begin(115200);               // 初始化串口,设置波特率为115200
    delay(10);                           // 延迟10ms,串口开启需要一点时间
    Serial.print("Connecting to "); 
    Serial.println(ssid);
    WiFi.begin(ssid, password);         // 连接WiFi
    
  /* 直道成功连上WiFi为止跳出while*/
  
  while (WiFi.status() != WL_CONNECTED)       //WiFi.status() ，这个函数是wifi连接状态，返回wifi链接状态
  {
      delay(500);                               // 每5ms发起一次连接WiFi请求
      Serial.print(".");                        //如果没有连通向串口发送.....
  }
  Serial.println("");                           // 换行作用。println()相当于print('\n')
  Serial.println("WiFi connected");             //串口打印, 连接WiFi成功
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());                //WiFi.localIP()返回8266获得的ip地址,由路由器分配,以下几乎不会使用,只做显示
  if (1) 
  {
    Serial.println("Found sensor");
  } 
  else 
  {
    Serial.println("No sensor?");
    while (1);
  }
// tsl.setGain(TSL2561_GAIN_16X);                 // set 16x gain (for dim situations)
// tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS);  // shortest integration time (bright light)
}
void loop()
{
  while (!client.connected())           //几个非连接的异常处理,当未连接上服务器,或者异常掉线,贝壳40秒未上传数据会掉线,所以当掉线需要在代码中自动连上
  {
    if (!client.connect(host, tcpPort)) // 连接服务器,并判断是否连接成功
    {
      Serial.print("disconnect");        // 连接服务器失败
//      delay(500);                       // 等待5ms后继续连接
    } 
    else 
    {                                  // 连接成功
      Serial.print("chickin");
      client.print("{\"M\":\"checkin\",\"ID\":\"5063\",\"K\":\"d1ae98011\"}\n"); // '\n' 一般为网络传输的结束符,"\r\n"也可以
//       delay(100);                                                                 // 发送数据的等待时间,不要太长,也不要太短,可以修改,有比没有好
    }
/*        
 *     client.print()为客户端对象向服务器发送数据
 *     具体数据内容及格式与服务器有关,贝壳为json,具体键值对见以上网址
 *      这里发送的内容为登录自己在服务器创建的"智能设备",每次创建会分配唯一的ID和APIKEY,告诉服务器你将要说话的"对象",毕竟一个账号可以创建多个"对象"
 *      ID、K对应的值需要改为自己的
*/
  }
/* 程序能运行到这里,说明成功连上服务器和对象*/

/*
   Simple data read example. Just read the infrared, fullspecrtrum diode 
   or 'visible' (difference between the two) channels.
   This can take 13-402 milliseconds! Uncomment whichever of the following you want to read
 */ 
    
  /*
  //uint16_t x = tsl.getLuminosity(TSL2561_FULLSPECTRUM);
  //uint16_t x = tsl.getLuminosity(TSL2561_INFRARED);
  //Serial.println(x, DEC);
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  */


 
  /*
  //Serial.print("IR: "); Serial.print(ir);   Serial.print("\t\t");
  //delay(1000);
  //Serial.print("Full: "); Serial.print(full);   Serial.print("\t");
  //delay(1000);
  //Serial.print("Visible: "); Serial.print(full - ir);   Serial.print("\t");
 // delay(1000);
*/

 
 
/* 
 *  当串口中有数据,可以在串口输入框内输入,在此输入的数据不用转义,但需要在末尾加'\n' 
 *  贝壳示例:{"M":"update","ID":"xx1","V":{"id1":"value1",...}}\n,修改后就可以在串口输入框内输入,然后网址会有你上传的数据,调试成功后可以定时上传传感器的数据 
 *  具体格式见以上网址
 */
//
//    int   value1=111;
//    client.print("{\"M\":\"update\",\"ID\":\"");
//    client.print(5063);
//    client.print("\",\"V\":{\"");
//    client.print(6175);
//    client.print("\":\"");
//    client.print(value1);
//    client.println("\"}}");
//    delay(5000);
//    Serial.println(millis()); 

      updata();
      downcmd();

}




void updata()   //在updata里面填写需要上传的代码
{
  if (millis() - last_time > 2000)
  {
    last_time = millis();


       
    client.print("{\"M\":\"update\",\"ID\":\"");
    client.print(5063);
    client.print("\",\"V\":{\"");
    client.print(6175);
    client.print("\":\"");
    client.print(value1);
    client.println("\"}}");
    delay(500);
    Serial.println(millis()); 
    Serial.println("\":\""); 
//    String s = "{\"M\":\"update\",\"ID\":\"3115\",\"V\":{\"2948\":\"15.3\"}}\n";
//    client.print(s);
//    delay(1000);
//    String t = "{\"M\":\"update\",\"ID\":\"3115\",\"V\":{\"2948\":\"30.5\"}}\n";
//    client.print(t);
  }
}

void downcmd()   //在downcmd里面填写解析下发命令的代码
{
  while (client.available())//，无线读取到的数据转发到到串口
  {
        Serial.println("\"start\""); 
    String s = client.readString();
    String str = "";
//    int count_f; //储存分离出来字符串
//    int count_r; //储存分离出来字符串
//    int num = 0; //储存云端下发的阀值
    if ( s.indexOf("offOn") != -1 )
    {
      Serial.println("offOn_*"); 
    }
    else if ( s.indexOf("play") != -1 )
    {
      Serial.println("\"play_*\""); 
    }
//    Serial.println(s);
//    count_f = s.indexOf("\"C\"");
//    count_r = s.indexOf("\"T\"");
//    if (count_f != -1) //如果有分隔符存在就向下执行
//    {
//      // Serial.println( s.substring(0, count_f)); //打印出第一个逗号位置的字符串
//      str = s.substring(count_f + 5, count_r - 2); //打印字符串，从当前位置+5开始 得到不带引号的字符数字
//      //      Serial.println(str);
//      char cstr[10] = "";
//      for (int i = 0; i < 10; i++)
//      {
//        cstr[i] = str[i];
//      }
//      num = atoi(cstr);
//      Serial.println(num);
//    }
  }
}
