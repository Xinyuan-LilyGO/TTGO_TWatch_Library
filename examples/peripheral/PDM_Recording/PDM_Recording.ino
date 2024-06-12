/**
 * @file      PDM_Recording.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-04-23
 *
 */

#include <LilyGoLib.h>
#include <LV_Helper.h>

#include <AudioOutputI2S.h>
#include <AudioFileSourceFATFS.h>
#include "AudioGeneratorWAV.h"
#include "AudioFileSourceFunction.h"


AudioGeneratorWAV       *wav;
AudioFileSourceFATFS   *file;
AudioOutputI2S          *out;

void microphone_record(const char *song_name, uint32_t duration);

void setup(void)
{
    Serial.begin(115200);

    watch.begin();

    // Initialize onboard PDM microphone
    watch.initMicrophone();

    beginLvglHelper();

    lv_obj_t  *label1 = lv_label_create(lv_scr_act());
    lv_label_set_text(label1, "Recording...");
    lv_obj_center(label1);
    lv_task_handler();

    Serial.println("Recording...");
    microphone_record("/rec.wav", 8);

    out = new AudioOutputI2S(1, AudioOutputI2S::EXTERNAL_I2S);
    out->SetPinout(BOARD_DAC_IIS_BCK, BOARD_DAC_IIS_WS, BOARD_DAC_IIS_DOUT);
    file = new AudioFileSourceFATFS("/rec.wav");
    wav = new AudioGeneratorWAV();
    wav->begin(file, out);

    lv_label_set_text(label1, "Play record");
    lv_task_handler();
}

void loop()
{
    if (wav->isRunning()) {
        if (!wav->loop()) {
            wav->stop();
        }
    } else {
        Serial.printf("wav done\n");
        delay(1000);
        file->open("/rec.wav");
        wav->begin(file, out);
    }
}



// Create a file and add wav header to it so we can play it from PC later
bool create_wav_file(const char *song_name, uint32_t duration, uint16_t num_channels, const uint32_t sampling_rate, uint16_t bits_per_sample)
{
    // data size in bytes - > this amount of data should be recorded from microphone later
    uint32_t data_size = sampling_rate * num_channels * bits_per_sample * duration / 8;

    File new_audio_file = FFat.open(song_name, FILE_WRITE);
    if (new_audio_file == NULL) {
        Serial.println("Failed to create file");
        return false;
    }

    /* *************** ADD ".WAV" HEADER *************** */
    uint8_t CHUNK_ID[4] = {'R', 'I', 'F', 'F'};
    new_audio_file.write(CHUNK_ID, 4);

    uint32_t chunk_size = data_size + 36;
    uint8_t CHUNK_SIZE[4] = {chunk_size, chunk_size >> 8, chunk_size >> 16, chunk_size >> 24};
    new_audio_file.write(CHUNK_SIZE, 4);

    uint8_t FORMAT[4] = {'W', 'A', 'V', 'E'};
    new_audio_file.write(FORMAT, 4);

    uint8_t SUBCHUNK_1_ID[4] = {'f', 'm', 't', ' '};
    new_audio_file.write(SUBCHUNK_1_ID, 4);

    uint8_t SUBCHUNK_1_SIZE[4] = {0x10, 0x00, 0x00, 0x00};
    new_audio_file.write(SUBCHUNK_1_SIZE, 4);

    uint8_t AUDIO_FORMAT[2] = {0x01, 0x00};
    new_audio_file.write(AUDIO_FORMAT, 2);

    uint8_t NUM_CHANNELS[2] = {num_channels, num_channels >> 8};
    new_audio_file.write(NUM_CHANNELS, 2);

    uint8_t SAMPLING_RATE[4] = {sampling_rate, sampling_rate >> 8, sampling_rate >> 16, sampling_rate >> 24};
    new_audio_file.write(SAMPLING_RATE, 4);

    uint32_t byte_rate = num_channels * sampling_rate * bits_per_sample / 8;
    uint8_t BYTE_RATE[4] = {byte_rate, byte_rate >> 8, byte_rate >> 16, byte_rate >> 24};
    new_audio_file.write(BYTE_RATE, 4);

    uint16_t block_align = num_channels * bits_per_sample / 8;
    uint8_t BLOCK_ALIGN[2] = {block_align, block_align >> 8};
    new_audio_file.write(BLOCK_ALIGN, 2);

    uint8_t BITS_PER_SAMPLE[2] = {bits_per_sample, bits_per_sample >> 8};
    new_audio_file.write(BITS_PER_SAMPLE, 2);

    uint8_t SUBCHUNK_2_ID[4] = {'d', 'a', 't', 'a'};
    new_audio_file.write(SUBCHUNK_2_ID, 4);

    uint8_t SUBCHUNK_2_SIZE[4] = {data_size, data_size >> 8, data_size >> 16, data_size >> 24};
    new_audio_file.write(SUBCHUNK_2_SIZE, 4);

    // Actual data should be appended after this point later

    new_audio_file.close();
    return true;
}


void microphone_record(const char *song_name, uint32_t duration)
{
    // Add wav header to the file so we can play it from PC later
    if (!create_wav_file(song_name, duration, 1, MIC_I2S_SAMPLE_RATE, MIC_I2S_BITS_PER_SAMPLE)) {
        Serial.println("Error during wav header creation");
        return;
    }

    Serial.println("Create wav file succssed!");

    // Buffer to receive data from microphone
    const size_t BUFFER_SIZE = 500;
    uint8_t *buf = (uint8_t *)malloc(BUFFER_SIZE);
    if (!buf) {
        Serial.println("Failed to alloc memory");
        return;
    }

    // Open created .wav file in append+binary mode to add PCM data
    File audio_file = FFat.open(song_name, FILE_APPEND);
    if (!audio_file) {
        Serial.println("Failed to create file");
        return;
    }

    // data size in bytes - > this amount of data should be recorded from microphone
    uint32_t data_size = MIC_I2S_SAMPLE_RATE * MIC_I2S_BITS_PER_SAMPLE * duration / 8;

    // Record until "file_size" bytes have been read from mic.
    uint32_t counter = 0;
    uint32_t bytes_written;
    Serial.println("Recording started");
    while (counter != data_size) {
        // Check for file size overflow
        if (counter > data_size) {
            Serial.println("File is corrupted. data_size must be multiple of BUFFER_SIZE. Please modify BUFFER_SIZE");
            break;
        }

        // Read data from microphone
        if (!watch.readMicrophone(buf, BUFFER_SIZE, &bytes_written)) {
            Serial.println("i2s_read() error");
        }

        if (bytes_written != BUFFER_SIZE) {
            Serial.println("Bytes written error");
        }

        // Save data to SD card
        audio_file.write( buf, BUFFER_SIZE);

        // Increment the counter
        counter += BUFFER_SIZE;
    }
    Serial.println("Recording finished");

    audio_file.close();
    free(buf);

}
