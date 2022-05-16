
#include <ESP8266WiFi.h>
#include "ThingSpeak.h" 
#include <dht11.h>          // dht11 kütüphanesini ekliyoruz.
#define DHT11PIN D1         // DHT11PIN olarak Dijital 1'yi belirliyoruz.
//************************************************************
const char *ssid = "HIBERDATING";      // baglanilacak olan wifi aginin adi  
const char *password = "charmenderr";      // baglanilacak olan wifi aginin sifresi

#define SECRET_CH_ID 1365844            // Secret channel ID si
const char *SECRET_WRITE_APIKEY = "O6YTM6Z5EITP27IY";  // write api key

float sicaklik=0.0, nem=0.0;

dht11 DHT11;                    // DHT11 nesnesi olusturuluyor

WiFiClient  wifi_istemci;
//************************************************************
void setup() {
  Serial.begin(9600);                   //seri portu baslat
  delay(1500);                          // 1500 ms = 1.5sn
  Serial.println("Wifi agına baglanıyor");
  
  WiFi.mode(WIFI_STA);                  // istasyon modunda çalış
  WiFi.begin(ssid, password);           // SSID si yukarıda belirtilmiş olan wifi a bağlan
  
  while (WiFi.status() != WL_CONNECTED) // baglanti kuruldu mu?
  {                                     // kurulmadiysa kurulana kaddar 
    delay(500);                         // 500 ms bekle
    Serial.print(".");                  // ekrana . koy    
  }
  
  Serial.println("");
  Serial.println("Wifi erisim noktasina baglanildi");

  Serial.print("Alinan IP adresi: ");
  Serial.println(WiFi.localIP());       // wifi agindan alinan IP adresini yazdir.

  ThingSpeak.begin(wifi_istemci);  // Initialize ThingSpeak

  pinMode(D0, OUTPUT);      // D0 pinini cikis yap
}
//************************************************************
void loop() 
{
 sicaklik_olc();               // sicakligi olc
 int x = ThingSpeak.writeField(SECRET_CH_ID, 1, sicaklik, SECRET_WRITE_APIKEY);    // alan 1 e sicaklik yaz
  if(x == 200)
    Serial.println("Kanala veri yazildi!");
  else
    Serial.println("Kanala yazma basarisiz. HTTP error code " + String(x));
    delay(18000);     // thingspeak sitesi 15sn de bir güncelleme yapıyor o yüzden 18sn bekle
  x = ThingSpeak.writeField(SECRET_CH_ID, 2, nem, SECRET_WRITE_APIKEY);    // alan 2 e nem yaz
  if(x == 200)
    Serial.println("Kanala veri yazildi!");
  else
    Serial.println("Kanala yazma basarisiz. HTTP error code " + String(x));

  delay(18000);     // thingspeak sitesi 15sn de bir güncelleme yapıyor o yüzden 18sn bekle

  if (sicaklik >= 25 ){
    digitalWrite (D0, HIGH);  // sicaklik 20 den büyük ledi yak
  }
   else
  {
    digitalWrite (D0, LOW);   //sicaklik 20 den kücük ledi sondur
  }

 serial_yazdirma();
 
  
}
//**************** sicaklik olcumu ********************
void sicaklik_olc()
{
  DHT11.read(DHT11PIN);       // sicaklik sensorunu oku
  sicaklik=DHT11.temperature; // sicaklik degerini sicaklik degiskenine ata
  nem=DHT11.humidity;         // nem degerini nem degiskenine ata
  delay(2000);                 // 2000ms bekle
}
void serial_yazdirma()
{
   DHT11.read(DHT11PIN);   // sensoru oku
  
  // Sensörden gelen verileri serial monitörde yazdırıyoruz.
  Serial.print("Nem (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Sicaklik (Celcius): ");
  Serial.println((float)DHT11.temperature, 2);

  // 18 saniye geçiş süresi
  delay(18000);
}
