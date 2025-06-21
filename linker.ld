ENTRY(_start)

SECTIONS
{
	. = 0x0
	
	.text :
	{
		*(.text)
	} :text

	.rodata :
	{
		*(.rodata)
	} :rodata

	.data :
	{
		*(.data)
	} :data

	.bss :
	{
		*(.bss)
	} :bss
}
