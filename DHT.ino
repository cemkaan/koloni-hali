// -----------------------------------------
//
//                   KÜTÜPHANE 
//
// -----------------------------------------
// arduino_uip-master - Version: Latest 
#include <Dhcp.h>
#include <Dns.h>
#include <UIPEthernet.h>

// SPI - Version: Latest 
#include <SPI.h>

// standart sensör kütüphaneleri
#include <DHT.h>

// dweet 
#include <Dweet.h>
#include <string.h>

// SD - Version: Latest 
//#include <SD.h>
// -----------------------------------------
//
//                   AYARLAR VE TANIMLAMA 
//
// -----------------------------------------

// dht sensöre bağlı delik.
#define DHTPIN           2         

// dth türü
#define DHTTYPE           DHT11     // DHT 11 

// SENSÖRLERİN OKUMA ARALIĞI dk olarak
const uint8_t okumaAraligi = 5;

// dweetlenecek şeyin adı
const char*  seyAd = "Yalova-Pasakent-63ba3722";

// sd karta yazılacakları TUTAMAĞI
//String veriDizisi =""; 

// dweetleneceklerin yazılacakları TUTAMAK
String veridenJsona   = ""; 

// boş bir mac adresi ver
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xAD };

// -----------------------------------------
//
//                   BAŞLAMA 
//
// -----------------------------------------

// dht başlat
DHT dht(DHTPIN, DHTTYPE);

//  Ethernet client library başlat
EthernetClient client;

// Dweet client library başlat
Dweet dweet(client);

// sd kart başlat
// sdBasla(); 

  
  
// -------------------------------------------------------------------------------
//                                SETUP BAŞLANGIÇ

void setup() {

  // seri bağlantı aç ve bekle:
  Serial.begin(9600);
  while (!Serial) {
    ; // seri bağlantının bağlanmasını bekle
  }
// -----------------------------------------
  
  // dht başlat
  dht.begin();  
// -----------------------------------------
 
  // Start the Ethernet connection
  Ethernet.begin(mac);

  // Give the Ethernet shield a second to initialize
  delay(500);

}
//                              SETUP BİTİŞ
// -----------------------------------------------------------------------------

 
 
 
// ------------------------------------------------------------------------------
//                            LOOP BAŞLANGIÇ
//

void loop() {
 delay(250);
  // Nemi ölç
  byte NEM = dht.readHumidity();
  
  // Isı ölç
  int8_t  ISI = dht.readTemperature();


  // sorun varmı kontrol et
  if (isnan(NEM) || isnan(ISI)) {
    Serial.println("DHT sensörü okunamadı!");
    return;
  }

    // Nemi Seri porta Yaz
    Serial.print("Nem: ");
    Serial.print("% ");
    Serial.println(NEM);
    // Isıyı  Seri porta Yaz
    Serial.print("Isı: ");
    Serial.print(ISI);
    Serial.println(" *C ");

    // VERİ DİZİSİ OLUŞTUR
    // convert to json
    //{\"ISI\": 18,\"NEM\": 50}
    
    veridenJsona = "{\"ISI\": "; 
    veridenJsona += ISI;
    veridenJsona += ",\"NEM\": ";
    veridenJsona += NEM;
    veridenJsona += "}";
/*
char veridenJsona[] = "{\"ISI\": ";
 char nemBolumu[] = ",\"NEM\": ";
 char bitisBolumu[] = "}";
 
strcat(veridenJsona, ISI);
strcat(veridenJsona, nemBolumu);
strcat(veridenJsona, NEM);
strcat(veridenJsona, bitisBolumu);*/
   Serial.print(veridenJsona);
// -----------------------------------------


    // dweetle
    dweetle(); 
// -----------------------------------------
/*
    // VERİ DİZİSİ OLUŞTUR
veriDizisi = String(ISI) + "," + String(NEM) + "," + String(kilo); // convert to CSV
// -----------------------------------------
   
    // SD Karta kayıt et
    kayitEt(); 
// -----------------------------------------
*/
    delay(okumaAraligi*60000); // okuma aralığı kadar tekrarla
}

//
//                   LOOP BİTİŞ
// -----------------------------------------------------------------------------


// -----------------------------------------
//                dweet.io ya gönderme
/**
 *  dweetle :  Fonksiyon dweetliyor
 *
 * 
 */
void dweetle()
{
    
        // dweet 
  dweet.dweet(seyAd, veridenJsona); 
  Serial.print("ŞEYİN ADI:");
  Serial.println(seyAd);
  Serial.print("Gönderilen json:");
   Serial.println(veridenJsona);
  delay(250);
  // en son dweeti al !
   char* cevap = dweet.get_latest(seyAd);
  // Serial monitöre yaz
  Serial.println(cevap);

}
//
// -----------------------------------------

// -----------------------------------------
//                sd kart başlatma
/**
 *  sdBasla :  Kart başlat
 *
 * 
 */
void sdBasla ()
{
  ///////////////////////////////////////////////////////////////
  ////////////           SD KART TEST  BAŞLANGIÇ             ////

  if (!SD.begin(KARTPINI)) {
    Serial.println("* KART TAKILIMI?");
    Serial.print("* kart buraya mı bağlı:");
    Serial.println(KARTPINI);
    return;
  } else {
    Serial.println("Kart takılı.");
  }

  // TABLO DATA1.CSV VAR MI?
  if (SD.exists("DATA1.CSV")) {
    Serial.println("data1.csv dosyası var");
  } else {
    Serial.println("DATA1.CSV YOK");
  }

  ////////////           SD KART TEST  SON                   ////
  ///////////////////////////////////////////////////////////////
}


// -----------------------------------------
//                 SD KARTA YAZIM
/**
 *  kayitEt :  Fonksiyon önce data1.csv dosyasının olup olmadığını kontrol ediyor sonra açıp yazıyor ve kapatıyor.
 *
 * 
 */
void kayitEt()
{
    
if(SD.exists("DATA1.CSV")){ // DOSYA VAR MI KONTROL

// DOSYAYI YAZILACAK ŞEKİLDE AÇ
File veriler = SD.open("DATA1.CSV", FILE_WRITE);
// -----------------------------------------
// YENİ VERİYİ DOSYAYA EKLE

if (veriler){
veriler.println(veriDizisi);
veriler.close(); // DOSYAYI KAPAT
}
}
// -----------------------------------------
else{
Serial.println("dosyaya yazılamadı !");
}
}
//
// -----------------------------------------
