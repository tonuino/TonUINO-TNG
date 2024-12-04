/**
 * Library to use Arduino MFRC522 module.
 * 
 * @authors Dr.Leong, Miguel Balboa, Søren Thing Andersen, Tom Clement, many more! See GitLog.
 * 
 * Boerge1: adapted for mocking in unit tests
 */
#ifndef MFRC522_h
#define MFRC522_h

#include <Arduino.h>

class MFRC522 {
public:
	
  // MFRC522 registers. Described in chapter 9 of the datasheet.
  // When using SPI all addresses are shifted one bit left in the "SPI address byte" (section 8.1.2.3)
  enum PCD_Register : byte {
    // Page 0: Command and status
    //              0x00      // reserved for future use
    CommandReg        = 0x01 << 1,  // starts and stops command execution
    ComIEnReg       = 0x02 << 1,  // enable and disable interrupt request control bits
    DivIEnReg       = 0x03 << 1,  // enable and disable interrupt request control bits
    ComIrqReg       = 0x04 << 1,  // interrupt request bits
    DivIrqReg       = 0x05 << 1,  // interrupt request bits
    ErrorReg        = 0x06 << 1,  // error bits showing the error status of the last command executed
    Status1Reg        = 0x07 << 1,  // communication status bits
    Status2Reg        = 0x08 << 1,  // receiver and transmitter status bits
    FIFODataReg       = 0x09 << 1,  // input and output of 64 byte FIFO buffer
    FIFOLevelReg      = 0x0A << 1,  // number of bytes stored in the FIFO buffer
    WaterLevelReg     = 0x0B << 1,  // level for FIFO underflow and overflow warning
    ControlReg        = 0x0C << 1,  // miscellaneous control registers
    BitFramingReg     = 0x0D << 1,  // adjustments for bit-oriented frames
    CollReg         = 0x0E << 1,  // bit position of the first bit-collision detected on the RF interface
    //              0x0F      // reserved for future use

    // Page 1: Command
    //              0x10      // reserved for future use
    ModeReg         = 0x11 << 1,  // defines general modes for transmitting and receiving
    TxModeReg       = 0x12 << 1,  // defines transmission data rate and framing
    RxModeReg       = 0x13 << 1,  // defines reception data rate and framing
    TxControlReg      = 0x14 << 1,  // controls the logical behavior of the antenna driver pins TX1 and TX2
    TxASKReg        = 0x15 << 1,  // controls the setting of the transmission modulation
    TxSelReg        = 0x16 << 1,  // selects the internal sources for the antenna driver
    RxSelReg        = 0x17 << 1,  // selects internal receiver settings
    RxThresholdReg      = 0x18 << 1,  // selects thresholds for the bit decoder
    DemodReg        = 0x19 << 1,  // defines demodulator settings
    //              0x1A      // reserved for future use
    //              0x1B      // reserved for future use
    MfTxReg         = 0x1C << 1,  // controls some MIFARE communication transmit parameters
    MfRxReg         = 0x1D << 1,  // controls some MIFARE communication receive parameters
    //              0x1E      // reserved for future use
    SerialSpeedReg      = 0x1F << 1,  // selects the speed of the serial UART interface

    // Page 2: Configuration
    //              0x20      // reserved for future use
    CRCResultRegH     = 0x21 << 1,  // shows the MSB and LSB values of the CRC calculation
    CRCResultRegL     = 0x22 << 1,
    //              0x23      // reserved for future use
    ModWidthReg       = 0x24 << 1,  // controls the ModWidth setting?
    //              0x25      // reserved for future use
    RFCfgReg        = 0x26 << 1,  // configures the receiver gain
    GsNReg          = 0x27 << 1,  // selects the conductance of the antenna driver pins TX1 and TX2 for modulation
    CWGsPReg        = 0x28 << 1,  // defines the conductance of the p-driver output during periods of no modulation
    ModGsPReg       = 0x29 << 1,  // defines the conductance of the p-driver output during periods of modulation
    TModeReg        = 0x2A << 1,  // defines settings for the internal timer
    TPrescalerReg     = 0x2B << 1,  // the lower 8 bits of the TPrescaler value. The 4 high bits are in TModeReg.
    TReloadRegH       = 0x2C << 1,  // defines the 16-bit timer reload value
    TReloadRegL       = 0x2D << 1,
    TCounterValueRegH   = 0x2E << 1,  // shows the 16-bit timer value
    TCounterValueRegL   = 0x2F << 1,

