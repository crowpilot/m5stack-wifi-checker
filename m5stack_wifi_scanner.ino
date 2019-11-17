
#include "WiFi.h"

#include <M5Stack.h>


int rssi[200][14] = {0};

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print("Wi-Fi channel scanner");
  M5.Lcd.setTextSize(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  int n = WiFi.scanNetworks();
 // M5.Lcd.clearDisplay();
    M5.Lcd.setCursor(0,30);
    M5.Lcd.print("ch");
  for (int i = 0; i < 13; i++) {
    M5.Lcd.setCursor(30 + i * 15, 30);
    M5.Lcd.printf("%2d", i + 1);
  }
  M5.Lcd.setCursor(246, 30);
  M5.Lcd.printf("%2d", 14);
  if (n == 0) {
    M5.Lcd.println("no networks found");
  }
  else {
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.print(n);
    M5.Lcd.println(" networks found");


    for (int i = 199; i > 0; i--) {
      int freq[100] = {};
      //y axi
      for (int j = 0; j < 14; j++) {
        //make x
        rssi[i][j] = rssi[i - 1][j];
        if (rssi[i][j] > 0) {
          if (j < 14) {
            freq[7 + 5 * j] = rssi[i][j];
            for (int k = 1; k < 12; k++) {
              freq[7 + 5 * j + k] += freq[7 + 5 * j] * (12 - k) / 12;
              freq[7 + 5 * j - k] += freq[7 + 5 * j] * (12 - k) / 12;
            }
          }
          else {
            freq[84] = rssi[i][j];
            for (int k = 1; k < 12; k++) {
              freq[84 + k] += 12 - k;
              freq[84 - k] += 12 - k;
            }
          }
          //M5.Lcd.drawPixel(freq*3,i,rssi[i][j]);
        }
      }
      //make draw pixel
      for (int j = 0; j < 100; j++) {
        if (freq[j] > 200) {
          M5.Lcd.drawLine(j * 3, 40 + i, j * 3 + 3, 40 + i, 0xf800);
        } else if (freq[j] > 100) {
          M5.Lcd.drawLine(j * 3, 40 + i, j * 3 + 3, 40 + i, 0xf800 + (int(63 * (200 - freq[j]) / 100) << 5));
        } else if (freq[j] > 0) {
          M5.Lcd.drawLine(j * 3, 40 + i, j * 3 + 3, 40 + i, 0x07E0 + (int(31 * freq[j] / 100) << 11));
        }else{
          M5.Lcd.drawLine(j*3,40+i,j*3+3,40+i,0x0000);
        }
      }
    }

    for (int i = 0; i < 14; i++) {
      rssi[0][i] = 0;
    }

    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      /*M5.Lcd.print(i + 1);
        M5.Lcd.print(": ");
        M5.Lcd.print(WiFi.SSID(i));
        M5.Lcd.print(" ");
        M5.Lcd.print(WiFi.channel(i));
        M5.Lcd.print("CH");
        M5.Lcd.print(" (");
        M5.Lcd.print(WiFi.RSSI(i));
        M5.Lcd.print(")");
        M5.Lcd.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");*/
      //M5.update();
      //delay(3);

      if (WiFi.channel(i) > -40) {
        rssi[0][WiFi.channel(i)] += 70;
      } else if (WiFi.channel(i) > -70) {
        rssi[0][WiFi.channel(i)] += 30;
      }
      else if (WiFi.channel(i) > -90) {
        rssi[0][WiFi.channel(i)] += 10;
      }
      else {
        rssi[0][WiFi.channel(i)] += 5;
      }
    }
    //for
  }
  //draw rssi
  M5.update();

}
