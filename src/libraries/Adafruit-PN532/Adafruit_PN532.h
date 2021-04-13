/**************************************************************************/
/*!
    @file     Adafruit_PN532.h
    @author   Adafruit Industries
        @license  BSD (see license.txt)


        This is a library for the Adafruit PN532 NFC/RFID breakout boards
        This library works with the Adafruit NFC breakout
        ----> https://www.adafruit.com/products/364

        Check out the links above for our tutorials and wiring diagrams
  These chips use SPI or I2C to communicate.

        Adafruit invests time and resources providing this open source code,
        please support Adafruit and open-source hardware by purchasing
        products from Adafruit!

        @section  HISTORY

  v2.0  - Refactored to add I2C support from Adafruit_NFCShield_I2C library.

        v1.1  - Added full command list
          - Added 'verbose' mode flag to constructor to toggle debug output
          - Changed readPassiveTargetID() to return variable length values

*/
/**************************************************************************/

#ifndef ADAFRUIT_PN532_H
#define ADAFRUIT_PN532_H

#include "Arduino.h"

#define PN532_PREAMBLE (0x00)
#define PN532_STARTCODE1 (0x00)
#define PN532_STARTCODE2 (0xFF)
#define PN532_POSTAMBLE (0x00)

#define PN532_HOSTTOPN532 (0xD4)
#define PN532_PN532TOHOST (0xD5)

// PN532 Commands
#define PN532_COMMAND_DIAGNOSE (0x00)
#define PN532_COMMAND_GETFIRMWAREVERSION (0x02)
#define PN532_COMMAND_GETGENERALSTATUS (0x04)
#define PN532_COMMAND_READREGISTER (0x06)
#define PN532_COMMAND_WRITEREGISTER (0x08)
#define PN532_COMMAND_READGPIO (0x0C)
#define PN532_COMMAND_WRITEGPIO (0x0E)
#define PN532_COMMAND_SETSERIALBAUDRATE (0x10)
#define PN532_COMMAND_SETPARAMETERS (0x12)
#define PN532_COMMAND_SAMCONFIGURATION (0x14)
#define PN532_COMMAND_POWERDOWN (0x16)
#define PN532_COMMAND_RFCONFIGURATION (0x32)
#define PN532_COMMAND_RFREGULATIONTEST (0x58)
#define PN532_COMMAND_INJUMPFORDEP (0x56)
#define PN532_COMMAND_INJUMPFORPSL (0x46)
#define PN532_COMMAND_INLISTPASSIVETARGET (0x4A)
#define PN532_COMMAND_INATR (0x50)
#define PN532_COMMAND_INPSL (0x4E)
#define PN532_COMMAND_INDATAEXCHANGE (0x40)
#define PN532_COMMAND_INCOMMUNICATETHRU (0x42)
#define PN532_COMMAND_INDESELECT (0x44)
#define PN532_COMMAND_INRELEASE (0x52)
#define PN532_COMMAND_INSELECT (0x54)
#define PN532_COMMAND_INAUTOPOLL (0x60)
#define PN532_COMMAND_TGINITASTARGET (0x8C)
#define PN532_COMMAND_TGSETGENERALBYTES (0x92)
#define PN532_COMMAND_TGGETDATA (0x86)
#define PN532_COMMAND_TGSETDATA (0x8E)
#define PN532_COMMAND_TGSETMETADATA (0x94)
#define PN532_COMMAND_TGGETINITIATORCOMMAND (0x88)
#define PN532_COMMAND_TGRESPONSETOINITIATOR (0x90)
#define PN532_COMMAND_TGGETTARGETSTATUS (0x8A)

#define PN532_RESPONSE_INDATAEXCHANGE (0x41)
#define PN532_RESPONSE_INLISTPASSIVETARGET (0x4B)

#define PN532_WAKEUP (0x55)

