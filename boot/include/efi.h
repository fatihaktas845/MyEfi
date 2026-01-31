#ifndef EFI_H
#define EFI_H

#define NULL				((void *)0)

typedef unsigned char		BOOLEAN;
typedef signed long long	INTN;
typedef unsigned long long	UINTN;
typedef signed char			INT8;
typedef unsigned char		UINT8;
typedef signed short		INT16;
typedef unsigned short		UINT16;
typedef signed int			INT32;
typedef unsigned int		UINT32;
typedef signed long long	INT64;
typedef unsigned long long	UINT64;
typedef unsigned char		CHAR8;
typedef unsigned short		CHAR16;

typedef void				VOID;

typedef struct {
	UINT32 Data1;
	UINT16 Data2;
	UINT16 Data3;
	UINT8  Data4[8];
} EFI_GUID;

typedef unsigned long long	EFI_STATUS;
typedef void*				EFI_HANDLE;
typedef void*				EFI_EVENT;

#define IN
#define OUT
#define OPTIONAL
#define CONST
#define EFIAPI __attribute__((ms_abi))

#define EFI_SUCCESS		0

#define EFI_ERROR(Status) (((INTN)(Status)) < 0)

#define EFI_PAGE_SIZE 4096
#define EFI_SIZE_TO_PAGES(Size) \
    (((Size) + EFI_PAGE_SIZE - 1) / EFI_PAGE_SIZE)

typedef struct {
	UINT64 Signature;
	UINT32 Revision;
	UINT32 HeaderSize;
	UINT32 CRC32;
	UINT32 Reserved;
} EFI_TABLE_HEADER;

