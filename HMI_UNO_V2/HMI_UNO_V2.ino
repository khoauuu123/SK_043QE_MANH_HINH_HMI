// #include <SoftwareSerial.h>

// SoftwareSerial mod(3, 2);  // RX, TX (thay đổi chân nếu cần)

//HardwareSerial mod(6, 5);  // RX, TX (thay đổi chân nếu cần)
HardwareSerial mod(PC11, PC10);

bool DEBUG = 0;
byte Dem_debug = 0;

#define SLAVE_ID 0x01  // Địa chỉ slave
#define FC_READ_COILS 0x01
#define FC_READ_WRITE_SINGLE_COILS 0x05
#define FC_READ_HOLDING_REGISTERS 0x03
#define FC_WRITE_SINGLE_REGISTER 0x06

const byte TONGSO_COI = 16;
bool coils[TONGSO_COI];
// coils[0] MANUAL/AUTO

// coils[1] ON/OFF PHASE1
// coils[2] ON/OFF PHASE2
// coils[3] ON/OFF PHASE3

// coils[7] ONLINE/OFFLINE 4G

const byte TONGSO_TS = 67;
uint16_t holdingRegs[TONGSO_TS];
//holdingRegs[1] = DÒNG ĐIỆN PHA1
//holdingRegs[2] = ĐIỆN ÁP PHA1
//holdingRegs[3] = CÔNG SUẤT PHA1

//holdingRegs[4] = DÒNG ĐIỆN PHA2
//holdingRegs[5] = ĐIỆN ÁP PHA2
//holdingRegs[6] = CÔNG SUẤT PHA2

//holdingRegs[7] = DÒNG ĐIỆN PHA2
//holdingRegs[8] = ĐIỆN ÁP PHA2
//holdingRegs[9] = CÔNG SUẤT PHA1

//holdingRegs[10] = GIỜ ON PORT1
//holdingRegs[11] = PHUT ON PORT1
//holdingRegs[12] = GIỜ OFF PORT1
//holdingRegs[13] = PHUT OFF PORT1
//holdingRegs[14] = %_1 PORT1
//holdingRegs[15] = GIỜ SET %_1 PORT1
//holdingRegs[16] = PHUT SET %_1 PORT1
//holdingRegs[17] = %_2 PORT1
//holdingRegs[18] = GIỜ SET %_2 PORT1
//holdingRegs[19] = PHUT SET %_2 PORT1
//holdingRegs[20] = %_3 PORT1
//holdingRegs[21] = GIỜ SET %_3 PORT1
//holdingRegs[22] = PHUT SET %_4 PORT1
//holdingRegs[23] = %_4 PORT1
//holdingRegs[24] = GIỜ SET %_4 PORT1
//holdingRegs[25] = PHUT SET %_4 PORT1

//holdingRegs[26] = GIỜ ON PORT2
//holdingRegs[27] = PHUT ON PORT2
//holdingRegs[28] = GIỜ OFF PORT2
//holdingRegs[29] = PHUT OFF PORT2
//holdingRegs[30] = %_1 PORT2
//holdingRegs[31] = GIỜ SET %_1 PORT2
//holdingRegs[32] = PHUT SET %_1 PORT2
//holdingRegs[33] = %_2 PORT2
//holdingRegs[34] = GIỜ SET %_2 PORT2
//holdingRegs[35] = PHUT SET %_2 PORT2
//holdingRegs[36] = %_3 PORT2
//holdingRegs[37] = GIỜ SET %_3 PORT2
//holdingRegs[38] = PHUT SET %_4 PORT2
//holdingRegs[39] = %_4 PORT2
//holdingRegs[40] = GIỜ SET %_4 PORT2
//holdingRegs[41] = PHUT SET %_4 PORT2

