sudo dmidecode | grep -A16 "Memory Device$"  

#[root@localhost memory]# sudo dmidecode | grep -A16 "Memory Device$"  
#Memory Device
#	Array Handle: 0x1000
#	Error Information Handle: Not Provided
#	Total Width: Unknown
#	Data Width: Unknown
#	Size: 8192 MB
#	Form Factor: DIMM
#	Set: None
#	Locator: DIMM 0
#	Bank Locator: Not Specified
#	Type: RAM
#	Type Detail: Other
#	Speed: Unknown
#	Manufacturer: QEMU
#	Serial Number: Not Specified
#	Asset Tag: Not Specified
#	Part Number: Not Specified

# 或者直接执行
dmidecode -t memory

