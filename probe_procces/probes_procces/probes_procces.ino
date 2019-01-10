#include "DHT.h" //Biblioteca retirada de https://github.com/adafruit/DHT-sensor-library
#include "ArduinoJson.h" //Biblioteca retirada de https://github.com/bblanchon/ArduinoJson

/*Definicoes e estruturas para trabalhar com sensor DHT11*/
#define DHTPIN_1 A1
#define DHTPIN_2 A2
#define DHTTYPE DHT11
DHT dht_1(DHTPIN_1, DHTTYPE);
DHT dht_2(DHTPIN_2, DHTTYPE);
/*Estrutura de dado para trabalhar com os sensores, responsavel pelo envio da informação para o servidor*/
struct probe{
  int id;
  char *descriptor;
  int value;
};
/*Instanciação dos sensores*/
probe P1;
probe P2;
probe P3;
probe P4;
/*Função responsável para recuperar os valores dos sensores, recebendo como parametro o id e o endereco de memoria em que se deseja atualizar o valor*/
void readFromProbes(int id, int *value)
{
  switch(id)
  {
    case 1:
      *value = (int) dht_1.readTemperature();
      break;
    case 2:
      *value = (int) dht_1.readHumidity();
      break;
    case 3:
      *value = (int) dht_2.readTemperature();
      break;
    case 4:
      *value = (int) dht_2.readHumidity();
      break; 
  }
}
/*Buffer JSON da biblioteca ArduinoJSON*/
DynamicJsonBuffer jsonBuffer;

void setup()
{
  /*Inicializa a serial*/
  Serial.begin(9600);
  /*Inicializa os modulos DHT11*/
  dht_1.begin();
  dht_2.begin();
  /*Inicializa as estrutras dos sensores*/
  P1.id = 1;
  P1.descriptor = (char *)"T1";
  P2.id = 2;
  P2.descriptor = (char *)"U1";
  P3.id = 3;
  P3.descriptor = (char *)"T2";
  P4.id = 4;
  P4.descriptor = (char *)"U2";
}

void loop() {
  /*Realiza as leituras dos sensores*/
  readFromProbes(P1.id,&P1.value);
  readFromProbes(P2.id,&P2.value);
  readFromProbes(P3.id,&P3.value);
  readFromProbes(P4.id,&P4.value);
  /*Monta o objeto JSON*/
  JsonArray& array = jsonBuffer.createArray();
  JsonObject& j_obj1 = jsonBuffer.createObject();
  JsonObject&obj_idf1 = j_obj1.createNestedObject("probesidf");
  obj_idf1["id_value"] = P1.id;
  obj_idf1["description"] = P1.descriptor;
  j_obj1["read_value"] = P1.value;
  JsonObject& j_obj2 = jsonBuffer.createObject();
  JsonObject&obj_idf2 = j_obj2.createNestedObject("probesidf");
  obj_idf2["id_value"] = P2.id;
  obj_idf2["description"] = P2.descriptor;
  j_obj2["read_value"] = P2.value;
  JsonObject& j_obj3 = jsonBuffer.createObject();
  JsonObject&obj_idf3 = j_obj3.createNestedObject("probesidf");
  obj_idf3["id_value"] = P3.id;
  obj_idf3["description"] = P3.descriptor;
  j_obj3["read_value"] = P3.value;
  JsonObject& j_obj4 = jsonBuffer.createObject();
  JsonObject&obj_idf4 = j_obj4.createNestedObject("probesidf");
  obj_idf4["id_value"] = P4.id;
  obj_idf4["description"] = P4.descriptor;
  j_obj4["read_value"] = P4.value;
  array.add(j_obj1);
  array.add(j_obj2);
  array.add(j_obj3);
  array.add(j_obj4);
  /*Envia para a serial*/
  array.printTo(Serial);
  Serial.flush();
  delay(30000);
  //delay(120000);
}