//holdingRegs[42] = GIỜ ON PORT1
//holdingRegs[43] = PHUT ON PORT1
//holdingRegs[44] = GIỜ OFF PORT1
//holdingRegs[45] = PHUT OFF PORT1
//holdingRegs[46] = %_1 PORT1
//holdingRegs[47] = GIỜ SET %_1 PORT1
//holdingRegs[48] = PHUT SET %_1 PORT1
//holdingRegs[49] = %_2 PORT1
//holdingRegs[50] = GIỜ SET %_2 PORT1
//holdingRegs[51] = PHUT SET %_2 PORT1
//holdingRegs[52] = %_3 PORT1
//holdingRegs[53] = GIỜ SET %_3 PORT1
//holdingRegs[54] = PHUT SET %_4 PORT1
//holdingRegs[55] = %_4 PORT1
//holdingRegs[56] = GIỜ SET %_4 PORT1
//holdingRegs[57] = PHUT SET %_4 PORT1

//holdingRegs[58] = DÒNG CẢNH BÁO POST1
//holdingRegs[59] = DÒNG CẢNH BÁO POST2
//holdingRegs[60] = DÒNG CẢNH BÁO POST3

//holdingRegs[61] = DIM % POST1,2,3

// TRẠNG THÁI CẢNH BÁO = 0 : BINH THƯỜNG, = 1 : CAO, = 2 THẤP, = 3 : KHÔNG KẾT NỐI
//holdingRegs[64] = TRẠNG THÁI CẢNH BÁO POST 1
//holdingRegs[65] = TRẠNG THÁI CẢNH BÁO POST 1
//holdingRegs[66] = TRẠNG THÁI CẢNH BÁO POST 3