#define PN532_SPI_STATREAD (0x02)
#define PN532_SPI_DATAWRITE (0x01)
#define PN532_SPI_DATAREAD (0x03)
#define PN532_SPI_READY (0x01)

#define PN532_I2C_ADDRESS (0x48 >> 1)
#define PN532_I2C_READBIT (0x01)
#define PN532_I2C_BUSY (0x00)
#define PN532_I2C_READY (0x01)
#define PN532_I2C_READYTIMEOUT (20)

#define PN532_MIFARE_ISO14443A (0x00)

// Mifare Commands
#define MIFARE_CMD_AUTH_A (0x60)
#define MIFARE_CMD_AUTH_B (0x61)
#define MIFARE_CMD_READ (0x30)
#define MIFARE_CMD_WRITE (0xA0)
#define MIFARE_CMD_TRANSFER (0xB0)
#define MIFARE_CMD_DECREMENT (0xC0)
#define MIFARE_CMD_INCREMENT (0xC1)
#define MIFARE_CMD_STORE (0xC2)
#define MIFARE_ULTRALIGHT_CMD_WRITE (0xA2)

// Prefixes for NDEF Records (to identify record type)
#define NDEF_URIPREFIX_NONE (0x00)
#define NDEF_URIPREFIX_HTTP_WWWDOT (0x01)
#define NDEF_URIPREFIX_HTTPS_WWWDOT (0x02)
#define NDEF_URIPREFIX_HTTP (0x03)
#define NDEF_URIPREFIX_HTTPS (0x04)
#define NDEF_URIPREFIX_TEL (0x05)
#define NDEF_URIPREFIX_MAILTO (0x06)
#define NDEF_URIPREFIX_FTP_ANONAT (0x07)
#define NDEF_URIPREFIX_FTP_FTPDOT (0x08)
#define NDEF_URIPREFIX_FTPS (0x09)
#define NDEF_URIPREFIX_SFTP (0x0A)
#define NDEF_URIPREFIX_SMB (0x0B)
#define NDEF_URIPREFIX_NFS (0x0C)
#define NDEF_URIPREFIX_FTP (0x0D)
#define NDEF_URIPREFIX_DAV (0x0E)
#define NDEF_URIPREFIX_NEWS (0x0F)
#define NDEF_URIPREFIX_TELNET (0x10)
#define NDEF_URIPREFIX_IMAP (0x11)
#define NDEF_URIPREFIX_RTSP (0x12)
#define NDEF_URIPREFIX_URN (0x13)
#define NDEF_URIPREFIX_POP (0x14)
#define NDEF_URIPREFIX_SIP (0x15)
#define NDEF_URIPREFIX_SIPS (0x16)
#define NDEF_URIPREFIX_TFTP (0x17)
#define NDEF_URIPREFIX_BTSPP (0x18)
#define NDEF_URIPREFIX_BTL2CAP (0x19)
#define NDEF_URIPREFIX_BTGOEP (0x1A)
#define NDEF_URIPREFIX_TCPOBEX (0x1B)
#define NDEF_URIPREFIX_IRDAOBEX (0x1C)
#define NDEF_URIPREFIX_FILE (0x1D)
#define NDEF_URIPREFIX_URN_EPC_ID (0x1E)
#define NDEF_URIPREFIX_URN_EPC_TAG (0x1F)
#define NDEF_URIPREFIX_URN_EPC_PAT (0x20)
#define NDEF_URIPREFIX_URN_EPC_RAW (0x21)
#define NDEF_URIPREFIX_URN_EPC (0x22)
#define NDEF_URIPREFIX_URN_NFC (0x23)

#define PN532_GPIO_VALIDATIONBIT (0x80)
#define PN532_GPIO_P30 (0)
#define PN532_GPIO_P31 (1)
#define PN532_GPIO_P32 (2)
#define PN532_GPIO_P33 (3)
#define PN532_GPIO_P34 (4)
#define PN532_GPIO_P35 (5)

