#pragma once

#include <Arduino.h>

typedef enum {
    GR_GET_IMAGE = 0X01,
    GR_GET_CHAR = 0X02,
    GR_MATCH = 0X03,
    GR_SEARCH = 0X04,
    GR_REG_MODEL = 0X05,
    GR_STORE_CHAR = 0X06,
    GR_LOAD_CHAR = 0X07,
    GR_UP_CHAR = 0X08,
    GR_DOWN_CHAR = 0X09,
    GR_UP_IMAGE = 0X0A,
    GR_DOWN_IMAGE = 0X0B,
    GR_DEL_CHAR = 0X0C,
    GR_EMPTY = 0X0D,
    GR_WRITE_REG = 0X0E,
    GR_READ_STATE = 0X0F,
    GR_SET_PASSWORD = 0X12,
    GR_VALID_PASSWORD = 0X13,
    GR_GET_RANDOM = 0X14,
    GR_SET_ADDRESS = 0X15,
    GR_PORT_CONTROL = 0X17,
    GR_WRITE_NOTEPAD = 0X18,
    GR_READ_NOTEPAD = 0X19,
    GR_GEN_BIN_IMAGE = 0X1C,
    GR_VALID_TEMPLETE_NUM = 0X1D,
    GR_READ_INDEX_TABLE = 0X1F
} GR_ORDER;

typedef enum {
    GR_CHAR_BUFFER_1 = 0x01,
    GR_CHAR_BUFFER_2 = 0x02
} GR_BufID_t;

enum {
    GR_OK = 0X00,
    PACK_ERROR = 0X01,
    GR_NO_FINGERPRINT = 0X02,
    GR_ENTERY_FAIL = 0X03,
    GR_IMAGE_FAIL_1 = 0X04,
    GR_IMAGE_FAIL_2 = 0X05,
    GR_IMAGE_FAIL_3 = 0X06,
    GR_IMAGE_FAIL_4 = 0X07,
    GR_IMAGE_NO_MATCH = 0X08,
    GR_IMAGE_SEARCH_FAIL = 0X09,
    GR_MERGE_FAIL = 0X0A,

    GR_REFUSE_PACK = 0X0E,
    GR_IMAGE_UP_FAIL = 0X0F,
    GR_IMAGE_DEL_FAIL = 0X10,
    GR_DATABASE_DEL_FAIL = 0X11,
    GR_ORDER_NO_MATCH = 0X12,
    GR_BUFFER_EMPTY = 0X13,

    GR_RW_FLASH_FAIL = 0X18,
    GR_ERROR_OTHER,
    GR_INVALID_REG,
    GR_REG_NUM_ERROR,
    GR_NOTEBOOK_ERROR,
    GR_PORT_FAIL,
    GR_REGISTER_FAIL,
    GR_DATABASE_FULL,
};

//数据包大小 1word
//波特率 1word  N * 9600
//模块口令 2word default<0>
// 默 认 波 特 率 为 57600bps
#define MODLUES_ADDRESS 0xffffffff //2WORD
#define STATUS_REG 0               //1WORD
#define SECURITY_LEVEL             //1WORD

/*设备地址*/
#define GR_MD_ADDRESS 0XFFFFFFFF

typedef struct {
    uint16_t statusReg;
    uint16_t code;
    uint16_t libSize;
    uint16_t level;
    uint32_t address;
    uint16_t packSize;
    uint16_t bps;
} SysParams_t;

class Fingerprint
{
public:
    Fingerprint(Stream *UART, uint32_t ADDRRESS = GR_MD_ADDRESS);
    ~Fingerprint(void);
    bool entryFinger(void);
    bool readSysParams(SysParams_t *params);
    bool generateFeature(GR_BufID_t id);
    bool mergeFeature(void);
    bool storeFeature(GR_BufID_t id, uint16_t posNum);
    bool searchFingerprint(GR_BufID_t id, uint16_t sPage, uint16_t pNum);
    bool readINFpage();
private:
    bool waitForAck(uint8_t *reqs, uint8_t len);
    int waitForResp(uint32_t timeout);
    bool validSum(uint8_t *resp, uint8_t len);
    int waitForResp(uint8_t respSize, uint32_t timeout);
    void __sendCom(uint8_t command, uint16_t len);
    void getErrorReason(int err);

    bool matchFingerprint(void);
    bool loadFeature(GR_BufID_t id, uint16_t pNum);
    bool uplaodFeature(GR_BufID_t id);
    bool downloadFeature(GR_BufID_t id);
    // bool uploadImage(void);
    // bool downloadImage(void);
    // bool uploadImage(void);
    // bool deleteLibarry(void);
    // bool writeSysReg(void);
    // bool setPassWord(void);
    // bool setPassWord(void);
    // bool getRandom(void);
    // bool setAddress(void);
    // bool setAddress(void);
    // bool writeNotepad(void);
    // bool readNotepad(void);
    // bool generateBinImage(void);
    // bool generateBinImage(void);
    // bool readNotepad(void);

    bool isPowerUp;
    Stream *_uart;
    uint32_t _addr;
    uint8_t _headBuffer[6];

protected:
};