    // Page 3: Test Registers
    //              0x30      // reserved for future use
    TestSel1Reg       = 0x31 << 1,  // general test signal configuration
    TestSel2Reg       = 0x32 << 1,  // general test signal configuration
    TestPinEnReg      = 0x33 << 1,  // enables pin output driver on pins D1 to D7
    TestPinValueReg     = 0x34 << 1,  // defines the values for D1 to D7 when it is used as an I/O bus
    TestBusReg        = 0x35 << 1,  // shows the status of the internal test bus
    AutoTestReg       = 0x36 << 1,  // controls the digital self-test
    VersionReg        = 0x37 << 1,  // shows the software version
    AnalogTestReg     = 0x38 << 1,  // controls the pins AUX1 and AUX2
    TestDAC1Reg       = 0x39 << 1,  // defines the test value for TestDAC1
    TestDAC2Reg       = 0x3A << 1,  // defines the test value for TestDAC2
    TestADCReg        = 0x3B << 1   // shows the value of ADC I and Q channels
    //              0x3C      // reserved for production tests
    //              0x3D      // reserved for production tests
    //              0x3E      // reserved for production tests
    //              0x3F      // reserved for production tests
  };

  // Commands sent to the PICC.
	enum PICC_Command : byte {
		// The commands used by the PCD to manage communication with several PICCs (ISO 14443-3, Type A, section 6.4)
		PICC_CMD_REQA			= 0x26,		// REQuest command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anticollision or selection. 7 bit frame.
		PICC_CMD_WUPA			= 0x52,		// Wake-UP command, Type A. Invites PICCs in state IDLE and HALT to go to READY(*) and prepare for anticollision or selection. 7 bit frame.
		PICC_CMD_CT				= 0x88,		// Cascade Tag. Not really a command, but used during anti collision.
		PICC_CMD_SEL_CL1		= 0x93,		// Anti collision/Select, Cascade Level 1
		PICC_CMD_SEL_CL2		= 0x95,		// Anti collision/Select, Cascade Level 2
		PICC_CMD_SEL_CL3		= 0x97,		// Anti collision/Select, Cascade Level 3
		PICC_CMD_HLTA			= 0x50,		// HaLT command, Type A. Instructs an ACTIVE PICC to go to state HALT.
		PICC_CMD_RATS           = 0xE0,     // Request command for Answer To Reset.
		// The commands used for MIFARE Classic (from http://www.mouser.com/ds/2/302/MF1S503x-89574.pdf, Section 9)
		// Use PCD_MFAuthent to authenticate access to a sector, then use these commands to read/write/modify the blocks on the sector.
		// The read/write commands can also be used for MIFARE Ultralight.
		PICC_CMD_MF_AUTH_KEY_A	= 0x60,		// Perform authentication with Key A
		PICC_CMD_MF_AUTH_KEY_B	= 0x61,		// Perform authentication with Key B
		PICC_CMD_MF_READ		= 0x30,		// Reads one 16 byte block from the authenticated sector of the PICC. Also used for MIFARE Ultralight.
		PICC_CMD_MF_WRITE		= 0xA0,		// Writes one 16 byte block to the authenticated sector of the PICC. Called "COMPATIBILITY WRITE" for MIFARE Ultralight.
		PICC_CMD_MF_DECREMENT	= 0xC0,		// Decrements the contents of a block and stores the result in the internal data register.
		PICC_CMD_MF_INCREMENT	= 0xC1,		// Increments the contents of a block and stores the result in the internal data register.
		PICC_CMD_MF_RESTORE		= 0xC2,		// Reads the contents of a block into the internal data register.
		PICC_CMD_MF_TRANSFER	= 0xB0,		// Writes the contents of the internal data register to a block.
		// The commands used for MIFARE Ultralight (from http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf, Section 8.6)
		// The PICC_CMD_MF_READ and PICC_CMD_MF_WRITE can also be used for MIFARE Ultralight.
		PICC_CMD_UL_WRITE		= 0xA2		// Writes one 4 byte page to the PICC.
	};
	
