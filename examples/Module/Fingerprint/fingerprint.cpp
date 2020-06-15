#include "fingerprint.h"

/*起始帧*/
#define GR_PACK_HEAD 0XEF01
/* 包标识*/
#define GR_PACK_COMMAND_FLAG 0X01
//当为2时，表示后续还有包
#define GR_PACK_DATA_FALG 0X02
#define GR_PACK_ACK_FALG 0X07
#define GR_PACK_END_FALG 0X08
// 包长度 = 数据到校验和之间的长度
// 校验和 = 包标识 ~ 校验和之间的和 ，超两个字节对高位进行忽略

Fingerprint::Fingerprint(Stream *UART, uint32_t ADDRRESS)
{
    _uart = UART;
    _addr = ADDRRESS;
    _headBuffer[0] = 0xEF;
    _headBuffer[1] = 0x01;
    memcpy(&_headBuffer[2], &_addr, sizeof(_addr));
};

Fingerprint::~Fingerprint(void)
{

};

/*********************
 *  0   1   2   3   4   5   6       7       8      9      10     11
 *  ef  01  ff  ff  ff  ff  flag   len_h    len_l  code   sum_h  sum_l
 * *******************/
bool Fingerprint::validSum(uint8_t *resp, uint8_t len)
{
    uint16_t sum = 0;
    for (int i = 6; i < len - 2; i++) {
        sum += resp[i];
    }
    // Serial.printf("sum:%x  -2:%x -1:%x \n", sum, resp[len - 2], resp[len - 1]);
    // Serial.printf("merge: %x \n", (resp[len - 2] << 8 | resp[len - 1]));
    return sum == (resp[len - 2] << 8 | resp[len - 1]);
}

bool Fingerprint::waitForAck(uint8_t *reqs, uint8_t len)
{
    uint32_t i = 0, timerStart = millis();
    while (1) {
        if (_uart->available()) {
            reqs[i++] = _uart->read();
            Serial.print(reqs[i - 1], HEX);
            Serial.print(" ");
            if (i >= len) {
                Serial.println();
                break;
            }
        }
        if (millis() - timerStart > 1000) {
            Serial.printf("time out\n");
            return false;
        }
    }
    return true;
}

int Fingerprint::waitForResp(uint8_t respSize, uint32_t timeout)
{
    int i = 0;
    uint32_t timerStart = millis();
    uint8_t response[respSize];
    while (1) {
        if (_uart->available()) {
            response[i++] = _uart->read();
            if (i >= sizeof(response))
                break;
        }
        if (millis() - timerStart > 1000 * timeout) {
            return -1;
        }
    }
    if (validSum(response, sizeof(response))) {
        return response[9];
    }
    return -1;
}

void Fingerprint::__sendCom(uint8_t command, uint16_t len)
{
    uint16_t sum = 0;
    uint8_t commandBuffer[6];
    commandBuffer[0] = GR_PACK_COMMAND_FLAG;
    commandBuffer[1] = (len & 0xFF00) >> 8;
    commandBuffer[2] = len & 0xFF;
    commandBuffer[3] = command;

    for (int i = 0; i < sizeof(commandBuffer) - 2; i++) {
        sum += commandBuffer[i];
    }
    commandBuffer[4] = (sum & 0xFF00) >> 8;
    commandBuffer[5] = sum & 0xFF;
    _uart->write(_headBuffer, sizeof(_headBuffer));
    _uart->write(commandBuffer, sizeof(commandBuffer));
}

/**
 * @brief  录入指纹到ImageBuffer
 * @note   标准应答格式 12Bytes
 * @retval 0H ~ 03H
 */
bool Fingerprint::entryFinger(void)
{
    int ret = GR_OK;

    __sendCom(GR_GET_IMAGE, 0x03);

    ret = waitForResp(12, 15);
    if (ret != GR_OK) {
        getErrorReason(ret);
    }
    return ret == GR_OK;
}

