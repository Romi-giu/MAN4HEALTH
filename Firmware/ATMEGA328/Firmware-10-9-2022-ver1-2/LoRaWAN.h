// ------------------------------------------------------------------------------------------------------------------
// Aliases
// ------------------------------------------------------------------------------------------------------------------
#define DIO0 3
#define NSS  10
#define TEMP A2
// ------------------------------------------------------------------------------------------------------------------
// Constantes
// ------------------------------------------------------------------------------------------------------------------
static const unsigned char PROGMEM S_Table[16][16] = {
  {0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76},
  {0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0},
  {0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15},
  {0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75},
  {0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84},
  {0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF},
  {0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8},
  {0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2},
  {0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73},
  {0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x0B,0xDB},
  {0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79},
  {0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08},
  {0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A},
  {0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E},
  {0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF},
  {0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16}
};
static const byte RF0x06[]={0xD9,0xD9,0xD9,0xD8,0xD8,0xD8,0xD8,0xD8};
static const byte RF0x07[]={0x06,0x13,0x20,0xC6,0xD3,0xE0,0xEC,0xF9};
static const byte RF0x08[]={0x8B,0x58,0x24,0x8B,0x58,0x24,0xF1,0xBE};
// ------------------------------------------------------------------------------------------------------------------
// Variáveis
// ------------------------------------------------------------------------------------------------------------------
unsigned char NwkSkey[16] = {0xC1, 0xFD, 0x07, 0x5A, 0x39, 0x99, 0x70, 0x86, 0x17, 0x78, 0xCB, 0xD0, 0x37, 0xFC, 0x75, 0x0B};
unsigned char AppSkey[16] = {0x83, 0x62, 0xC5, 0xF0, 0xDA, 0x8A, 0xB1, 0xE0, 0x42, 0xE1, 0x9A, 0xAD, 0x0D, 0x8B, 0x38, 0xBF};
unsigned char DevAddr[4] =  {0xA1, 0x44, 0xE7, 0xC9};

unsigned int Frame_Counter_Tx = 0x0000;

// Variáveis utilizadas na definição aleatória da portador
int arraylist[]={0,1,2,3,4,5,6,7};
int maxval = 8;

// Registos para alterar o SF, BW e CR
//
//  Registo 0x1E
// |   SF      |x |CC|  x
// |b7|bc|b5|b4|c0|d0|e1|e0
//
// Spreading factor (SF)
// 0110 - 6
// 0111 - 7
// 1000 - 8
// 1001 - 9
// 1010 - 10 
// 1011 - 11
// 1100 - 12
//
// CRC
// 0 - Header indicates CRC off
// 1 - Header indicates CRC on
//
//  Registo 0x1D
// |   BW      |  CR    |Hr
// |b2|b2|b1|b0|c2|c1|c0|d0
//
// Valores de BW com interesse prático: 125 kHz ou 250 kHz (apenas para 868.3) 
// 0111 - 125 kHz
// 1000 - 250 kHz
//
// Coding Rate (normalmente 4/5)
// 001 - 4/5
// 010 - 4/6
// 011 - 4/7
// 100 - 4/8
//
// Header type
// 0 - Explicit header
// 1 - Implicit header
//
// 0111 0010 -> 0x72
//
// Registo 0x26
//| xxx       |MN|G | xx
// b3|b2|b1|b0|c0|d0|e1|e0|
// 
// MN - Mobile node
//  0 - Static node
//  1 - Mobile node
//
//  G - LNA gain
//  0 - Set by reg LnaGain
//  1 - Set by iunternal AGC loop
// 0000 0100 - > 4

static const byte RF0x1E = 0x74; //SF7 CRC On
static const byte RF0x1D = 0x72; //125 kHz 4/5 coding rate explicit header mode
static const byte RF0x26 = 0x04; //Low datarate | optimization off | AGC auto on

// ------------------------------------------------------------------------------------------------------------------
// Protótipos
// ------------------------------------------------------------------------------------------------------------------
void LORA_Send_Data(unsigned char *Data, unsigned char Data_Length, unsigned int Frame_Counter_Tx);
void RFM_Write(unsigned char RFM_Address, unsigned char RFM_Data);
void RFM_Init();
void RFM_Send_Package(unsigned char *RFM_Tx_Package, unsigned char Package_Length);
void Encrypt_Payload(unsigned char *Data, unsigned char Data_Length, unsigned int Frame_Counter, unsigned char Direction);
void Calculate_MIC(unsigned char *Data, unsigned char *Final_MIC, unsigned char Data_Length, unsigned int Frame_Counter, unsigned char Direction);
void Generate_Keys(unsigned char *K1, unsigned char *K2);
void Shift_Left(unsigned char *Data);
void XOR(unsigned char *New_Data,unsigned char *Old_Data);
void AES_Encrypt(unsigned char *Data, unsigned char *Key);
void AES_Add_Round_Key(unsigned char *Round_Key, unsigned char (*State)[4]);
unsigned char AES_Sub_Byte(unsigned char Byte);
void AES_Shift_Rows(unsigned char (*State)[4]);
void AES_Mix_Collums(unsigned char (*State)[4]);
void AES_Calculate_Round_Key(unsigned char Round, unsigned char *Round_Key);
