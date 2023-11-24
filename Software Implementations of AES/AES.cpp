#include <iostream>
#include <string.h>
#include <stdint.h>
#include <chrono>
#include "AES.h"

using namespace std;

void sub_bytes(uint8_t *state){
    for(int i = 0; i < 16; i++){
        state[i] = sbox[state[i]];
    }
}

void inv_sub_bytes(uint8_t *state){
    for(int i = 0; i < 16; i++){
        state[i] = inv_sbox[state[i]];
    }
}

void shift_rows(uint8_t *state){
    uint8_t temp[16];
    memcpy(temp, state, 16);
    for(int i = 0; i < 4; i++){
        state[i*4] = temp[i*4];
        state[i*4+1] = temp[(i*4+5)%16];
        state[i*4+2] = temp[(i*4+10)%16];
        state[i*4+3] = temp[(i*4+15)%16];
    }
}

void inv_shift_rows(uint8_t *state){
    uint8_t temp[16];
    memcpy(temp, state, 16);
    for(int i = 0; i < 4; i++){
        state[i*4] = temp[i*4];
        state[i*4+1] = temp[(i*4+13)%16];
        state[i*4+2] = temp[(i*4+10)%16];
        state[i*4+3] = temp[(i*4+7)%16];
    }
}

void mix_columns(uint8_t *state){
    uint8_t temp[16];
    memcpy(temp, state, 16);
    for(int i = 0; i < 4; i++, state+=4){
        temp[0]=L_box[1][state[0]]^L_box[2][state[1]]^state[2]^state[3];
        temp[1]=state[0]^L_box[1][state[1]]^L_box[2][state[2]]^state[3];
        temp[2]=state[0]^state[1]^L_box[1][state[2]]^L_box[2][state[3]];
        temp[3]=L_box[2][state[0]]^state[1]^state[2]^L_box[1][state[3]];

        state[0]=temp[0];
        state[1]=temp[1];
        state[2]=temp[2];
        state[3]=temp[3];
    }
}

void inv_mix_columns(uint8_t *state){
    uint8_t temp[16];
    memcpy(temp, state, 16);
    for(int i = 0; i < 4; i++, state+=4){
        temp[0]=L_box[13][state[0]]^L_box[10][state[1]]^L_box[12][state[2]]^L_box[8][state[3]];
        temp[1]=L_box[8][state[0]]^L_box[13][state[1]]^L_box[10][state[2]]^L_box[12][state[3]];
        temp[2]=L_box[12][state[0]]^L_box[8][state[1]]^L_box[13][state[2]]^L_box[10][state[3]];
        temp[3]=L_box[10][state[0]]^L_box[12][state[1]]^L_box[8][state[2]]^L_box[13][state[3]];

        state[0]=temp[0];
        state[1]=temp[1];
        state[2]=temp[2];
        state[3]=temp[3];
    }
}


void add_round_key(uint8_t *state, uint8_t *round_key){
    for(int i = 0; i < 16; i++){
        state[i] ^= round_key[i];
    }

}

void expand_key(const uint8_t *key, uint8_t round_key[][16]) {
    memcpy(round_key[0], key, 16); // 将初始密钥复制到 round_key 的第一行

    for (int round = 1; round <= 10; round++) {
        uint8_t temp[4];
        memcpy(temp, round_key[round - 1] + 12, 4);
        uint8_t t = temp[0];
        for (int i = 0; i < 3; i++) {
            temp[i] = sbox[temp[(i + 1) % 4]];
        }
        temp[3] = sbox[t];
        temp[0] ^= rcon[round-1];
        for (int i = 0; i < 4; i++) {
            temp[i] ^= round_key[round - 1][i];
        }
        memcpy(round_key[round], temp, 4);
        for (int i = 1; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                round_key[round][4 * i + j] = round_key[round - 1][4 * i + j] ^ round_key[round][4 * (i - 1) + j];
            }
        }
    }
}

void encrypt(uint8_t *plain, uint8_t *key, uint8_t *cipher){
    uint8_t round_key[11][16];
    uint8_t state[16];
    uint8_t temp[4];

    memcpy(state, plain, 16);

    expand_key(key, round_key);
    add_round_key(state, round_key[0]);

    for(int i = 0; i < 9; i++){
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, round_key[i+1]);
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, round_key[10]);

    memcpy(cipher, state, 16);
}

void decrypt(uint8_t *cipher, uint8_t *key, uint8_t *plain){
    uint8_t round_key[11][16];
    uint8_t state[16];
    uint8_t temp[4];

    memcpy(state, cipher, 16);

    expand_key(key, round_key);
    add_round_key(state, round_key[10]);

    for(int i = 0; i < 9; i++){
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, round_key[9-i]);
        inv_mix_columns(state);
    }

    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, round_key[0]);

    memcpy(plain, state, 16);
}

void printhex(string s, uint8_t *a){
    cout<<s;
    for(int i = 0; i < 16; i++){
        printf("%02x ", a[i]);
    }
    cout<<endl;
}

int main() {
    cout << "----------------128bit AES 加密----------------" << endl;
    uint8_t key[16] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    uint8_t plain[16] = {
        0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34
    };
    uint8_t cipher[16];

    printhex("明文：", plain);

    constexpr int numIterations = 1000000; // 循环执行的次数

    auto start = chrono::high_resolution_clock::now(); // 记录开始时间

    for (int i = 0; i < numIterations; ++i) {
        encrypt(plain, key, cipher);
    }

    auto end = chrono::high_resolution_clock::now(); // 记录结束时间

    printhex("密文：", cipher);
    decrypt(cipher, key, plain);
    printhex("解密：", plain);

    // 计算执行时间（单位：毫秒）
    chrono::duration<double, milli> duration = end - start;

    // 计算每秒加密的数据量（单位：MB/s）
    double dataProcessed = sizeof(plain) * numIterations;
    double timeInSeconds = duration.count() / 1000.0;
    double dataRate = dataProcessed / (1024.0 * 1024.0 * timeInSeconds);
    cout << "每秒加密的数据量：" << dataRate << " MB/s" << endl;

    return 0;
}
