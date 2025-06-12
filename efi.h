#pragma once

#include <stdint.h>

#define IN
#define OUT
#define OPTIONAL
#define CONST const
#define EFIAPI __attribute__((ms_abi))

//
// Variables
//
typedef uint8_t        BOOLEAN;
typedef int64_t        INTN;
typedef uint64_t       UINTN;
typedef int8_t         INT8;
typedef uint8_t        UINT8;
typedef int16_t        INT16;
typedef uint16_t       UINT16;
typedef int32_t        INT32;
typedef uint32_t       UINT32;
typedef int64_t        INT64;
typedef uint64_t       UINT64;
typedef uint8_t        CHAR8;
typedef uint16_t       CHAR16;
typedef void           VOID;
typedef uint64_t       EFI_STATUS;
typedef void           *EFI_HANDLE;
typedef void           *EFI_EVENT;

typedef struct {
	UINT32 TimeLow;
	UINT16 TimeMid;
	UINT16 TimeHighAndVersion;
	UINT8  ClockSeqAndReserved;
	UINT8  ClockSeqLow;
	UINT8  Node[6];
} __attribute__((packed)) EFI_GUID;

//
// Status Codes
//
#define EFI_SUCCESS 0

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *EFI_INPUT_RESET) (
	IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL     *This,
	IN BOOLEAN                            ExtendedVerification
);

typedef struct {
	UINT16    ScanCode;
	CHAR16    UnicodeChar;
} EFI_INPUT_KEY;

typedef
EFI_STATUS
(EFIAPI *EFI_INPUT_READ_KEY) (
	IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL     *This,
	OUT EFI_INPUT_KEY                     *Key
);

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
	EFI_INPUT_RESET           Reset;
	EFI_INPUT_READ_KEY        ReadKeyStroke;
	EFI_EVENT                 WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;


typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_STRING)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
	IN CHAR16                          *String
);

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
	void            *a;
	EFI_TEXT_STRING OutputString;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;



typedef struct {
	UINT64 Signature;
	UINT32 Revision;
	UINT32 HeaderSize;
	UINT32 CRC32;
	UINT32 Reserved;
} EFI_TABLE_HEADER;

typedef struct {
	EFI_TABLE_HEADER                Hdr;
	CHAR16                          *FirmwareVendor;
	UINT32                          FirmwareRevision;
	EFI_HANDLE                      ConsoleInHandle;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *ConIn;
	EFI_HANDLE                      ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
	EFI_HANDLE                      StandartErrorHandle;
	EFI_SIMPLE_TEXT_OUTPUT_HANDLE   *StdErr;
} EFI_SYSTEM_TABLE;

typedef
EFI_STATUS
(EFIAPI *EFI_IMAGE_ENTRY_POINT) (
	IN EFI_HANDLE                   ImageHandle,
	IN EFI_SYSTEM_TABLE             *SystemTable
);