	// MIFARE constants that does not fit anywhere else
	enum MIFARE_Misc {
		MF_ACK					= 0xA,		// The MIFARE Classic uses a 4 bit ACK/NAK. Any other value than 0xA is NAK.
		MF_KEY_SIZE				= 6			// A Mifare Crypto1 key is 6 bytes.
	};

	// PICC types we can detect. Remember to update PICC_GetTypeName() if you add more.
	// last value set to 0xff, then compiler uses less ram, it seems some optimisations are triggered
	enum PICC_Type : byte {
		PICC_TYPE_UNKNOWN		,
		PICC_TYPE_ISO_14443_4	,	// PICC compliant with ISO/IEC 14443-4
		PICC_TYPE_ISO_18092		, 	// PICC compliant with ISO/IEC 18092 (NFC)
		PICC_TYPE_MIFARE_MINI	,	// MIFARE Classic protocol, 320 bytes
		PICC_TYPE_MIFARE_1K		,	// MIFARE Classic protocol, 1KB
		PICC_TYPE_MIFARE_4K		,	// MIFARE Classic protocol, 4KB
		PICC_TYPE_MIFARE_UL		,	// MIFARE Ultralight or Ultralight C
		PICC_TYPE_MIFARE_PLUS	,	// MIFARE Plus
		PICC_TYPE_MIFARE_DESFIRE,	// MIFARE DESFire
		PICC_TYPE_TNP3XXX		,	// Only mentioned in NXP AN 10833 MIFARE Type Identification Procedure
		PICC_TYPE_NOT_COMPLETE	= 0xff	// SAK indicates UID is not complete.
	};
	
	// Return codes from the functions in this class. Remember to update GetStatusCodeName() if you add more.
	// last value set to 0xff, then compiler uses less ram, it seems some optimisations are triggered
	enum StatusCode : byte {
		STATUS_OK				,	// Success
		STATUS_ERROR			,	// Error in communication
		STATUS_COLLISION		,	// Collission detected
		STATUS_TIMEOUT			,	// Timeout in communication.
		STATUS_NO_ROOM			,	// A buffer is not big enough.
		STATUS_INTERNAL_ERROR	,	// Internal error in the code. Should not happen ;-)
		STATUS_INVALID			,	// Invalid argument.
		STATUS_CRC_WRONG		,	// The CRC_A does not match
		STATUS_MIFARE_NACK		= 0xff	// A MIFARE PICC responded with NAK.
	};
	
	// A struct used for passing the UID of a PICC.
	typedef struct {
		byte		size;			// Number of bytes in the UID. 4, 7 or 10.
		byte		uidByte[10];
		byte		sak;			// The SAK (Select acknowledge) byte returned from the PICC after successful selection.
	} Uid;

	// A struct used for passing a MIFARE Crypto1 key
	typedef struct {
		byte		keyByte[MF_KEY_SIZE];
	} MIFARE_Key;
	
	// Member variables
	Uid uid;								// Used by PICC_ReadCardSerial().
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Functions for setting up the Arduino
	/////////////////////////////////////////////////////////////////////////////////////
	MFRC522() {}
	MFRC522(byte resetPowerDownPin) {}
	MFRC522(byte chipSelectPin, byte resetPowerDownPin) {}
	
  /////////////////////////////////////////////////////////////////////////////////////
  // Basic interface functions for communicating with the MFRC522
  /////////////////////////////////////////////////////////////////////////////////////
  byte PCD_ReadRegister(PCD_Register reg) { return 0x92; }

  /////////////////////////////////////////////////////////////////////////////////////
	// Functions for manipulating the MFRC522
	/////////////////////////////////////////////////////////////////////////////////////
  bool called_Init = false;
	void PCD_Init() { called_Init = true; }
  bool called_AntennaOff = false;
	void PCD_AntennaOff() { called_AntennaOff = true; }
  bool PCD_PerformSelfTest() { return true; }
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Power control functions
	/////////////////////////////////////////////////////////////////////////////////////
  bool called_SoftPowerDown = false;
	void PCD_SoftPowerDown() { called_SoftPowerDown = true; }
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Functions for communicating with PICCs
	/////////////////////////////////////////////////////////////////////////////////////
	bool called_PICC_RequestA = false;
	StatusCode PICC_RequestA(byte *bufferATQA, byte *bufferSize) {
	  if (!called_PCD_Authenticate && card_is_in) {
	    called_PICC_RequestA = true;
	    return STATUS_OK;
	  }
    called_PICC_RequestA = false;
	  return STATUS_ERROR;
	}