#define EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID \
	{0x387477c1, 0x69c7, 0x11d2, \
		{0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}

#define EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID \
	{0x387477c2,0x69c7,0x11d2, \
		{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}

#define EFI_LOADED_IMAGE_PROTOCOL_GUID \
	{0x5B1B31A1,0x9562,0x11d2,\
		{0x8E,0x3F,0x00,0xA0,0xC9,0x69,0x72,0x3B}}

#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID \
	{0x9042a9de,0x23dc,0x4a38,\
		{0x96,0xfb,0x7a,0xde,0xd0,0x80,0x51,0x6a}}

#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID \
	{0x0964e5b22,0x6459,0x11d2,\
		{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL
				EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *EFI_INPUT_RESET) (
		IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL	*This,
		IN BOOLEAN							ExtendedVerification
);

typedef struct {
	UINT16		ScanCode;
	CHAR16		UnicodeChar;
} EFI_INPUT_KEY;

typedef
EFI_STATUS
(EFIAPI *EFI_INPUT_READ_KEY) (
		IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL	*This,
		OUT EFI_INPUT_KEY					*Key
);

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
	EFI_INPUT_RESET			Reset;
	EFI_INPUT_READ_KEY		ReadKeyStroke;
	EFI_EVENT				WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;


typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
				EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_STRING) (
		IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*This,
		IN CHAR16							*String
);

typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_CLEAR_SCREEN) (
		IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*This
);

typedef struct {
	INT32 MaxMode;
	INT32 Mode;
	INT32 Attribute;
	INT32 CursorColumn;
	INT32 CursorRow;
	BOOLEAN CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
	void						*Reset;
	EFI_TEXT_STRING				OutputString;
	void						*TestString;
	void						*QueryMode;
	void						*SetMode;
	void						*SetAttribute;
	EFI_TEXT_CLEAR_SCREEN		ClearScreen;
	void						*SetCursorPosition;
	void						*EnableCursor;
	SIMPLE_TEXT_OUTPUT_MODE		*Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef enum {
	AllocateAnyPages,
	AllocateMaxAddress,
	AllocateAddress,
	MaxAllocateType
} EFI_ALLOCATE_TYPE;

typedef enum {
	EfiReservedMemoryType,
	EfiLoaderCode,
	EfiLoaderData,
	EfiBootServicesCode,
	EfiBootServicesData,
	EfiRuntimeServicesCode,
	EfiRuntimeServicesData,
	EfiConventionalMemory,
	EfiUnusableMemory,
	EfiACPIReclaimMemory,
	EfiACPIMemoryNVS,
	EfiMemoryMappedIO,
	EfiMemoryMappedIOPortSpace,
	EfiPalCode,
	EfiPersistentMemory,
	EfiUnacceptedMemoryType,
	EfiMaxMemoryType
} EFI_MEMORY_TYPE;

typedef UINT64 EFI_PHYSICAL_ADDRESS;

typedef
EFI_STATUS
(EFIAPI *EFI_ALLOCATE_PAGES) (
		IN EFI_ALLOCATE_TYPE		Type,
		IN EFI_MEMORY_TYPE			MemoryType,
		IN UINTN					Pages,
		IN OUT EFI_PHYSICAL_ADDRESS *Memory
);

typedef
EFI_STATUS
(EFIAPI *EFI_FREE_PAGES) (
		IN EFI_PHYSICAL_ADDRESS		Memory,
		IN UINTN					Pages
);

typedef UINT64 EFI_VIRTUAL_ADDRESS;

typedef struct {
	UINT32					Type;
	EFI_PHYSICAL_ADDRESS	PhysicalStart;
	EFI_VIRTUAL_ADDRESS		VirtualStart;
	UINT64					NumberOfPages;
	UINT64					Attribute;
} EFI_MEMORY_DESCRIPTOR;

typedef
EFI_STATUS
(EFIAPI *EFI_GET_MEMORY_MAP) (
		IN OUT UINTN				*MemoryMapSize,
		OUT EFI_MEMORY_DESCRIPTOR	*MemoryMap,
		OUT UINTN					*MapKey,
		OUT UINTN					*DescriptorSize,
		OUT UINT32					*DescriptorVersion
);

typedef
EFI_STATUS
(EFIAPI *EFI_ALLOCATE_POOL) (
		IN EFI_MEMORY_TYPE			PoolType,
		IN UINTN					Size,
		OUT VOID					**Buffer
);

typedef
EFI_STATUS
(EFIAPI *EFI_FREE_POOL) (
		IN VOID *Buffer
);

typedef
EFI_STATUS
(EFIAPI *EFI_EXIT_BOOT_SERVICES) (
		IN EFI_HANDLE	ImageHandle,
		IN UINTN		MapKey
);

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL	0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL			0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL			0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER	0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER				0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE				0x00000020

typedef
EFI_STATUS
(EFIAPI *EFI_OPEN_PROTOCOL) (
		IN EFI_HANDLE		Handle,
		IN EFI_GUID			*Protocol,
		OUT VOID			**Interface OPTIONAL,
		IN EFI_HANDLE		AgentHandle,
		IN EFI_HANDLE		ControllerHandler,
		IN UINT32			Attributes
);

typedef
EFI_STATUS
(EFIAPI *EFI_CLOSE_PROTOCOL) (
		IN EFI_HANDLE		Handle,
		IN EFI_GUID			*Protocol,
		IN EFI_HANDLE		AgentHandle,
		IN EFI_HANDLE		ControllerHandler
);

typedef
EFI_STATUS
(EFIAPI *EFI_LOCATE_PROTOCOL) (
		IN EFI_GUID		*Protocol,
		IN VOID			*Registration OPTIONAL,
		OUT VOID		**Interface
);

typedef struct {
	EFI_TABLE_HEADER Hdr;

	//
	// Task Priority Services
	//
	void *RaiseTPL;
	void *RestoreTPL;

	//
	// Memory Services
	//
	EFI_ALLOCATE_PAGES	AllocatePages;
	EFI_FREE_PAGES		FreePages;
	EFI_GET_MEMORY_MAP	GetMemoryMap;
	EFI_ALLOCATE_POOL	AllocatePool;
	EFI_FREE_POOL		FreePool;

	//
	// Event & Timer Services
	//
	void *CreateEvent;
	void *SetTimer;
	void *WaitForEvent;
	void *SignalEvent;
	void *CloseEvent;
	void *CheckEvent;

	//
	// Protocol Handler Services
	//
	void *InstallProtocolInterface;
	void *ReinstallProtocolInterface;
	void *UninstallProtocolInterface;
	void *HandleProtocol;
	VOID *Reserved;
	void *RegisterProtocolNotify;
	void *LocateHandle;
	void *LocateDevicePath;
	void *InstallConfigurationTable;

	//
	// Image Services
	//
	void					*LoadImage;
	void					*StartImage;
	void					*Exit;
	void					*UnloadImage;
	EFI_EXIT_BOOT_SERVICES	ExitBootServices;

	//
	// Miscellaneous Services
	//
	void *GetNextMonotonicCount;
	void *Stall;
	void *SetWatchdogTimer;

	//
	// DriverSupport Services
	//
	void *ConnectController;
	void *DisconnectController;

	//
	// Open and Close Protocol Services
	//
	EFI_OPEN_PROTOCOL	OpenProtocol;
	EFI_CLOSE_PROTOCOL	CloseProtocol;
	void				*OpenProtocolInformation;

	//
	// Library Services
	//
	void				*ProtocolsPerHandle;
	void				*LocateHandleBuffer;
	EFI_LOCATE_PROTOCOL	LocateProtocol;
	void				*InstallMultipleProtocolInterfaces;
	void				*UninstallMultipleProtocolInterfaces;

	// ...
} EFI_BOOT_SERVICES;

typedef struct {
	EFI_TABLE_HEADER				Hdr;
	CHAR16							*FirmwareVendor;
	UINT32							FirmwareRevision;
	EFI_HANDLE						ConsoleInHandle;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL	*ConIn;
	EFI_HANDLE						ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*ConOut;
	EFI_HANDLE						StandardErrorHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
	void							*RuntimeServices;
	EFI_BOOT_SERVICES				*BootServices;

	// ...
} EFI_SYSTEM_TABLE;

typedef struct {
	UINT32				Revision;
	EFI_HANDLE			ParentHandle;
	EFI_SYSTEM_TABLE	*SystemTable;

	// Source Location of the Image
	EFI_HANDLE			DeviceHandle;

	// ...
} EFI_LOADED_IMAGE_PROTOCOL;

typedef struct {
	UINT32 RedMask;
	UINT32 GreenMask;
	UINT32 BlueMask;
	UINT32 ReservedMask;
} EFI_PIXEL_BITMASK;

typedef enum {
	PixelRedGreenBlueReserved8BitPerColor,
	PixelBlueGreenRedReserved8BitPerColor,
	PixelBitMask,
	PixelBltOnly,
	PixelFormatMax
} EFI_GRAPHICS_PIXEL_FORMAT;

typedef struct {
	UINT32						Version;
	UINT32						HorizontalResolution;
	UINT32						VerticalResolution;
	EFI_GRAPHICS_PIXEL_FORMAT	PixelFormat;
	EFI_PIXEL_BITMASK			PixelInformation;
	UINT32						PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct {
	UINT32									MaxMode;
	UINT32									Mode;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION	*Info;
	UINTN									SizeOfInfo;
	EFI_PHYSICAL_ADDRESS					FrameBufferBase;
	UINTN									FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct _EFI_GRAPHICS_OUTPUT_PROTOCOL
				EFI_GRAPHICS_OUTPUT_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE) (
		IN EFI_GRAPHICS_OUTPUT_PROTOCOL				*This,
		IN UINT32									ModeNumber,
		OUT UINTN									*SizeOfInfo,
		OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION	**Info
);

typedef
EFI_STATUS
(EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE) (
		IN EFI_GRAPHICS_OUTPUT_PROTOCOL			*This,
		IN UINT32								ModeNumber
);

typedef struct {
	UINT8 Blue;
	UINT8 Green;
	UINT8 Red;
	UINT8 Reserved;
} EFI_GRAPHICS_OUTPUT_BLT_PIXEL;

typedef enum {
	EfiBltVideoFill,
	EfiBltVideoToBltBuffer,
	EfiBltBufferToVideo,
	EfiBltVideoToVideo,
	EfiGraphicsOutputBltOperationMax
} EFI_GRAPHICS_OUTPUT_BLT_OPERATION;

typedef
EFI_STATUS
(EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT) (
		IN EFI_GRAPHICS_OUTPUT_PROTOCOL      *This,
		IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer OPTIONAL,
		IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation,
		IN UINTN                             SourceX,
		IN UINTN                             SourceY,
		IN UINTN                             DestinationX,
		IN UINTN                             DestinationY,
		IN UINTN                             Width,
		IN UINTN                             Height,
		IN UINTN                             Delta OPTIONAL
);

typedef struct _EFI_GRAPHICS_OUTPUT_PROTOCOL {
	EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE QueryMode;
	EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE	SetMode;
	EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT		Blt;
	EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE		*Mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

typedef struct _EFI_FILE_PROTOCOL
				EFI_FILE_PROTOCOL;

#define EFI_FILE_MODE_READ		0x0000000000000001
#define EFI_FILE_MODE_WRITE		0x0000000000000002
#define EFI_FILE_MODE_CREATE	0x8000000000000000

#define EFI_FILE_READ_ONLY		0x0000000000000001
#define EFI_FILE_HIDDEN			0x0000000000000002
#define EFI_FILE_SYSTEM			0x0000000000000004
#define EFI_FILE_RESERVED		0x0000000000000008
#define EFI_FILE_DIRECTORY		0x0000000000000010
#define EFI_FILE_ARCHIVE		0x0000000000000020
#define EFI_FILE_VALID_ATTR		0x0000000000000037

typedef
EFI_STATUS
(EFIAPI *EFI_FILE_OPEN) (
		IN EFI_FILE_PROTOCOL			*This,
		OUT EFI_FILE_PROTOCOL			**NewHandle,
		IN CHAR16						*FileName,
		IN UINT64						OpenMode,
		IN UINT64						Attributes
);

typedef
EFI_STATUS
(EFIAPI *EFI_FILE_CLOSE) (
		IN EFI_FILE_PROTOCOL			*This
);

typedef
EFI_STATUS
(EFIAPI *EFI_FILE_READ) (
		IN EFI_FILE_PROTOCOL			*This,
		IN OUT UINTN					*BufferSize,
		OUT VOID						*Buffer
);

typedef
EFI_STATUS
(EFIAPI *EFI_FILE_SET_POSITION) (
		IN EFI_FILE_PROTOCOL			*This,
		IN UINT64						Position
);

typedef struct _EFI_FILE_PROTOCOL {
	UINT64					Revision;
	EFI_FILE_OPEN			Open;
	EFI_FILE_CLOSE			Close;
	void					*Delete;
	EFI_FILE_READ			Read;
	void					*Write;
	void					*GetPosition;
	EFI_FILE_SET_POSITION	SetPosition;
	// ...
} EFI_FILE_PROTOCOL;

typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
				EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME) (
		IN EFI_SIMPLE_FILE_SYSTEM_PROTOCOL	*This,
		OUT EFI_FILE_PROTOCOL				**Root
);

typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
	UINT64											Revision;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME		OpenVolume;
} _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

#endif // EFI_H