void Fingerprint::getErrorReason(int err)
{
    // Serial.printf("Error :[%d]\n", err);
    switch (err) {
    case 0:
        break;
    }
}

/**
 * @brief  生成特征标识
 * @note   标准应答格式 12Bytes
 * @param  id: 01H ~ 02H
 * @retval
 */
bool Fingerprint::generateFeature(GR_BufID_t id)
{
    int ret = GR_OK;
    uint8_t commandBuffer[7];
    _uart->write(_headBuffer, 6);
    commandBuffer[0] = GR_PACK_COMMAND_FLAG;
    commandBuffer[1] = 0x00;
    commandBuffer[2] = 0x04;
    commandBuffer[3] = GR_GET_CHAR;
    commandBuffer[4] = id;
    commandBuffer[5] = 0x00;
    commandBuffer[6] = 0x07 + id;
    _uart->write(commandBuffer, sizeof(commandBuffer));
    ret = waitForResp(12, 1);
    if (ret != GR_OK) {
        getErrorReason(ret);
    }
    return ret == GR_OK;
}

/**
 * @brief  精确比对两枚指纹特征
 * @note   //! 非标准应答格式 14Bytes
 * @retval 0x00  0x01 0x08
 */
bool Fingerprint::matchFingerprint(void)
{
    int ret = GR_OK;

    __sendCom(GR_MATCH, 0x03);
    ret = waitForResp(14, 1);
    if (ret != GR_OK) {
        getErrorReason(ret);
    }
    return ret == GR_OK;
}

/**
 * @brief  搜索指纹
 * @note   //! 非标准应答格式 16Bytes
 * @retval 确认字，页码（相配指纹模板）
 */
bool Fingerprint::searchFingerprint(GR_BufID_t id, uint16_t sPage, uint16_t pNum)
{
    int ret = GR_OK;
    uint32_t sum = 0;
    uint8_t commandBuffer[11];
    _uart->write(_headBuffer, 6);
    commandBuffer[0] = GR_PACK_COMMAND_FLAG;
    commandBuffer[1] = 0x00;
    commandBuffer[2] = 0x08;
    commandBuffer[3] = GR_SEARCH;
    commandBuffer[4] = id;
    commandBuffer[5] = (sPage & 0xFF00) >> 8;
    commandBuffer[6] = sPage & 0xFF;
    commandBuffer[7] = (pNum & 0xFF00) >> 8;
    commandBuffer[8] = pNum & 0xFF;

    for (int i = 0; i < sizeof(commandBuffer) - 2; i++) {
        sum += commandBuffer[i];
    }
    commandBuffer[9] = (sum & 0x00FF00) >> 8;
    commandBuffer[10] = sum & 0xFF;

    _uart->write(commandBuffer, sizeof(commandBuffer));
    ret = waitForResp(16, 1);
    if (ret != GR_OK) {
        getErrorReason(ret);
    }
    return ret == GR_OK;
}

/**
 * @brief  合并特征(生成模板)
 * @note   标准应答格式 12Bytes
 * @retval 0x00 0x01 0x0A
 */
bool Fingerprint::mergeFeature(void)
{
    int ret = GR_OK;
    uint8_t commandBuffer[6];
    _uart->write(_headBuffer, 6);
    commandBuffer[0] = GR_PACK_COMMAND_FLAG;
    commandBuffer[1] = 0x00;
    commandBuffer[2] = 0x03;
    commandBuffer[3] = GR_REG_MODEL;
    commandBuffer[4] = 0x00;
    commandBuffer[5] = 0x09;
    _uart->write(commandBuffer, sizeof(commandBuffer));
    ret = waitForResp(12, 1);
    if (ret != GR_OK) {
        getErrorReason(ret);
    }
    return ret == GR_OK;
}

/**
 * @brief  储存模板
 * @note   标准应答格式 12Bytes
 * @retval
 */
