SECTIONS {
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
