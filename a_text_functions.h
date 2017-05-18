// functions for the canton electronics 16x64 led matrix.
// Andres Vilallave
// mail: andresvilallave@gmail.com

#define byte unsigned char

byte canvas[16][64]; //each byte it's a matrix bit.(only 0 or 1 values). Left to right and up to down. [0][0] = left top matrix corner.
unsigned char buffer[128]; // 2 bits per pixel. reversed bit weight.

void fillCanvas(byte x){
    int n, m;
	for (n=0;n<16;n++) 
		for (m=0;m<64;m++) canvas[n][m]=x;
}

void checkerCanvas(){
	byte val = 0;
    int n,m;
    
	for (n=0;n<16;n++) {
		if (val) val = 0;
		else val = 1;
		for (m=0;m<64;m++) {
			canvas[n][m]=val;
			if (val) val = 0;
			else val = 1;
		}
	}
}

void testLines(){
	int val;
	int n, m;
    
	for (n=0;n<16;n++) {
		for (m=0;m<64;m++) {
			if (m <= n) val = 1;
			else val = 0;
			canvas[n][m]=val;
		}
	}
}


#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))



void translateCanvasToBuffer(){
	unsigned char temp=0x00;
	//memset(buffer,0,256);
	int _bit=0;
	int index=0;
    int n, m;
    
	for (n=0;n<16;n++) {
		for (m=0;m<64;m++) {
			bitWrite(temp,_bit,canvas[n][m]);
			_bit++;
			if (_bit>7){
				_bit=0;
				//  translateToReverseDobleBit(temp,buffer[index*2],buffer[index*2+1]);
				buffer[index] = temp;
				index++;
				temp=0x00;
			} 
		}  
	}
}  


int cuantos_bytes(byte a){
  int result = a / 8;
   if (a % 8 > 0) 
    result = (a / 8)+1;
    return result;
}

void textToCanvas(const unsigned char font[],int fontHeight, int fontWidth, char *message, int row, int column) {
   
	int offset_row=fontHeight*row;  //start at row. 
	// int offset_column=fontHeight*column;  //start at column
	int offset_font=0;
	int bytesPerCharacter=fontHeight * cuantos_bytes(fontWidth);  //fuentes, cada linea de byte en byte.
	int bitsToRead=0;
	int bitIndex=0;
	int totalIndex=0;

  
	bitsToRead=7;
  
	if (fontWidth<8) bitsToRead=fontWidth-1;
 
	// for (int n=0; n<message.length()&&((column+(n*fontWidth)) < 64); n++){
    unsigned int n;
	for (n=0; n<strlen(message); n++){
        offset_font=(message[n]-32) * bytesPerCharacter;  // font sets start at 32 in ascii
		totalIndex=0;
     
		// to far to teh left ?
		if ((column+7+(n*fontWidth)) < 0) continue;
    
		//  Serial.print("Char:"); Serial.println(message.charAt(n));
        int x;
		for (x=0;x<fontHeight;x++){ 
		//	unsigned char c =  pgm_read_byte_near (font + (offset_font+x));
			unsigned char c =  font[offset_font+x];
			bitIndex=bitsToRead;
            
            int y;
			for (y=0;y<fontWidth;y++){
				int yoffset = column+y+(n*fontWidth);
				if (yoffset < 0) {
					totalIndex++;
					bitIndex--;
					if (bitIndex==-1) bitIndex=bitsToRead;
					continue;  
				}
          
				if (yoffset > 63) break;  
				//   Serial.print("y offset: "); Serial.println(yoffset);
				canvas[offset_row+x][yoffset]=bitRead(c , bitIndex);  
				totalIndex++;
				bitIndex--;
				if (bitIndex==-1) bitIndex=bitsToRead;
			} // row of pixels
        
		} // column of pixels
    
	} // for loop

}

