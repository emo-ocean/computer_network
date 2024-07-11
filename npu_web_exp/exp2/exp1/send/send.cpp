
#include<stdio.h>
#include<stdlib.h>
#include<pcap.h>

#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"wpcap.lib")

const u_int16_t ETHERNET_TYPE = 0x0080;
//const u_int16_t ETHERNET_TYPE = 0x0800;  // Change to the appropriate type for binary data

const int MAX_SIZE = 2048;

typedef struct ethernet_header {
	//total size=14Bytes
	u_int8_t dest_mac[6];
	u_int8_t src_mac[6];
	u_int16_t ethernet_type;
}ethernet_header;

class crc32 {
private:
	unsigned int crc32_table[256];

public:
	crc32() {
		unsigned int crc;
		for (int i = 0; i < 256; i++) {
			crc = i;
			for (int j = 0; j < 8; j++) {
				if (crc & 1) {
					crc = (crc >> 1) ^ 0xEDB88320;
				}
				else {
					crc >>= 1;
				}
			}
			crc32_table[i] = crc;
		}
	}

	unsigned int calculate_crc(char* buffer, int len) {
		unsigned int crc = 0xffffffff;
		for (int i = 0; i < len; i++) {
			crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ buffer[i]];
		}
		return crc ^ 0xffffffff;
	}
};

class packet {
public:
	u_int8_t buffer[MAX_SIZE];
	int size_of_packet;
	int size_ethernet_header;

private:
	void load_ethernet_header() {
		//强制类型转换，这里没有什么想到什么好办法让这段代码的可读性提高，也尝试用了union但是还是感觉怪怪的
		//如果有同学想到什么好的方法可以告诉我
		ethernet_header* hdr = (ethernet_header*)buffer;

		// add destination mac address,this should be consistent with reciever.
		for (int i = 0; i < 6; i++) {
			hdr->dest_mac[i] = 0xFF;
		}

		//add source mac address, this can be any value.
		hdr->src_mac[0] = 0xB4;
		hdr->src_mac[1] = 0xA9;
		hdr->src_mac[2] = 0xFC;
		hdr->src_mac[3] = 0x9C;
		hdr->src_mac[4] = 0x7B;
		hdr->src_mac[5] = 0xBB;

		// add source typy
		hdr->ethernet_type = ETHERNET_TYPE;

		// caculate the size of packet now
		size_of_packet += size_ethernet_header;
	}
public:

	packet() {
		memset(&buffer, 0, MAX_SIZE);
		size_of_packet = 0;
		size_ethernet_header = 14;
		load_ethernet_header();
	}

	void load_ethernet_data(FILE* fp, crc32& const class_crc) {
		int size_of_data = 0;
		/*
		fseek(fp, 0, SEEK_END);
		int size_of_data = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		fread(&buffer[size_ethernet_header], 1, size_of_data, fp);
		*/
		char tmp[MAX_SIZE] = { 0 };
		char ch;

		while ((ch = fgetc(fp)) != EOF) {
			tmp[size_of_data++] = ch;
		}

		for (int i = 0; i < size_of_data; i++) {
			buffer[i + size_ethernet_header] = tmp[i];//相对数据包头偏移14字节，用来保存数据
		}

		u_int32_t crc = class_crc.calculate_crc(tmp, size_of_data);
		int index = size_ethernet_header + size_of_data;//获得当前buffer的尾位置
		u_int8_t* tail = &buffer[index];//获得当前buffer的尾指针（此时偏移量为8bit）
		int* crc_num = (int*)tail;//将uchar*强制转换为int*，使得指针偏移量从8bit增置32bit，用来储存crc校验码信息。
		*crc_num = crc;

		size_of_packet += (size_of_data + 4);
		//size_of_packet += size_of_data;

	}
};



int main() {
	crc32 crc;
	packet pack;
	FILE* fp = fopen("data.txt", "r");
	//FILE* fp = fopen("video.mp4", "rb");

	pack.load_ethernet_data(fp, crc);


	pcap_t* adhandle;
	pcap_if_t* alldevs;
	pcap_if_t* d;
	int i = 0;
	int inum;
	char error_buffer[PCAP_ERRBUF_SIZE];

	// get the all network adapter handle 

	if (pcap_findalldevs(&alldevs, error_buffer) == -1) {
		printf("%s\n", error_buffer);
		return -1;
	}

	/* Print the list of all network adapter information */
	for (d = alldevs; d; d = d->next) {
		printf("%d. %s", ++i, d->name);
		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}

	if (i == 0) {
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return -1;
	}

	printf("Enter the interface number (1-%d):", i);
	scanf("%d", &inum);
	if (inum < 1 || inum > i) {
		printf("\nInterface number out of range.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* Jump to the selected adapter */
	for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);

	/* Open the adapter */
	if ((adhandle = pcap_open_live(d->name, // name of the device
		65536, // portion of the packet to capture.65536 grants that the whole packet will be captured on/// all the MACs.
		1, // promiscuous mode
		1000, // read timeout
		error_buffer // error buffer
	)) == NULL) {
		printf("\nUnable to open the adapter. %s is not supported by WinPcap\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}


	/* Check the link layer. We support only Ethernet for simplicity. */
	if (pcap_datalink(adhandle) != DLT_EN10MB) {
		printf("\nThis program works only on Ethernet networks.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	// start to send ethernet frame
	while (1) {
		pcap_sendpacket(adhandle, pack.buffer, pack.size_of_packet);;
		//printf("%d", *(int*)(buffer + size_of_packet - 4));
	}
	pcap_close(adhandle);
	return 0;
}