	bool called_PICC_HaltA = false;
	StatusCode PICC_HaltA() {
	  called_PICC_HaltA = true;
	  return STATUS_OK;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	// Functions for communicating with MIFARE PICCs
	/////////////////////////////////////////////////////////////////////////////////////
	bool called_PCD_Authenticate = false;
	StatusCode PCD_Authenticate(byte command, byte blockAddr, MIFARE_Key *key, Uid *uid) {
	  if (command == PICC_CMD_MF_AUTH_KEY_A && blockAddr == 7 && key->keyByte[0] == 0xff && uid->size != 0) {
	    called_PCD_Authenticate = true;
	    return STATUS_OK;
	  }
	  called_PCD_Authenticate = false;
	  return STATUS_ERROR;
	}
	void PCD_StopCrypto1() {
	  called_PCD_Authenticate = false;
	}
	StatusCode MIFARE_Read(byte blockAddr, byte *buffer, byte *bufferSize) {
	  if (!called_PCD_Authenticate && blockAddr != 4 && *bufferSize != buffferSizeRead)
	    return STATUS_ERROR;
	  memcpy(buffer, t_buffer, buffferSizeRead);
	  return STATUS_OK;
	}
	StatusCode MIFARE_Write(byte blockAddr, byte *buffer, byte bufferSize) {
    if (!called_PCD_Authenticate && blockAddr != 4 && bufferSize != buffferSizeWrite)
      return STATUS_ERROR;
    memcpy(t_buffer, buffer, buffferSizeWrite);
	  return STATUS_OK;
	}
	StatusCode MIFARE_Ultralight_Write(byte page, byte *buffer, byte bufferSize) {
//    if (!called_PCD_Authenticate && blockAddr != 4 && bufferSize != buffferSizeWrite)
//      return STATUS_ERROR;
//    memcpy(t_buffer, buffer, buffferSizeWrite);
	  // TODO
    return STATUS_OK;
	}

	StatusCode PCD_NTAG216_AUTH(byte *passWord, byte pACK[]) {
	  return STATUS_ERROR; // todo: implement UL
	}
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Support functions
	/////////////////////////////////////////////////////////////////////////////////////
	static const __FlashStringHelper *GetStatusCodeName(StatusCode code) { return ""; }
	static PICC_Type PICC_GetType(byte sak   ///< The SAK byte returned from PICC_Select().
	                    ) {
	  // http://www.nxp.com/documents/application_note/AN10833.pdf
	  // 3.2 Coding of Select Acknowledge (SAK)
	  // ignore 8-bit (iso14443 starts with LSBit = bit 1)
	  // fixes wrong type for manufacturer Infineon (http://nfc-tools.org/index.php?title=ISO14443A)
	  sak &= 0x7F;
	  switch (sak) {
	    case 0x04:  return PICC_TYPE_NOT_COMPLETE;  // UID not complete
	    case 0x09:  return PICC_TYPE_MIFARE_MINI;
	    case 0x08:  return PICC_TYPE_MIFARE_1K;
	    case 0x18:  return PICC_TYPE_MIFARE_4K;
	    case 0x00:  return PICC_TYPE_MIFARE_UL;
	    case 0x10:
	    case 0x11:  return PICC_TYPE_MIFARE_PLUS;
	    case 0x01:  return PICC_TYPE_TNP3XXX;
	    case 0x20:  return PICC_TYPE_ISO_14443_4;
	    case 0x40:  return PICC_TYPE_ISO_18092;
	    default:  return PICC_TYPE_UNKNOWN;
	  }
	} // End PICC_GetType()
	static const __FlashStringHelper *PICC_GetTypeName(PICC_Type piccType ///< One of the PICC_Type enums.
	                          ) {
	  switch (piccType) {
	    case PICC_TYPE_ISO_14443_4:   return F("PICC compliant with ISO/IEC 14443-4");
	    case PICC_TYPE_ISO_18092:   return F("PICC compliant with ISO/IEC 18092 (NFC)");
	    case PICC_TYPE_MIFARE_MINI:   return F("MIFARE Mini, 320 bytes");
	    case PICC_TYPE_MIFARE_1K:   return F("MIFARE 1KB");
	    case PICC_TYPE_MIFARE_4K:   return F("MIFARE 4KB");
	    case PICC_TYPE_MIFARE_UL:   return F("MIFARE Ultralight or Ultralight C");
	    case PICC_TYPE_MIFARE_PLUS:   return F("MIFARE Plus");
	    case PICC_TYPE_MIFARE_DESFIRE:  return F("MIFARE DESFire");
	    case PICC_TYPE_TNP3XXX:     return F("MIFARE TNP3XXX");
	    case PICC_TYPE_NOT_COMPLETE:  return F("SAK indicates UID is not complete.");
	    case PICC_TYPE_UNKNOWN:
	    default:            return F("Unknown type");
	  }
	} // End PICC_GetTypeName()
	
	// Support functions for debuging
	void PCD_DumpVersionToSerial() {}
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Convenience functions - does not add extra functionality
	/////////////////////////////////////////////////////////////////////////////////////
	virtual bool PICC_ReadCardSerial() {
	  if (card_is_in && called_PICC_RequestA) {
	    called_PICC_RequestA = false;
	    uid.size = 4;
	    for (uint8_t i = 0; i < uid.size; ++i)
	      uid.uidByte[i] = i+1;
	    uid.sak = 0x08; // todo: implement other card types
	    return true;
	  }
	  return false;
	}

	// implement MFRC522 with following usage:
	// Chip_card::getCardEvent():
	//   PICC_RequestA() if card in --> return STATUS_OK
	//   PICC_ReadCardSerial() if card in --> init uid
	// Chip_card::readCard()
  //   PICC_GetType() --> PICC_TYPE_MIFARE_MINI, PICC_TYPE_MIFARE_1K, PICC_TYPE_MIFARE_4K or PICC_TYPE_MIFARE_UL
	//   Chip_card::auth()
	//     PCD_Authenticate() or PCD_NTAG216_AUTH()
	//   MIFARE_Read() (for UL for more blocks) --> fills buffer
	//   PCD_StopCrypto1()
  // Chip_card::writeCard()
  //   PICC_GetType() --> PICC_TYPE_MIFARE_MINI, PICC_TYPE_MIFARE_1K, PICC_TYPE_MIFARE_4K or PICC_TYPE_MIFARE_UL
  //   Chip_card::auth()
  //     PCD_Authenticate() or PCD_NTAG216_AUTH()
  //   MIFARE_Write() (for UL for more blocks) --> fills buffer
  //   PCD_StopCrypto1()

	static constexpr size_t buffferSizeRead  = 18; // buffer size for read and write
  static constexpr size_t buffferSizeWrite = 16; // buffer size for read and write
  byte t_buffer[buffferSizeRead]{};

  bool card_is_in{false};
	void card_in(uint32_t cookie, uint8_t version, uint8_t folder, uint8_t mode, uint8_t special, uint8_t special2) {
	  card_is_in    = true    ;
	  byte coockie_4 = (cookie & 0x000000ff) >>  0;
	  byte coockie_3 = (cookie & 0x0000ff00) >>  8;
	  byte coockie_2 = (cookie & 0x00ff0000) >> 16;
	  byte coockie_1 = (cookie & 0xff000000) >> 24;
	  byte buffer[buffferSizeRead]{coockie_1, coockie_2, coockie_3, coockie_4,
                                 version,
                                 folder,
                                 static_cast<uint8_t>(mode),
                                 special,
                                 special2,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
             };
	  memcpy(t_buffer, buffer, buffferSizeRead);
	}
	void card_out() {
    card_is_in = false;
    uid.size = 0;
    uid.sak  = 0;
    called_PICC_RequestA = false;
	}
	void card_decode(uint32_t &cookie, uint8_t &version, uint8_t &folder, uint8_t &mode, uint8_t &special, uint8_t &special2) {
	  cookie = (static_cast<uint32_t>(t_buffer[0]) << 24) +
	           (static_cast<uint32_t>(t_buffer[1]) << 16) +
	           (static_cast<uint32_t>(t_buffer[2]) <<  8) +
	           (static_cast<uint32_t>(t_buffer[3]) <<  0) ;
	  version  = t_buffer[4];
    folder   = t_buffer[5];
    mode     = t_buffer[6];
    special  = t_buffer[7];
    special2 = t_buffer[8];
	}
};

#endif
