#include "SPI.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SPI_SCK 5
#define SPI_SDA 18
#define SPI_CS  21

#define VFD_RESET 4
#define VFD_BUSY  15

static const int spiClk = 1000000;
SPIClass * vspi = NULL;

char Row0Data[21] = {"-SPI TEST DEMO--"};
char Row1Data[21] = {"EVC16X02---DWP2K"};

char TimeCount[7];

void VFD_DataTransmit(uint8_t Data)
{
  vspi->beginTransaction(SPISettings(spiClk, LSBFIRST, SPI_MODE3));
  vspi->transfer(Data);
  vspi->endTransaction();
}

void setup()
{

  pinMode(SPI_CS, OUTPUT);
  pinMode(VFD_RESET, OUTPUT);
  pinMode(VFD_BUSY, INPUT);

  vspi = new SPIClass(VSPI);
  vspi->begin(SPI_SCK, -1, SPI_SDA, -1);
  vspi->setDataMode(SPI_MODE3);

  digitalWrite(SPI_CS, 1);

  digitalWrite(VFD_RESET, 0);
  delay(50);
  digitalWrite(VFD_RESET, 1);

  delay(200);

  digitalWrite(SPI_CS, 0);
  VFD_DataTransmit(0xB1);
  VFD_DataTransmit(0x01);
  digitalWrite(SPI_CS, 1);

  delayMicroseconds(200);

  digitalWrite(SPI_CS, 0);
  VFD_DataTransmit(0xA0);
  VFD_DataTransmit(255);
  digitalWrite(SPI_CS, 1);

  delayMicroseconds(200);

  digitalWrite(SPI_CS, 0);
  VFD_DataTransmit(0xCA);
  VFD_DataTransmit(0x00);
  for (uint8_t i = 0; i < 16; i ++)
  {
    VFD_DataTransmit(0x7F);
  }
  digitalWrite(SPI_CS, 1);

  delayMicroseconds(200);

  digitalWrite(SPI_CS, 0);
  VFD_DataTransmit(0xCB);
  VFD_DataTransmit(0x00);
  for (uint8_t i = 0; i < 16; i ++)
  {
    VFD_DataTransmit(Row1Data[i]);
  }
  digitalWrite(SPI_CS, 1);

  delayMicroseconds(200);

  digitalWrite(SPI_CS, 0);
  VFD_DataTransmit(0xCD);
  VFD_DataTransmit(0x00);
  VFD_DataTransmit(0x00);
  VFD_DataTransmit(0x55);
  VFD_DataTransmit(0xaa);
  VFD_DataTransmit(0x55);
  VFD_DataTransmit(0xaa);
  VFD_DataTransmit(0x55);
  digitalWrite(SPI_CS, 1);

  delayMicroseconds(200);

  digitalWrite(SPI_CS, 0);
  VFD_DataTransmit(0xCC);
  VFD_DataTransmit(0x00);
  for (uint8_t i = 0; i < 16; i ++)  // Write 
  {
    VFD_DataTransmit(0x0f);
  }
  digitalWrite(SPI_CS, 1);

  delayMicroseconds(200);

}

void WaitBusy()
{
  delayMicroseconds(30);
  while (digitalRead(VFD_BUSY))
  {

  }
}

void loop()
{
  strcpy(Row0Data, "                ");
  itoa(millis() / 10, TimeCount, 10);
  strcpy(Row0Data, "Test Demo-");
  strcat(Row0Data, TimeCount);


  digitalWrite(SPI_CS, 0);
  VFD_DataTransmit(0xCA);
  VFD_DataTransmit(0x00);
  for (uint8_t i = 0; i < 16; i ++)
  {
    VFD_DataTransmit(Row0Data[i]);   // Write Row0 DCRAM
  }
  digitalWrite(SPI_CS, 1);


  //delayMicroseconds(150);
  WaitBusy();

}