class Adafruit_PN532
{
public:
    Adafruit_PN532(uint8_t irq, uint8_t reset); // Hardware I2C
    void begin(void);
    // Generic PN532 functions
    bool SAMConfig(void);
    uint32_t getFirmwareVersion(void);
    bool sendCommandCheckAck(uint8_t *cmd, uint8_t cmdlen,
                             uint16_t timeout = 100);
    bool writeGPIO(uint8_t pinstate);
    uint8_t readGPIO(void);
    bool setPassiveActivationRetries(uint8_t maxRetries);

    // ISO14443A functions
    bool readPassiveTargetID(
        uint8_t cardbaudrate, uint8_t *uid, uint8_t *uidLength,
        uint16_t timeout = 0); // timeout 0 means no timeout - will block forever.
    bool startPassiveTargetIDDetection(uint8_t cardbaudrate);
    bool readDetectedPassiveTargetID(uint8_t *uid, uint8_t *uidLength);
    bool inDataExchange(uint8_t *send, uint8_t sendLength, uint8_t *response,
                        uint8_t *responseLength);
    bool inListPassiveTarget();
    uint8_t AsTarget();
    uint8_t getDataTarget(uint8_t *cmd, uint8_t *cmdlen);
    uint8_t setDataTarget(uint8_t *cmd, uint8_t cmdlen);

    // Mifare Classic functions
    bool mifareclassic_IsFirstBlock(uint32_t uiBlock);
    bool mifareclassic_IsTrailerBlock(uint32_t uiBlock);
    uint8_t mifareclassic_AuthenticateBlock(uint8_t *uid, uint8_t uidLen,
                                            uint32_t blockNumber,
                                            uint8_t keyNumber, uint8_t *keyData);
    uint8_t mifareclassic_ReadDataBlock(uint8_t blockNumber, uint8_t *data);
    uint8_t mifareclassic_WriteDataBlock(uint8_t blockNumber, uint8_t *data);
    uint8_t mifareclassic_FormatNDEF(void);
    uint8_t mifareclassic_WriteNDEFURI(uint8_t sectorNumber,
                                       uint8_t uriIdentifier, const char *url);

    // Mifare Ultralight functions
    uint8_t mifareultralight_ReadPage(uint8_t page, uint8_t *buffer);
    uint8_t mifareultralight_WritePage(uint8_t page, uint8_t *data);

    // NTAG2xx functions
    uint8_t ntag2xx_ReadPage(uint8_t page, uint8_t *buffer);
    uint8_t ntag2xx_WritePage(uint8_t page, uint8_t *data);
    uint8_t ntag2xx_WriteNDEFURI(uint8_t uriIdentifier, char *url,
                                 uint8_t dataLen);

    // Help functions to display formatted text
    static void PrintHex(const byte *data, const uint32_t numBytes);
    static void PrintHexChar(const byte *pbtData, const uint32_t numBytes);

    typedef void (*readBytesCall_t)(uint8_t devAddress, uint8_t *data, uint16_t len)    ;
    typedef void (*writeBytesCall_t)(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint16_t len) ;

    void setI2CRead(readBytesCall_t cb);
    void setI2CWrite(writeBytesCall_t cb);

private:
    int8_t _irq = -1, _reset = -1;
    int8_t _uid[7];      // ISO14443A uid
    int8_t _uidLen;      // uid len
    int8_t _key[6];      // Mifare Classic key
    int8_t _inListedTag; // Tg number of inlisted tag.

    // Low level communication functions that handle both SPI and I2C.
    void readdata(uint8_t *buff, uint8_t n);
    void writecommand(uint8_t *cmd, uint8_t cmdlen);
    bool isready();
    bool waitready(uint16_t timeout);
    bool readack();

    readBytesCall_t _readCallBack;
    writeBytesCall_t _writeCallBack;
};

#endif