/* Check biến set thai đổi từ HMI gửi xuống
Hàm check sự thai đổi giá trị của biến holdingRegs[]
để biết được có biết nào được set thay đổi từ
HMI xuống không.
Hàm phải được bỏ trong loop để check liên tục xem có
biến holdingRegs[] nào thai đổi không để lưu set lại
thông số cho thiết bi.
*/
void checkHoldingRegsChangeVerbose() {
  static uint16_t oldRegs[TONGSO_TS];

  for (int i = 0; i <= TONGSO_TS - 1; i++) {
    if (holdingRegs[i] != oldRegs[i]) {
      oldRegs[i] = holdingRegs[i];

      switch (i) {

        //-----------Thông số hẹn giờ post 1-----------//
        case 10:
          {
            Serial.print(F("GIỜ ON PORT1: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 11:
          {
            Serial.print(F("PHÚT ON PORT1: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 12:
          {
            Serial.print(F("GIỜ OFF PORT1: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 13:
          {
            Serial.print(F("PHÚT OFF PORT1: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 14:
          {
            Serial.print(F("%_1 PORT1: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 15:
          {
            Serial.print(F("GIỜ SET %_1 PORT1: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 16:
          {
            Serial.print(F("PHÚT SET %_1 PORT1: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 17:
          {
            Serial.print(F("%_2 PORT1: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 18:
          {
            Serial.print(F("GIỜ SET %_2 PORT1: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 19:
          {
            Serial.print(F("PHÚT SET %_2 PORT1: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 20:
          {
            Serial.print(F("%_3 PORT1: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 21:
          {
            Serial.print(F("GIỜ SET %_3 PORT1: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 22:
          {
            Serial.print(F("PHÚT SET %_3 PORT1: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 23:
          {
            Serial.print(F("%_4 PORT1: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 24:
          {
            Serial.print(F("GIỜ SET %_4 PORT1: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 25:
          {
            Serial.print(F("PHÚT SET %_4 PORT1: "));
            Serial.println(holdingRegs[i]);
          }
          break;


          //-----------Thông số hẹn giờ post 2-----------//
        case 26:
          {
            Serial.print(F("GIỜ ON PORT2: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 27:
          {
            Serial.print(F("PHÚT ON PORT2: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 28:
          {
            Serial.print(F("GIỜ OFF PORT2: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 29:
          {
            Serial.print(F("PHÚT OFF PORT2: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 30:
          {
            Serial.print(F("%_1 PORT2: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 31:
          {
            Serial.print(F("GIỜ SET %_1 PORT2: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 32:
          {
            Serial.print(F("PHÚT SET %_1 PORT2: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 33:
          {
            Serial.print(F("%_2 PORT2: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 34:
          {
            Serial.print(F("GIỜ SET %_2 PORT2: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 35:
          {
            Serial.print(F("PHÚT SET %_2 PORT2: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 36:
          {
            Serial.print(F("%_3 PORT2: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 37:
          {
            Serial.print(F("GIỜ SET %_3 PORT2: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 38:
          {
            Serial.print(F("PHÚT SET %_3 PORT2: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 39:
          {
            Serial.print(F("%_4 PORT2: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 40:
          {
            Serial.print(F("GIỜ SET %_4 PORT2: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 41:
          {
            Serial.print(F("PHÚT SET %_4 PORT2: "));
            Serial.println(holdingRegs[i]);
          }
          break;


          //-----------Thông số hẹn giờ post 3-----------//
        case 42:
          {
            Serial.print(F("GIỜ ON PORT3: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 43:
          {
            Serial.print(F("PHÚT ON PORT3: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 44:
          {
            Serial.print(F("GIỜ OFF PORT3: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 45:
          {
            Serial.print(F("PHÚT OFF PORT3: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 46:
          {
            Serial.print(F("%_1 PORT3: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 47:
          {
            Serial.print(F("GIỜ SET %_1 PORT3: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 48:
          {
            Serial.print(F("PHÚT SET %_1 PORT3: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 49:
          {
            Serial.print(F("%_2 PORT3: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 50:
          {
            Serial.print(F("GIỜ SET %_2 PORT3: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 51:
          {
            Serial.print(F("PHÚT SET %_2 PORT3: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 52:
          {
            Serial.print(F("%_3 PORT3: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 53:
          {
            Serial.print(F("GIỜ SET %_3 PORT3: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 54:
          {
            Serial.print(F("PHÚT SET %_3 PORT3: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 55:
          {
            Serial.print(F("%_4 PORT3: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 56:
          {
            Serial.print(F("GIỜ SET %_4 PORT3: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 57:
          {
            Serial.print(F("PHÚT SET %_4 PORT3: "));
            Serial.println(holdingRegs[i]);
          }
          break;


          //----------Dòng cảnh báo từng post----------//
        case 58:
          {
            Serial.print(F("DÒNG CẢNH BÁO POST1: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 59:
          {
            Serial.print(F("DÒNG CẢNH BÁO POST2: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        case 60:
          {
            Serial.print(F("DÒNG CẢNH BÁO POST3: "));
            Serial.println(holdingRegs[i]);
          }
          break;


          //---------Thông số dim1,2,2 từng post---------//
        case 61:
          {
            Serial.print(F("DIM % POST1,2,3: "));
            Serial.println(holdingRegs[i]);
          }
          break;
        default:
          {
            Serial.print(F("holdingRegs["));
            Serial.print(i);
            Serial.print(F("]: "));
            Serial.println(holdingRegs[i]);
          }
          break;
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  mod.begin(9600);  // RS485 baudrate

  Serial.println(F("Modbus Slave Started..."));

  coils[0] = 0;  //MANUAL / AUTO

  coils[1] = 0;  // ON/OFF PHASE1
  coils[2] = 1;  // ON/OFF PHASE2
  coils[3] = 0;  // ON/OFF PHASE3

  coils[7] = 1;  // TRẠNG THÁI ONLINE/OFFLINE 4G

  holdingRegs[1] = 11.11 * 100;  // DÒNG ĐIỆN PHA1 (Gửi lên HMI 2 số sao đâu. nhân 100)
  holdingRegs[2] = 220.1 * 10;   // ĐIỆN ÁP PHA1  (Gửi lên HMI 1 số sao đâu. nhân 10)
  holdingRegs[3] = 12.22 * 100;  // CÔNG SUẤT PHA1 (Gửi lên HMI 2 số sao đâu. nhân 100)

  holdingRegs[4] = 22.22 * 100;  // DÒNG ĐIỆN PHA2 (Gửi lên HMI 2 số sao đâu. nhân 100)
  holdingRegs[5] = 220.2 * 10;   // ĐIỆN ÁP PHA2 (Gửi lên HMI 1 số sao đâu. nhân 10)
  holdingRegs[6] = 23.33 * 100;  // CÔNG SUẤT PHA3 (Gửi lên HMI 2 số sao đâu. nhân 100)

  holdingRegs[7] = 33.33 * 100;  // DÒNG ĐIỆN PHA2 (Gửi lên HMI 2 số sao đâu. nhân 100)
  holdingRegs[8] = 220.3 * 10;   // ĐIỆN ÁP PHA3 (Gửi lên HMI 1 số sao đâu. nhân 10)
  holdingRegs[9] = 34.44 * 100;  // CÔNG SUẤT PHA3 (Gửi lên HMI 2 số sao đâu. nhân 100)

  holdingRegs[61] = 20;  // DIM % POST1,2,3

  // DATA PORT1
  holdingRegs[10] = 18;  //GIỜ ON PORT1
  holdingRegs[11] = 30;  // PHUT ON PORT1
  holdingRegs[12] = 5;   // GIỜ OFF PORT1
  holdingRegs[13] = 30;  // PHUT OFF PORT1
  holdingRegs[14] = 20;  //%_1 PORT1
  holdingRegs[15] = 19;  //GIỜ SET %_1 PORT1
  holdingRegs[16] = 31;  //PHUT SET %_1 PORT1
  holdingRegs[17] = 40;  //%_2 PORT1
  holdingRegs[18] = 20;  //GIỜ SET %_2 PORT1
  holdingRegs[19] = 32;  //PHUT SET %_2 PORT1
  holdingRegs[20] = 60;  //%_3 PORT1
  holdingRegs[21] = 21;  //GIỜ SET %_3 PORT1
  holdingRegs[22] = 33;  //PHUT SET %_4 PORT1
  holdingRegs[23] = 80;  //%_4 PORT1
  holdingRegs[24] = 22;  //GIỜ SET %_4 PORT1
  holdingRegs[25] = 34;  //PHUT SET %_4 PORT1

  // DATA PORT2
  holdingRegs[26] = 18;  //GIỜ ON PORT2
  holdingRegs[27] = 30;  // PHUT ON PORT2
  holdingRegs[28] = 5;   // GIỜ OFF PORT2
  holdingRegs[29] = 30;  // PHUT OFF PORT2
  holdingRegs[30] = 20;  //%_1 PORT2
  holdingRegs[31] = 19;  //GIỜ SET %_1 PORT2
  holdingRegs[32] = 31;  //PHUT SET %_1 PORT2
  holdingRegs[33] = 40;  //%_2 PORT2
  holdingRegs[34] = 20;  //GIỜ SET %_2 PORT2
  holdingRegs[35] = 32;  //PHUT SET %_2 PORT2
  holdingRegs[36] = 60;  //%_3 PORT2
  holdingRegs[37] = 21;  //GIỜ SET %_3 PORT2
  holdingRegs[38] = 33;  //PHUT SET %_4 PORT2
  holdingRegs[39] = 80;  //%_4 PORT2
  holdingRegs[40] = 22;  //GIỜ SET %_4 PORT2
  holdingRegs[41] = 34;  //PHUT SET %_4 PORT2

  // DATA PORT3
  holdingRegs[42] = 18;  //GIỜ ON PORT3
  holdingRegs[43] = 30;  // PHUT ON PORT3
  holdingRegs[44] = 5;   // GIỜ OFF PORT3
  holdingRegs[45] = 30;  // PHUT OFF PORT3
  holdingRegs[46] = 20;  //%_1 PORT3
  holdingRegs[47] = 19;  //GIỜ SET %_1 PORT3
  holdingRegs[48] = 31;  //PHUT SET %_1 PORT3
  holdingRegs[49] = 40;  //%_2 PORT3
  holdingRegs[50] = 20;  //GIỜ SET %_2 PORT3
  holdingRegs[51] = 32;  //PHUT SET %_2 PORT3
  holdingRegs[52] = 60;  //%_3 PORT3
  holdingRegs[53] = 21;  //GIỜ SET %_3 PORT3
  holdingRegs[54] = 33;  //PHUT SET %_4 PORT3
  holdingRegs[55] = 80;  //%_4 PORT3
  holdingRegs[56] = 22;  //GIỜ SET %_4 PORT3
  holdingRegs[57] = 34;  //PHUT SET %_4 PORT3

  holdingRegs[58] = 7.15 * 100;   //DÒNG CẢNH BÁO POST1 (Gửi lên HMI 2 số sao đâu. nhân 100)
  holdingRegs[59] = 8.20 * 100;   //DÒNG CẢNH BÁO POST2 (Gửi lên HMI 2 số sao đâu. nhân 100)
  holdingRegs[60] = 10.00 * 100;  //DÒNG CẢNH BÁO POST3 (Gửi lên HMI 2 số sao đâu. nhân 100)

  // TRẠNG THÁI CẢNH BÁO = 0 : BINH THƯỜNG, = 1 : CAO, = 2 THẤP, = 3 : KHÔNG KẾT NỐI
  holdingRegs[64] = 0;  //TRẠNG THÁI CẢNH BÁO POST 1
  holdingRegs[65] = 2;  //TRẠNG THÁI CẢNH BÁO POST 1
  holdingRegs[66] = 3;  //TRẠNG THÁI CẢNH BÁO POST 3

  checkHoldingRegsChangeVerbose();
}

void loop() {
  if (mod.available()) {
    byte request[8];  // Mảng lưu trữ dữ liệu yêu cầu từ Master
    int bytesRead = 0;

    // Đọc đủ 8 byte yêu cầu trong khoan timer out
    uint32_t timer_out_run = millis();
    while (bytesRead < 8 && millis() - timer_out_run < 200) {
      if (mod.available()) {
        request[bytesRead] = mod.read();
        bytesRead++;
      }
    }

    // In ra dữ liệu nhận được để kiểm tra
    if (DEBUG) {
      Serial.print(F("Received request: "));
      for (int i = 0; i < 8; i++) {
        Serial.print(F("0x"));
        Serial.print(request[i], HEX);
        Serial.print(F(" "));
      }
      Serial.println();
    } else {
      delay(1);  // Đơi 1ms rồi mới phẩn hổi data lại
    }

    // Kiểm tra yêu cầu có hợp lệ không
    if (request[0] == SLAVE_ID) {
      byte functionCode = request[1];  // Mã chức năng

      if (functionCode == FC_READ_COILS) {
        handleReadCoils(request, DEBUG);  // Xử lý yêu cầu đọc coil
      }

      else if (functionCode == FC_READ_WRITE_SINGLE_COILS) {
        handleWriteSingleCoil(request, DEBUG);  // Xử lý yêu cầu ghi single coils
      }

      else if (functionCode == FC_READ_HOLDING_REGISTERS) {
        handleReadHoldingRegisters(request, DEBUG);  // Xử lý yêu cầu đọc holding registers
      }

      else if (functionCode == FC_WRITE_SINGLE_REGISTER) {
        handleWriteSingleRegister(request, DEBUG);  // Xử lý yêu cầu ghi single register

        // Gọi check biến holdingRegs có gửi từ HMI xuống không
        checkHoldingRegsChangeVerbose();
      }

      else {
        if (DEBUG) Serial.println(F("Unknown function code."));
      }
    } else {
      if (DEBUG) Serial.println(F("Invalid Slave ID."));
    }
  }

  // Bắt trạng thái biến coils[1] thai đổi
  static bool Status_P1 = !coils[1];
  if (Status_P1 != coils[1]) {
    Status_P1 = coils[1];

    if (coils[1]) {
      Serial.println(F("ON PHASE_1"));
    } else {
      Serial.println(F("OFF PHASE_1"));
    }
  }

  // Bắt trạng thái biến coils[2] thai đổi
  static bool Status_P2 = !coils[2];
  if (Status_P2 != coils[2]) {
    Status_P2 = coils[2];

    if (coils[2]) {
      Serial.println(F("ON PHASE_2"));
    } else {
      Serial.println(F("OFF PHASE_2"));
    }
  }

  // Bắt trạng thái biến coils[3] thai đổi
  static bool Status_P3 = !coils[3];
  if (Status_P3 != coils[3]) {
    Status_P3 = coils[3];

    if (coils[3]) {
      Serial.println(F("ON PHASE_3"));
    } else {
      Serial.println(F("OFF PHASE_3"));
    }
  }

  // Bắt trạng thái biến coils[0] thai đổi
  static bool Status_DK = !coils[0];
  if (Status_DK != coils[0]) {
    Status_DK = coils[0];

    if (coils[0]) {
      Serial.println(F("DK TỰ ĐỘNG"));
    } else {
      Serial.println(F("DK BẰNG TAY"));
    }
  }

  if (Serial.available()) {
    if (Serial.read() == 'd') {
      if (DEBUG) DEBUG = 0;
      else DEBUG = 1;
    }
  }

  if (DEBUG) {
    static uint32_t time_run_1s = millis();
    if (millis() - time_run_1s >= 1000) {
      time_run_1s = millis();

      Dem_debug++;
      if (Dem_debug >= 60) {
        Dem_debug = 0;
        DEBUG = 0;
      }
    }
  } else {
    Dem_debug = 0;
  }
}

// Xử lý lệnh đọc trạng thái các coil (bit ON/OFF)
void handleReadCoils(byte* request, bool debug) {
  uint16_t startAddress = (request[2] << 8) | request[3];  // Địa chỉ bắt đầu đọc
  uint16_t quantity = (request[4] << 8) | request[5];      // Số lượng coil cần đọc

  if (startAddress + quantity > sizeof(coils)) {
    if (debug) Serial.println(F("Coil read out of bounds."));  // Kiểm tra lỗi: địa chỉ vượt quá mảng coils
    return;
  }

  byte byteCount = (quantity + 7) / 8;  // Tính số byte cần gửi (8 coil = 1 byte)
  byte response[256];                   // Bộ đệm phản hồi, đủ lớn cho dữ liệu

  response[0] = SLAVE_ID;
  response[1] = FC_READ_COILS;
  response[2] = byteCount;

  for (uint16_t i = 0; i < quantity; i++) {
    uint16_t coilIndex = startAddress + i;
    if (coils[coilIndex]) {
      response[3 + (i / 8)] |= (1 << (i % 8));  // Set bit nếu coil ON
    } else {
      response[3 + (i / 8)] &= ~(1 << (i % 8));  // Clear bit nếu coil OFF
    }
  }

  // Tính CRC cho response
  uint16_t crc = crc16(response, 3 + byteCount);
  response[3 + byteCount] = crc & 0xFF;
  response[4 + byteCount] = (crc >> 8) & 0xFF;

  // Gửi phản hồi
  if (debug) Serial.print(F("Response sent for Read Coils: "));
  for (int i = 0; i < 5 + byteCount; i++) {
    mod.write(response[i]);

    if (debug) {
      Serial.print(F("0x"));
      Serial.print(response[i], HEX);
      Serial.print(F(" "));
    }
  }
  if (debug) Serial.println(F("\n"));
}

// Xử lý lệnh ghi 1 coil đơn (bật hoặc tắt)
void handleWriteSingleCoil(byte* request, bool debug) {
  uint16_t coilAddress = (request[2] << 8) | request[3];  // Địa chỉ coil cần ghi
  uint16_t coilValue = (request[4] << 8) | request[5];    // Giá trị ghi (0xFF00 = ON, 0x0000 = OFF)

  if (coilAddress >= sizeof(coils)) {
    if (debug) Serial.println(F("Invalid coil address."));  // Kiểm tra lỗi địa chỉ
    return;
  }

  if (coilValue == 0xFF00) {
    coils[coilAddress] = 1;  // Ghi coil ON
    if (debug) Serial.print(F("Writing ON to coil "));
  } else if (coilValue == 0x0000) {
    coils[coilAddress] = 0;  // Ghi coil OFF
    if (debug) Serial.print(F("Writing OFF to coil "));
  } else {
    if (debug) Serial.println(F("Invalid coil value."));  // Giá trị không hợp lệ
    return;
  }

  if (debug) Serial.println(coilAddress);

  // Gửi phản hồi lại chính xác yêu cầu (echo request)
  byte response[8];
  for (int i = 0; i < 6; i++) {
    response[i] = request[i];
  }

  // Tính CRC cho response
  uint16_t crc = crc16(response, 6);
  response[6] = crc & 0xFF;
  response[7] = (crc >> 8) & 0xFF;

  // Gửi phản hồi
  if (debug) Serial.print(F("Response sent for Write Single Coil: "));
  for (int i = 0; i < 8; i++) {
    mod.write(response[i]);

    if (debug) {
      Serial.print(F("0x"));
      Serial.print(response[i], HEX);
      Serial.print(F(" "));
    }
  }
  if (debug) Serial.println();
}

// Xử lý lệnh đọc các thanh ghi dữ liệu (Holding Registers)
void handleReadHoldingRegisters(byte* request, bool debug) {
  uint16_t startAddress = (request[2] << 8) | request[3];  // Địa chỉ bắt đầu đọc
  uint16_t quantity = (request[4] << 8) | request[5];      // Số lượng thanh ghi cần đọc

  if (startAddress + quantity > sizeof(holdingRegs) / sizeof(holdingRegs[0])) {
    if (debug) Serial.println(F("Register read out of bounds."));  // Kiểm tra lỗi
    return;
  }

  byte byteCount = quantity * 2;  // Mỗi thanh ghi 2 byte
  byte response[256];

  response[0] = SLAVE_ID;
  response[1] = FC_READ_HOLDING_REGISTERS;
  response[2] = byteCount;

  // Copy dữ liệu từ mảng holdingRegs vào response
  for (uint16_t i = 0; i < quantity; i++) {
    response[3 + i * 2] = highByte(holdingRegs[startAddress + i]);
    response[4 + i * 2] = lowByte(holdingRegs[startAddress + i]);
  }

  // Tính CRC
  uint16_t crc = crc16(response, 3 + byteCount);
  response[3 + byteCount] = crc & 0xFF;
  response[4 + byteCount] = (crc >> 8) & 0xFF;

  // Gửi phản hồi
  if (debug) Serial.print(F("Response sent for Read Holding Registers: "));
  for (int i = 0; i < 5 + byteCount; i++) {
    mod.write(response[i]);

    if (debug) {
      Serial.print(F("0x"));
      Serial.print(response[i], HEX);
      Serial.print(F(" "));
    }
  }
  if (debug) Serial.println(F("\n"));
}

// Xử lý lệnh ghi 1 thanh ghi duy nhất (Single Register)
void handleWriteSingleRegister(byte* request, bool debug) {
  uint16_t regAddress = (request[2] << 8) | request[3];  // Địa chỉ thanh ghi cần ghi
  uint16_t regValue = (request[4] << 8) | request[5];    // Giá trị cần ghi

  if (regAddress >= sizeof(holdingRegs) / sizeof(holdingRegs[0])) {
    if (debug) Serial.println(F("Invalid register address."));  // Kiểm tra lỗi
    return;
  }

  holdingRegs[regAddress] = regValue;  // Ghi giá trị vào mảng holdingRegs

  if (debug) {
    Serial.print(F("Wrote "));
    Serial.print(regValue);
    Serial.print(F(" to register "));
    Serial.println(regAddress);
  }

  // Phản hồi lại giống yêu cầu ban đầu
  byte response[8];
  for (int i = 0; i < 6; i++) {
    response[i] = request[i];
  }

  uint16_t crc = crc16(response, 6);
  response[6] = crc & 0xFF;
  response[7] = (crc >> 8) & 0xFF;

  if (debug) Serial.print(F("Response sent for Write Single Register: "));
  for (int i = 0; i < 8; i++) {
    mod.write(response[i]);

    if (debug) {
      Serial.print(F("0x"));
      Serial.print(response[i], HEX);
      Serial.print(F(" "));
    }
  }
  if (debug) Serial.println();
}

// Hàm tính CRC-16 Modbus chuẩn (Polynomial 0xA001)
unsigned int crc16(unsigned char* buf, unsigned int len) {
  unsigned int crc = 0xFFFF;
  for (unsigned int pos = 0; pos < len; pos++) {
    crc ^= (unsigned int)buf[pos];           // XOR byte vào byte thấp nhất của CRC
    for (unsigned int i = 8; i != 0; i--) {  // Duyệt từng bit
      if ((crc & 0x0001) != 0) {             // Nếu bit thấp nhất được đặt
        crc >>= 1;                           // Dịch phải và XOR với 0xA001
        crc ^= 0xA001;
      } else {
        crc >>= 1;  // Chỉ dịch phải
      }
    }
  }
  return crc;
}
