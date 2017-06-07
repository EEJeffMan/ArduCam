#include <SPI.h>
#include <SD.h>

// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;

// file name
char filename[] = "testfile00.jpg";

// number from 0-99 that represents the last two digits of the file name.
unsigned int file_index = 0;

char temp = 0;

unsigned int transfer_ready = 0;
unsigned int transfer_complete = 0;

// the logging file
File testfile;

void error()
{
  while(1);
}

void setup(void)
{
unsigned int i = 0;
  
  Serial.begin(9600);
  
  // initialize the SD card
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    error();//"Card failed, or not present");
  }
//  Serial.println("card initialized.");
  
  // create a new file
	for (uint8_t i = 0; i < 100; i++) 
	{
		filename[8] = i/10 + '0';
		filename[9] = i%10 + '0';
		if (! SD.exists(filename)) 
		{
			// only open a new file if it doesn't exist
			testfile = SD.open(filename, FILE_WRITE); 
			break;  // leave the loop!
		}
	}
	file_index = i;
  //}
  
  if (! testfile) {
    error();//"couldnt create file");
  }
}

void loop(void)
{
	char length_data[10];
	unsigned int length = 0;
	unsigned int index = 0;
	
	unsigned int i = 0;

//	Read in length
	while (!Serial.available());
	length_data[index++] = Serial.read();
	if(index >= 10) 
	{
		error();
	}
	else if(length_data[index-1] == 0)
	{
		Serial.println('a');
		for(i=0; i< index-1; i++)
		{
			Serial.print(length_data[i]);
		}
		Serial.println('b');
		
		//convert ascii number to dec
		
	}
	
	if(transfer_ready)
	{
		//read in data bytes one at a time  
		while(1)
		{
			while (!Serial.available());
			temp = Serial.read();
			
			if(transfer_complete)
				break;
			else
			{
				testfile.write(temp);
			}
		}
		  
		//when complete, close file and open new file in prep for next file transfer
		
		//testfile = SD.close();
		
		file_index++;

		if(file_index >= 100)
		{
			filename[8] = file_index/10 + '0';
			filename[9] = file_index%10 + '0';
			if (! SD.exists(filename)) 
			{
				// only open a new file if it doesn't exist
				testfile = SD.open(filename, FILE_WRITE); 
			}
		}
		else
			error();//"couldnt create file");
	}
}


