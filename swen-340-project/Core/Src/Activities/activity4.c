//Create a struct to represent the midi header chunk
#include "convert_to_uint16.h"
#include "convert_to_uint32.h"
#include "song.h"
#include "printf.h"

struct header_chunk {
	char chunk_type [4];
	unsigned int length;
	short format;
	unsigned short ntrks;
	short division;
};

void run() {
	struct header_chunk* p_header;
	// Assign p_header to point to song_0

	p_header = (struct header_chunk*)(get_song(0).p_song);

//	uint8_t* p_value =  p_header->length;
//	for (int i = 0; i < 8; i++) {
//			printf("%s\r\n", p_value[i]);
//	}

	// Print out the pieces of the header one per line
	printf("Type: %s\r\n", p_header->chunk_type);
	printf("Length: %d\r\n", convert_to_uint32((uint8_t*) &(p_header->length)));
	printf("Format: 0x%04x\r\n", convert_to_uint16((uint8_t*) &(p_header->format)));
	printf("Num Tracks: %d\r\n", convert_to_uint16((uint8_t*) &(p_header->ntrks)));
	printf("Division: %d\r\n", convert_to_uint16((uint8_t*) &(p_header->division)));
}
