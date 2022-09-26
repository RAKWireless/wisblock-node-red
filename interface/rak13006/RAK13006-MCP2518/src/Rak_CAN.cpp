
/**
   @file RAK13006_CAN_LoopBack.ino
   @author rakwireless.com
   @brief  This code is designed to config CAN device
   @version 0.1
   @date 2022-2-28
   @copyright Copyright (c) 2022
**/

#include <Rak_CAN.h>
#include <SPI.h>

const uint8_t MCP2518_SCK = SCK;
const uint8_t MCP2518_MOSI = MOSI;
const uint8_t MCP2518_MISO = MISO;

const uint8_t MCP2518_CS = SS;
const uint8_t MCP2518_INT = WB_IO6;

#if defined(_VARIANT_RAK4630_)
#define BOARD "RAK4631"
SPIClass SPI_USER(NRF_SPIM3, MCP2518_MISO, MCP2518_SCK, MCP2518_MOSI);
#elif defined(_VARIANT_RAK11300_)
#define BOARD "RAK11300 "
MbedSPI SPI_USER(MCP2518_MISO, MCP2518_MOSI, MCP2518_SCK);
#else
#define BOARD "RAK11200 "
SPIClass SPI_USER;
#endif

void RAK_CAN::init(void)
{
#if defined(_VARIANT_RAK4630_)
#define BOARDFLAG 1
  SPI_USER.begin();
#elif defined(_VARIANT_RAK11300_)
#define BOARDFLAG 1
  SPI_USER.begin();
#else
#define BOARDFLAG 0
  SPI_USER.begin(MCP2518_SCK, MCP2518_MISO, MCP2518_MOSI);
#endif
}

RAK_CAN Rak_Can(MCP2518_CS, SPI_USER, MCP2518_INT);