bool Fingerprint::storeFeature(GR_BufID_t id, uint16_t posNum)
{
    int ret = GR_OK;
    uint32_t sum = 0;
    uint8_t commandBuffer[9];
    _uart->write(_headBuffer, 6);
    commandBuffer[0] = GR_PACK_COMMAND_FLAG;
    commandBuffer[1] = 0x00;
    commandBuffer[2] = 0x06;
    commandBuffer[3] = GR_STORE_CHAR;
    commandBuffer[4] = id;
    commandBuffer[5] = (posNum & 0xFF00) >> 8;
    commandBuffer[6] = posNum & 0xFF;
    for (int i = 0; i < sizeof(commandBuffer) - 2; i++) {
        sum += commandBuffer[i];
    }
    commandBuffer[7] = (sum & 0x00FF00) >> 8;
    commandBuffer[8] = sum & 0xFF;

    _uart->write(commandBuffer, sizeof(commandBuffer));
    ret = waitForResp(12, 1);
    if (ret != GR_OK) {
        getErrorReason(ret);
    }
    return ret == GR_OK;
}

/**
 * @brief  读出模板
 * @note   标准应答格式 12Bytes,将flash数据库中指定ID号的指纹模板读入到模板缓冲区CharBuffer1或CharBuffer2
 * @retval
 */
bool Fingerprint::loadFeature(GR_BufID_t id, uint16_t pNum)
{
    int ret = GR_OK;
    uint32_t sum = 0;
    uint8_t commandBuffer[9];
    _uart->write(_headBuffer, 6);
    commandBuffer[0] = GR_PACK_COMMAND_FLAG;
    commandBuffer[1] = 0x00;
    commandBuffer[2] = 0x06;
    commandBuffer[3] = GR_LOAD_CHAR;
    commandBuffer[4] = id;
    commandBuffer[5] = (pNum & 0xFF00) >> 8;
    commandBuffer[6] = pNum & 0xFF;
    for (int i = 0; i < sizeof(commandBuffer) - 2; i++) {
        sum += commandBuffer[i];
    }
    commandBuffer[7] = (sum & 0x00FF00) >> 8;
    commandBuffer[8] = sum & 0xFF;

    _uart->write(commandBuffer, sizeof(commandBuffer));
    ret = waitForResp(12, 1);
    if (ret != GR_OK) {
        getErrorReason(ret);
    }
    return ret == GR_OK;
}

/**
 * @brief  上传特征或模板
 * @note   标准应答格式 12Bytes
 * @retval
 */
bool Fingerprint::uplaodFeature(GR_BufID_t id)
{
    int ret = GR_OK;
    uint32_t sum = 0;
    uint8_t commandBuffer[7];
    _uart->write(_headBuffer, 6);
    commandBuffer[0] = GR_PACK_COMMAND_FLAG;
    commandBuffer[1] = 0x00;
    commandBuffer[2] = 0x04;
    commandBuffer[3] = GR_UP_CHAR;
    commandBuffer[4] = id;
    for (int i = 0; i < sizeof(commandBuffer) - 2; i++) {
        sum += commandBuffer[i];
    }
    commandBuffer[5] = (sum & 0x00FF00) >> 8;
    commandBuffer[6] = sum & 0xFF;

    _uart->write(commandBuffer, sizeof(commandBuffer));
    ret = waitForResp(12, 1);
    if (ret != GR_OK) {
        getErrorReason(ret);
    }
    return ret == GR_OK;
}

/**
 * @brief  下载特征或模板
 * @note   标准应答格式 12Bytes
 * @retval
 */
bool Fingerprint::downloadFeature(GR_BufID_t id)
{
    return true;
}

// /**
//  * @brief  上传原始图像
//  * @note   标准应答格式 12Bytes
//  * @retval
//  */
// bool Fingerprint::uploadImage(void)
// {
// }

// /**
//  * @brief  下载原始图像
//  * @note   标准应答格式 12Bytes
//  * @retval
//  */
// bool Fingerprint::downloadImage(void)
// {
// }

