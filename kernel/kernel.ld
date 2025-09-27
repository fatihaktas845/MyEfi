SECTIONS {
	. = 0x200000;

	.header : {
		KEEP(*(.header));
		*(.header);
	}

	.text : {
		*(.text*);
	}

	.data : {
		*(.data*);
	}

	.bss : {
		*(.bss*);
	}
}
