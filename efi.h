#pragma once

#include <stdint.h>

#define IN
#define OUT
#define CONST const
#define EFIAPI __attribute__((ms_abi))

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
typedef void          *EFI_HANDLE;

typedef struct {
	UINT32 TimeLow;
	UINT16 TimeMid;
	UINT16 TimeHighAndVersion;
	UINT8  ClockSeqAndReserved;
	UINT8  ClockSeqLow;
	UINT8  Node[6];
} __attribute__((packed)) EFI_GUID;

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_STRING)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
	IN CHAR16                                  *String
);

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
	void           *a;
	EFI_TEXT_STRING OutputString;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef struct {
	char                             a[52];
	EFI_HANDLE                       ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
} EFI_SYSTEM_TABLE;