// /**
//  * @brief  删除模板
//  * @note   标准应答格式 12Bytes
//  * @retval
//  */
// bool Fingerprint::uploadImage(void)
// {
// }

// /**
//  * @brief  清空指纹库
//  * @note   标准应答格式 12Bytes
//  * @retval
//  */
// bool Fingerprint::deleteLibarry(void)
// {
// }

// /**
//  * @brief  写系统寄存器
//  * @note   标准应答格式 12Bytes
//  * @retval
//  */
// bool Fingerprint::writeSysReg(void)
// {
// }

/**
 * @brief  读系统基本参数
 * @note   //! 非标准应答格式 28Bytes
 * @retval
 */
bool Fingerprint::readSysParams(SysParams_t *params)
{
    uint8_t response[28];
    const uint8_t order[] = {0x01, 0x00, 0x03, 0x0F, 0x00, 0x13};
    _uart->write(_headBuffer, sizeof(_headBuffer));
    _uart->write(order, sizeof(order));
    if (waitForAck(response, sizeof(response))) {
        if (validSum(response, sizeof(response)) && response[9] == GR_OK) {
            Serial.printf("\nvalid pass\n\n");
            params->statusReg = (response[10] << 8) | response[11];
            params->code = response[12] << 8 | response[13];
            params->libSize = response[14] << 8 | response[15];
            params->level = response[16] << 8 | response[17];
            params->address = response[18] << 24 | response[19] << 16 | response[20] << 8 | response[21];
            params->packSize = response[22] << 8 | response[23];
            params->bps = response[24] << 8 | response[25];
            return true;
        }
    }
    return false;
}


bool Fingerprint::readINFpage()
{
    uint8_t response[255];
    const uint8_t order[] = {0x01, 0x00, 0x03, 0x16, 0x00, 0x19};
    _uart->write(_headBuffer, sizeof(_headBuffer));
    _uart->write(order, sizeof(order));
    if (waitForAck(response, sizeof(response))) {
        if (validSum(response, sizeof(response)) && response[9] == GR_OK) {
            Serial.printf("\nvalid pass\n\n");
            return true;
        }
    }
    return false;

}



// /**
//  * @brief  设置口令
//  * @note   //! 非标准应答格式 11Bytes
//  * @retval
//  */
// bool Fingerprint::setPassWord(void)
// {
// }

// /**
//  * @brief  验证口令
//  * @note   标准应答格式 12Bytes
//  * @retval
//  */
// bool Fingerprint::setPassWord(void)
// {
// }

// /**
//  * @brief  采样随机数
//  * @note   //! 非标准应答格式 16Bytes
//  * @retval
//  */
// bool Fingerprint::getRandom(void)
// {
// }

// /**
//  * @brief  设置模块地址
//  * @note   标准应答格式 12Bytes
//  * @retval
//  */
// bool Fingerprint::setAddress(void)
// {
// }

// /**
//  * @brief  端口控制
//  * @note   标准应答格式 12Bytes
//  * @retval
//  */
// bool Fingerprint::setAddress(void)
// {
// }

// /**
//  * @brief  写记事本
//  * @note   标准应答格式 12Bytes
//  * @retval
//  */
// bool Fingerprint::writeNotepad(void)
// {
// }

// /**
//  * @brief  读记事本
//  * @note   //! 非标准应答格式 44Bytes
//  * @retval
//  */
// bool Fingerprint::readNotepad(void)
// {
// }

// /**
//  * @brief  生成细化指纹图像
//  * @note   标准应答格式 12Bytes
//  * @retval
//  */
// bool Fingerprint::generateBinImage(void)
// {
// }

// /**
//  * @brief  读有效模板个数
//  * @note   //! 标准应答格式 14Bytes
//  * @retval
//  */
// bool Fingerprint::generateBinImage(void)
// {
// }

// /**
//  * @brief  读索引表
//  * @note   //! 非标准应答格式 44Bytes
//  * @retval
//  */
// bool Fingerprint::readNotepad(void)
// {
// }