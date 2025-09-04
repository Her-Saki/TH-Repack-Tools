//Decode and encode To Heart PAK files.
//By Saki - 2020-2023.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#define KEYLEN 11

FILE *InputFile;
FILE *SourceFile;
FILE *EncodedFile;
FILE *CompleteFile;
FILE *EncryptedFile;
FILE *TempFile;
FILE *ScenarioFile;
char InputFileName[20];
char SupraInputFileName[100];
char EncInputFileName[20];
char OutputFileName[20];
unsigned char *SourceData;
unsigned char *ScenarioData;
unsigned char *CompleteData;
static int InputFileSize;

char InputFileSizeArr[5];
char Zero[2] = {0, 0};
unsigned char *NotedData;
long NotedFileSize;

static int SourceFileSize;

#define R 0

int get_len_th(char letter)
{
	int len;

	if(
			letter == 0x20 ||
			letter == '.' ||
			letter == ',' ||
			letter == '*' ||
			letter == '!' ||
			letter == '\'' ||
			letter == '"' ||
			letter == 'i' ||
			letter == 'l'
	)
	{
		len = 1;
	}
	else
	{
		len = 2;
	}

	return len;
}

void GetSourceData()
{
    fseek(InputFile, 0L, SEEK_END);
    InputFileSize = ftell(InputFile);
    fseek(InputFile, 0L, SEEK_SET);
    SourceData = malloc(InputFileSize * sizeof(char));
    fread(SourceData, sizeof(char), InputFileSize, InputFile);
    return;
}

void uwu()
{
	printf("algo");
    return;
}

void GetSourceDataEncodedFile()
{
    EncodedFile = fopen(OutputFileName, "rb");
    SourceData = malloc(InputFileSize * sizeof(char));
    fread(SourceData, sizeof(char), InputFileSize, EncodedFile);
    fclose(EncodedFile);
    return;
}

void EncryptFile()
{
    sprintf(OutputFileName, "%s_Encrypted.BIN", InputFileName);
    EncryptedFile = fopen(OutputFileName, "wb");

    char Key[KEYLEN] = {0xd1, 0x58, 0x6a, 0x56, 0x9a, 0x13, 0xa5, 0xf7, 0x7c, 0x3e, 0x74};
    int KeyPosition = 0;
    int SDPosition = 0;

    unsigned char *SourceData;
    fseek(InputFile, 0L, SEEK_END);
    InputFileSize = ftell(InputFile);
    fseek(InputFile, 0L, SEEK_SET);
    SourceData = malloc(InputFileSize * sizeof(char));
    fread(SourceData, sizeof(char), InputFileSize, InputFile);

    printf("\nKeyPosition1: %02X\n", KeyPosition);
    printf("\nSize: %02X\n", InputFileSize);

    for (SDPosition = 0, KeyPosition = 0; SDPosition < InputFileSize; SDPosition++, KeyPosition++)
    {
        SourceData[SDPosition] = (SourceData[SDPosition] + Key[KeyPosition % 11]) & 0xFF;
        printf("KeyPosition: %02X", KeyPosition);
    }

    printf("KeyPosition3: %02X", KeyPosition);

    fwrite(SourceData, sizeof(char), InputFileSize, EncryptedFile);
    return;
}

void EncodingFile()
{
    sprintf(OutputFileName, "%s_Encoded.ENCODED", InputFileName);
    EncodedFile = fopen(OutputFileName, "wb");

    fseek(InputFile, 0L, SEEK_END);
    InputFileSize = ftell(InputFile);
    fseek(InputFile, 0, SEEK_SET);

    //Write size
    int AnnoyingF = 0xFF;
    long NegativeInputFileSize = ~(InputFileSize);
    fwrite(&NegativeInputFileSize, sizeof(char), 4, EncodedFile);
    fwrite(&AnnoyingF, sizeof(char), 1, EncodedFile);
    fseek(EncodedFile, 5, SEEK_SET);

    unsigned char InputByte;
    unsigned char IterationCounter;
    unsigned char FByte = 0xFF;

    for (; InputFileSize != 0; InputFileSize--)
    {
        if (IterationCounter++ == 8)
        {
            fwrite(&FByte, sizeof(char), 1, EncodedFile);
            IterationCounter = 0;
            InputFileSize++;
        }
        else
        {
            InputByte = fgetc(InputFile);
            fwrite(&InputByte, sizeof(char), 1, EncodedFile);
        }
    }

    fseek(EncodedFile, 0L, SEEK_END);
    InputFileSize = ftell(EncodedFile);
    fclose(EncodedFile);
    return;
}

int EncodingFileAll()
{
    int SceneNumber = 393;
    char AkariFileList[393][20] = { //Morning scenes
									"0000", "0001", "0002", "0003", "0004", "0005", "0020", "0021", "0033", "0034",
									"0035", "0036", "0037", "0038", "0039", "003A", "003B", "003C", "003D", "003E",
									"003F", "0040", "0041", "0042", "0043", "0044", "0045", "0046", "0047", "0048",
									"0049", "0057", "0058", "0059", "005A", "005B", "005C", "005D", "005E", "005F",
									"0060", "0061", "0062", "0063", "0064", "0065", "0066", "0067", "0068", "0069",
									"006A", "006B", "006C", "006D", "006E", "006F", "0070", "0071", "0072", "0080",
									"0081", "0082", "0083", "0084", "0085", "0086", "0087", "0088", "0089", "008A",
									"008B", "008C", "008D", "008E", "008F", "0090", "0091", "0092", "0093", "0094",
									"0095", "0096", "0097", "0098", "0099", "009A", "00A0", "00A1", "00A2", "00A3",
									"00A4", "00A5", "00A6", "00A7", "00A8", "00A9", "00AA", "00AB", "00AC", "00AD",
									"00AE", "00AF", "00B0", "00B1", "00B2", "00B3", "00B4", "00B5", "00B6", "00B7",
									"00B8", "00B9", "00BA", "00BB", "00BC", "00BD", "00BE", "00BF", "00C0", "00C1",
									"00C2", "00C3", "00C4", "00C5", "00C6", "00C7", "00C8", "00C9", "00CA", "00CB",
									"00CC", "00CD", "00CE", "00CF", "00D0", "00D1", "00D2", "00D3", "00D4", "00D5",
									"00D6", "00D7", "00D8", "00D9", "00DA", "00DB", "00DC", "00DD", "00F0", "00F1",
									"00F2", "00F3",
									//Akari scenes
									"0100", "0101", "0102", "0103", "0104", "0105", "0106", "0107", "0108", "0109",
                                    "010A", "0110", "0120", "0121", "0122", "0123", "0124", "0125", "0126", "0127",
                                    "0128", "0129", "012A", "012B", "012C", "0140", "0141", "0142", "0143", "0144",
                                    "0145", "0146", "0147", "0148", "0149", "014A", "014B", "014C", "014D", "014E",
                                    "014F", "0150", "0151", "0152", "0153", "0154", "0155", "0156", "0157", "0158",
                                    "0159", "015A", "015B", "015C", "015D", "015E", "015F", "0160", "0161", "0162",
                                    "0163", "0164", "0165", "0166", "0167", "0180", "0181", "0182", "0183", "0184",
                                    "0185", "0186", "0187", "0188", "0189", "018A", "018B", "018C", "018D", "018E",
                                    "018F", "0190", "0191", "0192", "0193", "0194", "0195", "0196", "0197", "0198",
                                    "0199", "019A", "019B", "019C", "019D", "019E", "019F", "01A0", "01A1", "01A2",
                                    "01A3", "01A4", "01A5", "01A6", "01A7", "01A8", "01A9", "01AA", "01AB", "01AC",
                                    "01AD", "01AE", "01AF", "01B0", "01B1", "01B2", "01B3", "01B4", "01B5", "01B6",
                                    "01B7", "01B8", "01B9", "01BA", "01BB", "01BC", "01BD", "01BE", "01BF", "01C0",
                                    "01C1", "01C2", "01C3", "01C4", "01C5", "01C6", "01C7", "01C8", "01C9", "01CA",
                                    "01CB", "01CC", "01CD", "01CE", "01CF",
									//Serika scenes
									"0200", "0201", "0210", "0220", "0221", "0222", "0223", "0224", "0241", "0242",
									"0243", "0244", "0245", "0246", "0247", "0260", "0261", "0262", "0280", "0281",
									"0282", "0283", "0284", "0285", "0286", "0287", "0288", "0289", "028A", "028B",
									"028C", "028D", "028E", "028F", "0290", "0291", "0292", "0293", "0294", "0295",
									"0296", "0297", "0298", "0299", "029A", "029B", "029C", "029D", "029E", "029F",
									"02A0", "02A1", "02A2", "02A3", "02A4", "02A5", "02A6", "02A7", "02A8", "02A9",
									"02AA", "02AB", "02AC", "02AD", "02AE", "02AF", "02B0", "02B1", "02B2", "02B3",
									"02B4", "02B5", "02B6", "02B7", "02B8", "02B9", "02BA", "02BB", "02BC", "02BD",
									"02BE", "02BF", "02C0", "02C1", "02C2", "02C3", "02C4", "02C5", "02C6", "02C7",
									"02C8", "02C9", "02CA", "02CB", "02CC", "02CD"
    								};

    //Size variables
    long EncodedFileSize;
    long NegativeInputFileSize;
    int ScenarioSize;
    long FullSize;
    long RouteBlockSize = 0;

    //File pointers
    FILE *SceneFile;
    FILE *ScenarioFile;
    FILE *TableFile;
    FILE *LVNS3PAK;
    FILE *NEWLVNS3PAK;

    //File reading variables
    int SceneCounter = 0;
    char SceneFileName[20];
    char ScenarioFileName[20];
    char FileNumber[20];

    //Arrays
    unsigned char *SourceData;
    unsigned char *ScenarioData;
    unsigned char *CompleteData;
    unsigned char *PropertyData;
    PropertyData = malloc(24 * sizeof(char));
    unsigned char *PropertyBlock;
    PropertyBlock = malloc(0x24D8 * sizeof(char));
    unsigned char *TableData;
    TableData = malloc(0x5D18 * sizeof(char));
    unsigned char *RouteBlock;
    RouteBlock = malloc(0xFFFFFF * sizeof(char));
    unsigned char *LVNS3Data;
    LVNS3Data = malloc(0x11EE42 * sizeof(char));

    //Property file variables
    char EmptyBytes[4] = {0x20, 0x20, 0x20, 0x20};
    char SCNString[4] = {0x53, 0x43, 0x4E, 0x00};
    long PropertyPosition = 0;
    long HeadOfNextFile = 0;
    long LastPosition = 0;

    //Key variables
    char Key[11] = {0xd1, 0x58, 0x6a, 0x56, 0x9a, 0x13, 0xa5, 0xf7, 0x7c, 0x3e, 0x74};
    int KeyPosition = 0;

    //Encoding file variables
    int SDPosition;
    int IT;

    //Main loop
    for (SceneCounter = 0; SceneCounter < SceneNumber; SceneCounter++)
    {
        //Get a file
        sprintf(FileNumber, "%s", AkariFileList + SceneCounter);
        sprintf(SceneFileName, "%s_TEXT.BIN", FileNumber);
        SceneFile = fopen(SceneFileName, "rb");

        //Get file size
        fseek(SceneFile, 0L, SEEK_END);
        EncodedFileSize = ftell(SceneFile);
        fseek(SceneFile, 0, SEEK_SET);
        //Get negative file size
        NegativeInputFileSize = ~(EncodedFileSize);

		//Get encoded file size
        EncodedFileSize += (EncodedFileSize / 8) + 5;

        //Allocate source data
        SourceData = malloc(EncodedFileSize * sizeof(char));

        //Write text
        for (SDPosition = 4, IT = 0; SDPosition != EncodedFileSize; SDPosition++, IT++)
        {
            if (IT % 9 == 0)
            {
                SourceData[SDPosition] = 0xFF;
            }

            else
            {
                SourceData[SDPosition] = fgetc(SceneFile);
            }
        }

		//Bugfix
		if (SourceData[EncodedFileSize - 1] == 0xFF)
		{
			EncodedFileSize--;
			SourceData = (unsigned char *)realloc(SourceData, EncodedFileSize);
		}

        //Write size
        memcpy(SourceData, &NegativeInputFileSize, 4);

        //NOT operation
        for (SDPosition = 0; SDPosition < EncodedFileSize; SDPosition++)
        {
            SourceData[SDPosition] = ~(SourceData[SDPosition]);
        }

        //Scenario file
        sprintf(ScenarioFileName, "%s_SCENARIO.scn", FileNumber);
        ScenarioFile = fopen(ScenarioFileName, "rb");
        if (ScenarioFile == NULL)
        {
            printf("\nScenario not found.\n");
            return(1);
        }

        //Scenario size
        fseek(ScenarioFile, 0L, SEEK_END);
        ScenarioSize = ftell(ScenarioFile);
        fseek(ScenarioFile, 0L, SEEK_SET);

        //Read scenario
        ScenarioData = malloc(ScenarioSize * sizeof(char));
        fread(ScenarioData, sizeof(char), ScenarioSize, ScenarioFile);
        fclose(ScenarioFile);

        //Get full size
        FullSize = EncodedFileSize + ScenarioSize;
        //Add to cumulative size
        RouteBlockSize += FullSize;

        //Allocate full data
        CompleteData = malloc(FullSize * sizeof(char));
        memcpy(CompleteData, ScenarioData, ScenarioSize);
        memcpy(CompleteData + ScenarioSize, SourceData, EncodedFileSize);

        //Encrypt
        for (SDPosition = 0, KeyPosition = 0; SDPosition < FullSize; SDPosition++, KeyPosition++)
        	CompleteData[SDPosition] = (CompleteData[SDPosition] + Key[KeyPosition % 11]) & 0xFF;

        RouteBlock = (unsigned char *)realloc(RouteBlock, RouteBlockSize);
        memcpy(RouteBlock + RouteBlockSize - FullSize, CompleteData, FullSize);

        //Get property position and head of the next file
        PropertyPosition = LastPosition;
        if (PropertyPosition == 0x00000000)
        {
            PropertyPosition = 0x0011912A;
        }
        HeadOfNextFile = PropertyPosition + FullSize;

        //Write to property array
        memcpy(PropertyData, &FileNumber, 4);
        memcpy(PropertyData + 4, &EmptyBytes, 4);
        memcpy(PropertyData + 8, &SCNString, 4);
        memcpy(PropertyData + 12, &PropertyPosition, 4);
        memcpy(PropertyData + 16, &FullSize, 4);
        memcpy(PropertyData + 20, &HeadOfNextFile, 4);

        //Save last position
        LastPosition = HeadOfNextFile;

        //Write property array to property block
        memcpy(PropertyBlock + (SceneCounter * 24), PropertyData, 24);

        //Write property block and route block to LVNS3.PAK when done
        if (strcmp(FileNumber, "02CD") == 0)
        {
            //Get LVNS3PAK data
            LVNS3PAK = fopen("LVNS3SCNB.PAK", "rb");
            fread(LVNS3Data, sizeof(char), 0x11EE42, LVNS3PAK);
            fclose(LVNS3PAK);

            LVNS3Data = (unsigned char *)realloc(LVNS3Data, 0x11EE42 + RouteBlockSize);
            memcpy(LVNS3Data + 0x11912A, RouteBlock, RouteBlockSize);

            //Open table file
            TableFile = fopen("TABLE.TABLE", "rb+");
            fread(TableData, sizeof(char), 0x5D18, TableFile);
            //Write property block to table
            memcpy(TableData, PropertyBlock, 0x24D8);
			fseek(TableFile, 0L, SEEK_SET);
			fwrite(TableData, sizeof(char), 0x5D18, TableFile);

            //Encrypt table
            for (SDPosition = 0, KeyPosition = 0; SDPosition < 0x5D18; SDPosition++, KeyPosition++)
            {
                TableData[SDPosition] = (TableData[SDPosition] + Key[KeyPosition % 11]) & 0xFF;
            }

            LVNS3Data = (unsigned char *)realloc(LVNS3Data, 0x11EE42 + RouteBlockSize + 0x5D18);

            //Copy table
            memcpy(LVNS3Data + 0x11912A + RouteBlockSize, TableData, 0x5D18);

            //Write brand new LVNS3PAK
            NEWLVNS3PAK = fopen("LVNS3SCN.PAK", "wb");
            fwrite(LVNS3Data, sizeof(char), 0x11EE42 + RouteBlockSize, NEWLVNS3PAK);
            fclose(NEWLVNS3PAK);
        }
    }

    return(0);
}

void DecryptFile()
{
	FILE *DecryptedFile;
    sprintf(OutputFileName, "%s_Decrypted.DECRYPTED", InputFileName);
    DecryptedFile = fopen(OutputFileName, "wb");

    char Key[KEYLEN] = {0xd1, 0x58, 0x6a, 0x56, 0x9a, 0x13, 0xa5, 0xf7, 0x7c, 0x3e, 0x74};
    int KeyPosition;
    int SDPosition;

    unsigned char *SourceData;
    fseek(InputFile, 0L, SEEK_END);
    InputFileSize = ftell(InputFile);
    fseek(InputFile, 0L, SEEK_SET);
    SourceData = malloc(InputFileSize * sizeof(char));
    fread(SourceData, sizeof(char), InputFileSize, InputFile);

    for (SDPosition = 0, KeyPosition = 0; SDPosition < InputFileSize; SDPosition++, KeyPosition++)
    {
        SourceData[SDPosition] = (SourceData[SDPosition] - Key[KeyPosition % 11]) & 0xFF;
    }

    fwrite(SourceData, sizeof(char), InputFileSize, DecryptedFile);

    return;
}

void DecryptFileShizuku()
{
	FILE *DecryptedFile;
    sprintf(OutputFileName, "%s_Decrypted.DECRYPTED", InputFileName);
    DecryptedFile = fopen(OutputFileName, "wb");

    char Key[KEYLEN] = {0x71, 0x48, 0x6a, 0x55, 0x9f, 0x13, 0x58, 0xf7, 0xd1, 0x7c, 0x3e};
    int KeyPosition;
    int SDPosition;

    unsigned char *SourceData;
    fseek(InputFile, 0L, SEEK_END);
    InputFileSize = ftell(InputFile);
    fseek(InputFile, 0L, SEEK_SET);
    SourceData = malloc(InputFileSize * sizeof(char));
    fread(SourceData, sizeof(char), InputFileSize, InputFile);

    for (SDPosition = 8, KeyPosition = 0; SDPosition < InputFileSize; SDPosition++, KeyPosition++)
    {
        SourceData[SDPosition] = (SourceData[SDPosition] - Key[KeyPosition % 11]) & 0xFF;
    }

    fwrite(SourceData, sizeof(char), InputFileSize, DecryptedFile);

    return;
}

void DoNot()
{
    FILE *NotedFile;
    sprintf(OutputFileName, "%s.NOTED", InputFileName);
    NotedFile = fopen(OutputFileName, "wb");

    int SDPosition;
    unsigned char NotedByte;

    unsigned char *SourceData;
    fseek(InputFile, 0L, SEEK_END);
    InputFileSize = ftell(InputFile);
    fseek(InputFile, 0L, SEEK_SET);
    SourceData = malloc(InputFileSize * sizeof(char));
    fread(SourceData, sizeof(char), InputFileSize, InputFile);

    for (SDPosition = 0; SDPosition < InputFileSize; SDPosition++)
    {
        NotedByte = ~(SourceData[SDPosition]);
        SourceData[SDPosition] = NotedByte;
        fwrite(&NotedByte, sizeof(char), 1, NotedFile);
    }
    fclose(NotedFile);
    return;
}

int MergeScenarioAndText()
{
    unsigned char PlaceHolder;
    int ScenarioSize;
    int CompleteFileSize;

    //Scenario file
    FILE *ScenarioFile;
    sprintf(OutputFileName, "%s_SCENARIO.scn", InputFileName);
    ScenarioFile = fopen(OutputFileName, "rb");
    if (ScenarioFile == NULL)
    {
        printf("\nScenario not found.\n");
        return(1);
    }

    //Scenario size
    fseek(ScenarioFile, 0L, SEEK_END);
    ScenarioSize = ftell(ScenarioFile);
    fseek(ScenarioFile, 0L, SEEK_SET);

    //File ready to be encrypted
    sprintf(OutputFileName, "%s.scn", InputFileName);
    CompleteFile = fopen(OutputFileName, "wb");

    //Write scenario
    while (ScenarioSize-- != 0x00)
    {
    PlaceHolder = fgetc(ScenarioFile);
    fwrite(&PlaceHolder, sizeof(char), 1, CompleteFile);
    }

    //Skip cursor right after scenario file
    fseek(CompleteFile, ScenarioSize, SEEK_SET);

    //Write text
    fwrite(SourceData, sizeof(char), InputFileSize, CompleteFile);

    //Size of complete file
    fseek(CompleteFile, 0L, SEEK_END);
    CompleteFileSize = ftell(CompleteFile);
    fseek(CompleteFile, 0L, SEEK_SET);

    //Writing property file
    //Create property file
    FILE *PropertyFile;
    char PropertyFileName[20];
    sprintf(PropertyFileName, "%s_Property.PROPERTY", InputFileName);
    PropertyFile = fopen(PropertyFileName, "wb");

    //Create scratch with position data
    FILE *LastPositionScratch;
    LastPositionScratch = fopen("LastPositionScratch.SCRATCH", "rb");

    //Variables
    char EmptyBytes[4] = {0x20, 0x20, 0x20, 0x20};
    char SCNString[4] = {0x53, 0x43, 0x4E, 0x00};
    char VeryEmptyBytes[4] = {0x00, 0x00, 0x00, 0x00};
    long PropertyPosition;

    //Get property position and head of the next file
    fread(&PropertyPosition, 4, 1, LastPositionScratch);
    if (PropertyPosition == 0x00000000)
    {
        PropertyPosition = 0x0011912A;
    }
    long HeadOfNextFile = PropertyPosition + CompleteFileSize;

    //Write to property file
    fwrite(&InputFileName, 4, 1, PropertyFile);
    fwrite(&EmptyBytes, 4, 1, PropertyFile);
    fwrite(&SCNString, 4, 1, PropertyFile);
    fwrite(&PropertyPosition, 4, 1, PropertyFile);
    fwrite(&CompleteFileSize, 4, 1, PropertyFile);
    fwrite(&HeadOfNextFile, 4, 1, PropertyFile);

    //Write last position to scratch
    fclose(LastPositionScratch);
    LastPositionScratch = fopen("LastPositionScratch.SCRATCH", "wb");
    fwrite(&HeadOfNextFile, 4, 1, LastPositionScratch);

    //Reset scratch when done
    if (strcmp(InputFileName, "01CF") == 0)
    {
        fseek(LastPositionScratch, 0L, SEEK_SET);
        fwrite(&VeryEmptyBytes, 4, 1, LastPositionScratch);
    }

    //Create property block
    if (PropertyPosition == 0x0011912A)
    {
        remove("PropertyBlock.BLOCK");
    }
    char PropertyFileBuffer[0x18];
    FILE *PropertyBlock;
    PropertyBlock = fopen("PropertyBlock.BLOCK", "ab");
    fclose(PropertyFile);
    PropertyFile = fopen(PropertyFileName, "rb");
    while(fread(&PropertyFileBuffer, sizeof(PropertyFileBuffer), 1, PropertyFile))
    {
        fwrite(&PropertyFileBuffer, sizeof(PropertyFileBuffer), 1, PropertyBlock);
    }

    //Write property block to table when done
    if (strcmp(InputFileName, "01CF") == 0)
    {
        FILE *TableFile;
        TableFile = fopen("TABLE.TABLE", "rb+");
        char RouteTableBuffer[0x0D98];
        fclose(PropertyBlock);
        PropertyBlock = fopen("PropertyBlock.BLOCK", "rb");

        fseek(TableFile, 0x0E40, SEEK_SET);
        while(fread(&RouteTableBuffer, sizeof(RouteTableBuffer), 1, PropertyBlock))
        {
            fwrite(&RouteTableBuffer, sizeof(RouteTableBuffer), 1, TableFile);
        }
    }

    fclose(ScenarioFile);
    fclose(CompleteFile);

    return(0);
}

//Using "cut" method
int Autolinebreaking()
{
	//IMPORTANT IMPORTANT IMPORTANT
	//DON'T LEAVE "ANY" EXTRA LINE (LIKE AN EMPTY LINE)
	//YOU'LL MESS UP LINE COUNT
	//THANK YOU
	//Akari variables
	int SceneNumber = 393;
	char AkariFileList[393][20] =  {
									//Morning scenes
									"0000", "0001", "0002", "0003", "0004", "0005", "0020", "0021", "0033", "0034",
									"0035", "0036", "0037", "0038", "0039", "003A", "003B", "003C", "003D", "003E",
									"003F", "0040", "0041", "0042", "0043", "0044", "0045", "0046", "0047", "0048",
									"0049", "0057", "0058", "0059", "005A", "005B", "005C", "005D", "005E", "005F",
									"0060", "0061", "0062", "0063", "0064", "0065", "0066", "0067", "0068", "0069",
									"006A", "006B", "006C", "006D", "006E", "006F", "0070", "0071", "0072", "0080",
									"0081", "0082", "0083", "0084", "0085", "0086", "0087", "0088", "0089", "008A",
									"008B", "008C", "008D", "008E", "008F", "0090", "0091", "0092", "0093", "0094",
									"0095", "0096", "0097", "0098", "0099", "009A", "00A0", "00A1", "00A2", "00A3",
									"00A4", "00A5", "00A6", "00A7", "00A8", "00A9", "00AA", "00AB", "00AC", "00AD",
									"00AE", "00AF", "00B0", "00B1", "00B2", "00B3", "00B4", "00B5", "00B6", "00B7",
									"00B8", "00B9", "00BA", "00BB", "00BC", "00BD", "00BE", "00BF", "00C0", "00C1",
									"00C2", "00C3", "00C4", "00C5", "00C6", "00C7", "00C8", "00C9", "00CA", "00CB",
									"00CC", "00CD", "00CE", "00CF", "00D0", "00D1", "00D2", "00D3", "00D4", "00D5",
									"00D6", "00D7", "00D8", "00D9", "00DA", "00DB", "00DC", "00DD", "00F0", "00F1",
									"00F2", "00F3",
									//Akari scenes
									"0100", "0101", "0102", "0103", "0104", "0105", "0106", "0107", "0108", "0109",
									"010A", "0110", "0120", "0121", "0122", "0123", "0124", "0125", "0126", "0127",
									"0128", "0129", "012A", "012B", "012C", "0140", "0141", "0142", "0143", "0144",
									"0145", "0146", "0147", "0148", "0149", "014A", "014B", "014C", "014D", "014E",
									"014F", "0150", "0151", "0152", "0153", "0154", "0155", "0156", "0157", "0158",
									"0159", "015A", "015B", "015C", "015D", "015E", "015F", "0160", "0161", "0162",
									"0163", "0164", "0165", "0166", "0167", "0180", "0181", "0182", "0183", "0184",
									"0185", "0186", "0187", "0188", "0189", "018A", "018B", "018C", "018D", "018E",
									"018F", "0190", "0191", "0192", "0193", "0194", "0195", "0196", "0197", "0198",
									"0199", "019A", "019B", "019C", "019D", "019E", "019F", "01A0", "01A1", "01A2",
									"01A3", "01A4", "01A5", "01A6", "01A7", "01A8", "01A9", "01AA", "01AB", "01AC",
									"01AD", "01AE", "01AF", "01B0", "01B1", "01B2", "01B3", "01B4", "01B5", "01B6",
									"01B7", "01B8", "01B9", "01BA", "01BB", "01BC", "01BD", "01BE", "01BF", "01C0",
									"01C1", "01C2", "01C3", "01C4", "01C5", "01C6", "01C7", "01C8", "01C9", "01CA",
									"01CB", "01CC", "01CD", "01CE", "01CF",
									//Serika scenes
									"0200", "0201", "0210", "0220", "0221", "0222", "0223", "0224", "0241", "0242",
									"0243", "0244", "0245", "0246", "0247", "0260", "0261", "0262", "0280", "0281",
									"0282", "0283", "0284", "0285", "0286", "0287", "0288", "0289", "028A", "028B",
									"028C", "028D", "028E", "028F", "0290", "0291", "0292", "0293", "0294", "0295",
									"0296", "0297", "0298", "0299", "029A", "029B", "029C", "029D", "029E", "029F",
									"02A0", "02A1", "02A2", "02A3", "02A4", "02A5", "02A6", "02A7", "02A8", "02A9",
									"02AA", "02AB", "02AC", "02AD", "02AE", "02AF", "02B0", "02B1", "02B2", "02B3",
									"02B4", "02B5", "02B6", "02B7", "02B8", "02B9", "02BA", "02BB", "02BC", "02BD",
									"02BE", "02BF", "02C0", "02C1", "02C2", "02C3", "02C4", "02C5", "02C6", "02C7",
									"02C8", "02C9", "02CA", "02CB", "02CC", "02CD"
									};

	//Filename arrays
	char FileNumber[100];
	char SceneFileName[100];
	char OutputSceneFileName[100];
	//File pointers
	FILE *InputText;
	FILE *OutputText;
	//General variables
	int SceneCounter = 0;
	char Catcher = '0';
	//Arrays
	char LineData[0xFFF];
	char LineDataReadOnly[0xFFF];
	//Array indexes
	int LDPosition = 0;
	int LDRPosition = 0;
	int LDTempPosition = 0;
	//Counters
	int Lines = 0;
	int NewLineCounter = 0;

	//Main loop
	for (SceneCounter = 0; SceneCounter < SceneNumber; SceneCounter++)
	{
		//Get a file
		sprintf(FileNumber, "%s", AkariFileList + SceneCounter);
		sprintf(SceneFileName, "%s_TEXT_TRANSLATED.TXT", FileNumber);
		InputText = fopen(SceneFileName, "r");
		//Exception handler
		if (InputText == NULL)
		{
			printf("\nText file doesn't exist: %s.\n", SceneFileName);
			return(1);
		}
		//Create output file
		sprintf(OutputSceneFileName, "%s_TEXT_TRANSLATED_AUTOLINEBROKEN.AUTOLINEBROKEN", FileNumber);
		OutputText = fopen(OutputSceneFileName, "w");
		//Exception handler
		if (OutputText == NULL)
		{
			printf("\nCan't create output file.\n");
			return(1);
		}
		printf("\nAutolinebreaking: %s.\n", OutputSceneFileName);
		//Get lines
		while ((Catcher = fgetc(InputText)) != EOF)
		{
			if (Catcher == '\n')
			{
				Lines++;
			}
		}
		//Include last line
		Lines++;
		//Move cursor to beginning
		fseek(InputText, 0L, SEEK_SET);
		//Copy loop
		while (Lines--)
		{
			//Get line
			fgets(LineData, 0xFFF, InputText);
			//Get read only line
			strncpy(LineDataReadOnly, LineData, 0xFFF);
			//Skip scene 0180
			if (strcmp(FileNumber, "0180") == 0)
			{
				goto WRITE_LINE;
			}
			//Check if "Text" string
			if (strncmp(LineDataReadOnly, "Text", 4) == 0)
			{
				//Starting after "Text"
				LDRPosition = 6;
				LDPosition = 6;
				while (LineDataReadOnly[LDRPosition] != '\n')
				{
					LineData[LDPosition] = LineDataReadOnly[LDRPosition];
					if (LineData[LDPosition] == '@')
					{
						NewLineCounter--;
					}
					if (NewLineCounter == 53)
					{
						//Check exceptions
						if (LineData[LDPosition] == ' ')
						{
							//Semi-perfect line
							LineData[LDPosition] = '>';
							NewLineCounter = 0;
							goto LOOP_END;
						}

						NewLineCounter = 0;
						LDTempPosition = LDPosition;
						while (LineData[LDTempPosition] != ' ')
						{
							LDTempPosition--;
							NewLineCounter++;
						}
						if (LDTempPosition <= 6)
						{
							goto LOOP_END;
						}

						LineData[LDTempPosition] = '>';
					}
					LOOP_END:
					LDPosition++;
					LDRPosition++;
					NewLineCounter++;
				}
				//Null terminated string
				LineData[LDPosition] = '\n';
				LDPosition++;
				LineData[LDPosition] = '\0';
				NewLineCounter = 0;
			}
			//Write line
			WRITE_LINE:
			fputs(LineData, OutputText);
		}
		//We're done with this file
		fclose(InputText);
		fclose(OutputText);
		Lines = 0;
	}
	return(0);
}

int AutolinebreakingScript()
{
	FILE *InputText;
	FILE *OutputText;

	char Catcher = '0';
	char LineData[0xFFF];
	char LineDataReadOnly[0xFFF];
	int LDPosition = 0;
	int LDRPosition = 0;
	int LDTempPosition = 0;
	int Lines = 0;
	int NewLineCounter = 0;

	InputText = fopen(SupraInputFileName, "r");

	if (InputText == NULL)
	{
		printf("\n%s", SupraInputFileName);
		printf("\nText file doesn't exist.\n");
		return(1);
	}

	OutputText = fopen("ScriptAutolinebroken.txt", "w");

	if (OutputText == NULL)
	{
		printf("\nCan't create output file.\n");
		return(1);
	}

	while ((Catcher = fgetc(InputText)) != EOF)
	{
		if (Catcher == '\n')
		{
			Lines++;
		}
	}

	//include last line
	Lines++;

	fseek(InputText, 0L, SEEK_SET);

	while (Lines--)
	{
		//Get line
		fgets(LineData, 0xFFF, InputText);
		//Get read only line
		strncpy(LineDataReadOnly, LineData, 0xFFF);

		//Text Handler
		if (strncmp(LineDataReadOnly, "`", 1) == 0)
		{
			//Start after `
			LDRPosition = 2;
			LDPosition = 2;

			while (LineDataReadOnly[LDRPosition] != '\n')
			{
				LineData[LDPosition] = LineDataReadOnly[LDRPosition];
				if (LineData[LDPosition] == '@')
				{
					NewLineCounter--;
				}
				//53 for normal font, 42 for CT font
				if (NewLineCounter == 84)
				{
					//Check exceptions
					if (LineData[LDPosition] == ' ')
					{
						//Semi-perfect line
						LineData[LDPosition] = '>';
						NewLineCounter = 0;
						goto LOOP_END;
					}

					NewLineCounter = 0;
					LDTempPosition = LDPosition;
					while (LineData[LDTempPosition] != ' ')
					{
						LDTempPosition--;
						NewLineCounter++;

//						if(	LineData[LDTempPosition] == 'i' ||
//							LineData[LDTempPosition] == 'l' ||
//							LineData[LDTempPosition] == '\''||
//							LineData[LDTempPosition] == '.' ||
//							LineData[LDTempPosition] == ',' ||
//							LineData[LDTempPosition] == '"')
//						{
//							NewLineCounter += 1;
//						}
//						else
//						{
//							NewLineCounter += 2;
//						}

					}
					if (LDTempPosition <= 2)
					{
						goto LOOP_END;
					}

					LineData[LDTempPosition] = '>';
				}
				LOOP_END:
				LDPosition++;
				LDRPosition++;

				if(LineData[LDPosition] == 0x20 ||
					LineData[LDPosition] == 'i' ||
					LineData[LDPosition] == 'l' ||
					LineData[LDPosition] == '\''||
					LineData[LDPosition] == '.' ||
					LineData[LDPosition] == ',' ||
					LineData[LDPosition] == '"')
				{
					NewLineCounter += 1;
				}
				else
				{
					NewLineCounter += 2;
				}
			}
			//Null terminated string
			LineData[LDPosition] = '\n';
			LDPosition++;
			LineData[LDPosition] = '\0';
			NewLineCounter = 0;
		}

		fputs(LineData, OutputText); //write line
	}

	fclose(InputText);
	fclose(OutputText);
	return(0);
}

int get_width(char i)
{
	int width;

	if (i == '|')
	{
		width = 12;
	}
	else if (i == 'j' || i == ':' || i == ';' || i == 'i' || i == 'l' || i =='!' || i == 0x20 || i == '.' || i == ',' || i == '\'')
	{
		width = 6;
	}
	else if (i == '1' || i == '(' || i == ')')
	{
		width = 8;
	}
	else if (i == '*' || i == '"')
	{
		width = 9;
	}
	else if (i == 't' || i == 'I' || i == '/')
	{
		width = 10;
	}
	else if (i == 'f')
	{
		width = 11;
	}
	else if (i == 'c' || i == 'r' || i == 's' || i == 'E' || i == 'F' || i == 'L' || i == 'S')
	{
		width = 12;
	}
	else if (i == 'T')
	{
		width = 13;
	}
	else if (i == '-' || i == 'N' || i == 'K' || i == '4')
	{
		width = 16;
	}
	else if (i == 'M' || i == '%')
	{
		width = 18;
	}
	else if (i == 'm' || i == 'w' || i == 'W')
	{
		width = 22;
	}
	else
	{
		width = 14;
	}
	return width;
}

int get_len(char letter)
{
	int len;

	if(
	letter == '_' ||
	letter == '.' ||
	letter == ',' ||
	letter == '*' ||
	letter == '!' ||
	letter == '"'
	)
	{
		len = 1;
	}
	else
	{
		len = 2;
	}

	return len;
}

int get_remainder(int width)
{
	int remainder;

	remainder = width % 8;
	if (remainder != 0)
	{
		remainder = 1;
	}

	return remainder;
}

int get_remainder_th(int pixel_count)
{
	int remainder;

	remainder = pixel_count % 8;
	if (remainder != 0)
	{
		remainder = 1;
	}

	return remainder;
}


int spacing_toheart()
{
	FILE *InputText;

	char line[0xFFF];
	char letter = '0';
	int pos = 0;
	int lines = 0;
	int x_offset = 0;

	int last_line_pos = 0;
	char next_line[0xFFF];

	InputText = fopen(SupraInputFileName, "r");

	if (InputText == NULL)
	{
		printf("\nFile %s doesn't exist.\n", SupraInputFileName);
		return(1);
	}

	while ((letter = fgetc(InputText)) != EOF)
	{
		if (letter == '\n')
		{
			lines++;
		}
	}

	lines++; //include last line

	fseek(InputText, 0L, SEEK_SET);

	while (lines--)
	{
		fgets(line, 0xFFF, InputText); //get line

		if(strncmp(line, "` ", 2) != 0) //ignore non-text lines
		{
			printf("%s", line); //print line as it is
		}
		else
		{
			printf("` ");

			pos = 2;
			x_offset = 2;

			while (line[pos] != '\n')
			{
				letter = line[pos];
				x_offset += 2;

				if(letter == '@' || letter == '|')
				{
					printf("%c", letter);
					x_offset -= 2;
					pos++;
					continue;
				}

				if(letter == '>')
				{
					printf("\nSetTextHOffset 02\n` ");
					x_offset = 2;
					pos++;
					continue;
				}

				if(letter == 0x20)
				{
					x_offset--;
					printf("\nCancelNewLine\nSetTextHOffset %02X\n` ", x_offset);
					pos++;
					continue;
				}

				if(letter == 'l' ||
					letter == 'i' ||
					letter == '!' ||
					letter == '.' ||
					letter == ',' ||
					letter == '\'' ||
					letter == '"')
				{
					x_offset -= 2;
					printf("\nCancelNewLine\nSetTextHOffset %02X\n` %c", x_offset, letter);
					x_offset++;
					printf("\nCancelNewLine\nSetTextHOffset %02X\n` ", x_offset);
					pos++;
					continue;
				}

				printf("%c", letter);
				pos++;
			}

			last_line_pos = ftell(InputText);
			fgets(next_line, 0xFFF, InputText);
			next_line[strcspn(next_line, "\n")] = 0; //remove newline
			if ((strncmp(next_line, "WaitPage", 8) != 0)) //cancel newline if next opcode is waitpage
			{
				printf("\nSetTextHOffset 02\n");
				goto SKIP_NEWLINE;
			}
			printf("\n");
			SKIP_NEWLINE:
			fseek(InputText, last_line_pos, SEEK_SET); //restore last position


			x_offset = 0;
			letter = '0';
		}
	}

	fclose(InputText);
	return(0);
}

int mono_toheart()
{
	FILE *InputText;

	char line[0xFFF];
	char letter = '0';
	int pos = 0;
	int lines = 0;
	int len = 0;
	int total_len = 0;
	char ahead_letter = '0';
	int ahead_pos = 0;
	int ahead_len = 0;

	InputText = fopen(SupraInputFileName, "r");

	if (InputText == NULL)
	{
		printf("\nFile %s doesn't exist.\n", SupraInputFileName);
		return(1);
	}

	while ((letter = fgetc(InputText)) != EOF)
	{
		if (letter == '\n')
		{
			lines++;
		}
	}

	lines++; //include last line

	fseek(InputText, 0L, SEEK_SET);

	while (lines--)
	{
		fgets(line, 0xFFF, InputText); //get line

		if(strncmp(line, "` ", 2) != 0) //ignore non-text lines
		{
			printf("%s", line); //print line as it is
		}
		else
		{
			total_len = 2;

			while(line[pos] != '\n')
			{
				letter = line[pos];

				if(letter == '|')
				{
					printf("%c", letter);
					pos++;
					continue;
				}

				if(letter == '@')
				{
					printf("%c", letter);
					pos++;
					continue;
				}

				if(
				letter == 0x20 ||
				letter == '.' ||
				letter == ',' ||
				letter == '*' ||
				letter == '!' ||
				letter == '\'' ||
				letter == '"' ||
				letter == 'i' ||
				letter == 'l'
				)
				{
					len = 1;
				}
				else
				{
					len = 2;
				}

				total_len += len;

				if(letter == 0x20)
				{
					letter = ' ';

					ahead_pos = pos;
					ahead_pos++;

					while (line[ahead_pos] != ' ')
					{
						ahead_letter = line[ahead_pos];

						if (ahead_letter == '\n')
						{
							break;
						}

						ahead_len += get_len_th(ahead_letter);
						ahead_pos++;
					}

					if(ahead_len + total_len >= 79)
					{
						printf(">");
						total_len = 0;
						goto ESCAPE;
					}

					ahead_pos = 0;
					ahead_letter = '0';
					ahead_len = 0;
				}

				printf("%c", letter);
				ESCAPE:;
				pos++;
			}
			printf("\n");
		}
		total_len = 0;
		ahead_pos = 0;
		ahead_letter = '0';
		ahead_len = 0;
		pos = 0;
	}
	fclose(InputText);
	return(0);
}



int conv_kiz_tbl()
{
	FILE *InputText;

	char line[0xFFF];
	char letter = '0';
	char letter2 = '0';
	char letter3 = '0';
	char letter4 = '0';
	int pos = 0;
	int lines = 0;

	InputText = fopen(SupraInputFileName, "r");

	if (InputText == NULL)
	{
		printf("\nFile %s doesn't exist.\n", SupraInputFileName);
		return(1);
	}

	while ((letter = fgetc(InputText)) != EOF)
	{
		if (letter == '\n')
		{
			lines++;
		}
	}

	lines++; //include last line

	fseek(InputText, 0L, SEEK_SET);

	while (lines--)
	{
		fgets(line, 0xFFF, InputText); //get line

			letter = line[pos]; pos++;
			letter2 = line[pos]; pos++;
			letter3 = line[pos]; pos++;
			letter4 = line[pos]; pos++;

			printf("%C%C%C%C%s",
					letter3, letter4,
					letter, letter2, line + 4);
		pos = 0;
	}
	fclose(InputText);
	return(0);
}

int mono_shizuku()
{
	FILE *InputText;

	char line[0xFFF];
	char letter = '0';
	int pos = 0;
	int lines = 0;
	int len = 0;
	int total_len = 0;
	int prev_letter = 0;
	char ahead_letter = '0';
	int ahead_pos = 0;
	int ahead_len = 0;

	InputText = fopen(SupraInputFileName, "r");

	if (InputText == NULL)
	{
		printf("\nFile %s doesn't exist.\n", SupraInputFileName);
		return(1);
	}

	while ((letter = fgetc(InputText)) != EOF)
	{
		if (letter == '\n')
		{
			lines++;
		}
	}

	lines++; //include last line

	fseek(InputText, 0L, SEEK_SET);

	while (lines--)
	{
		fgets(line, 0xFFF, InputText); //get line

		while (line[pos] != '\n')
		{
			letter = line[pos];

			if(letter == '#')
			{
				printf("{XPOS:64}\n#\n");
				pos++;
				continue;
			}

			if(letter == '@')
			{
				printf("{__mono}{len2}\n{@}{len1}\n");
				pos++;
				continue;
			}

			if(pos == 0)
			{
				len = 2;
			}
			else if(
			prev_letter == '_' ||
			prev_letter == '.' ||
			prev_letter == ',' ||
			prev_letter == '*' ||
			prev_letter == '!' ||
			prev_letter == '\'' ||
			prev_letter == '"'
			)
			{
				len = 1;
			}
			else
			{
				len = 2;
			}

			total_len += len;

			if(letter == 0x20)
			{
				letter = '_';

				ahead_pos = pos;
				ahead_pos++;

				while (line[ahead_pos] != ' ')
				{
					ahead_letter = line[ahead_pos];

					if (ahead_letter == '\n')
					{
						break;
					}

					ahead_len += get_len(ahead_letter);
					ahead_pos++;
				}

				if(ahead_len + total_len >= 75)
				{
					printf(">\n");
					total_len = 0;
					goto ESCAPE;
				}

				ahead_pos = 0;
				ahead_letter = '0';
				ahead_len = 0;
			}

			printf("{%c_mono}{len%d}\n", letter, len);
			ESCAPE:;
			prev_letter = letter;
			pos++;
		}
		total_len = 0;
		ahead_pos = 0;
		ahead_letter = '0';
		ahead_len = 0;
		printf("{__mono}{len2}\n@>\n");
		pos = 0;
	}
	fclose(InputText);
	return(0);
}

int autolinebreak_shizuku()
{
	FILE *InputText;

	char line[0xFFF];
	char letter = '0';
	char ahead_letter = '0';
	int pos = 0;
	int ahead_pos = 0;
	int lines = 0;
	int total_width = 0;
	int ahead_width = 0;

	InputText = fopen(SupraInputFileName, "r");

	if (InputText == NULL)
	{
		printf("\nFile %s doesn't exist.\n", SupraInputFileName);
		return(1);
	}

	while ((letter = fgetc(InputText)) != EOF)
	{
		if (letter == '\n')
		{
			lines++;
		}
	}

	lines++; //include last line

	fseek(InputText, 0L, SEEK_SET);

	while (lines--)
	{
		fgets(line, 0xFFF, InputText); //get line

		if(strcmp(line, "\n") == 0 || strcmp(line,"\r\n") == 0 || strncmp(line, "{", 1) == 0) //ignore empty line and opcodes
		{
			printf("%s", line); //print line as it is
		}

		else
		{
			while (line[pos] != '\n')
			{
				letter = line[pos];

				if (letter == '>')
				{
					total_width = 0;
					printf("%c", letter);
					pos++;
					continue;
				}

				if (letter == '@' || letter == '#')
				{
					printf("%c", letter);
					pos++;
					continue;
				}

				total_width += get_width(letter);

				if (letter == 0x20)
				{
					ahead_pos = pos;
					ahead_pos++;

					while (line[ahead_pos] != ' ')
					{
						ahead_letter = line[ahead_pos];

						if (ahead_letter == '\n')
						{
							break;
						}

						ahead_width += get_width(ahead_letter);
						ahead_pos++;
					}

					if ((total_width + ahead_width) >= 608)
					{
						letter = '>';
						total_width = 0;
					}

					ahead_pos = 0;
					ahead_letter = '0';
					ahead_width = 0;
				}
				printf("%c", letter);
				pos++;
			}
			printf("\n");
			pos = 0;
			total_width = 0;
		}
	}
	fclose(InputText);
	return(0);
}

int text_to_vwf_shizuku()
{
	FILE *InputText;
	char letter = '0';
	char line[0xFFF];
	char len_array[0xFFF];
	char shift_array[0xFFF];
	int line_pos = 0;
	int len_array_pos = 0;
	int shift_array_pos = 0;
	int lines = 0;
	int total_width = 0;
	int total_shift = 0;

	int block_end = 0;
	int remainder = 0;
	int spc_left = 0;
	int shift = 0;
	int prev_block_end = 1;
	int len = 0;
	bool j_mess = false;

	int j;
	int k;

	InputText = fopen(SupraInputFileName, "r");

	if (InputText == NULL)
	{
		printf("\nFile %s doesn't exist.\n", SupraInputFileName);
		return(1);
	}

	while ((letter = fgetc(InputText)) != EOF)
	{
		if (letter == '\n')
		{
			lines++;
		}
	}

	lines++; //include last line
	fseek(InputText, 0L, SEEK_SET);

	while (lines--)
	{
		fgets(line, 0xFFF, InputText); //get line

		if (strcmp(line, "\n") == 0 || strcmp(line,"\r\n") == 0 || strncmp(line, "{", 1) == 0 || strncmp(line, "\\", 1) == 0) //ignore empty line and comments
		{
			printf("%s", line); //print line as it is
		}

		else
		{
			shift_array[0] = 0;
			len_array[0] = 2;

			while (line[line_pos] != '\n')
			{
				letter = line[line_pos];

				len_array_pos++;
				shift_array_pos++;

				if (letter == '@')
				{
					printf("{%c}{len%d}", letter, len);
					line_pos++;
					j = shift_array[shift_array_pos - 1];
					shift_array[shift_array_pos] = j;
					j = 0;
					k = len_array[len_array_pos - 1];
					len_array[len_array_pos] = k;
					k = 0;
					continue;
				}

				if (letter == '#')
				{
					printf("%c", letter);
					line_pos++;
					continue;
				}

				if (letter == '\\')
				{
					printf("%c", letter);
					line_pos++;
					letter = line[line_pos];
					printf("%c", letter);
					line_pos++;
					continue;
				}

				if (letter == '>')
				{
					printf("%c\n", letter);
					line_pos++;
					total_width = 0;
					remainder = 0;
					spc_left = 0;
					shift = 0;
					total_shift = 0;
					len = 0;
					prev_block_end = 1;
					block_end = 0;
					letter = 0;
					len_array_pos = 0;
					shift_array_pos = 0;
					continue;
				}

				total_width += get_width(line[line_pos]);

				remainder = get_remainder(total_width);
				block_end = (total_width / 8) + remainder;

				if (total_width % 8 == 0)
				{
					block_end++;
				}

				len = block_end - prev_block_end;

				if(letter == 'j')
				{
					if(shift_array[shift_array_pos - 1] < 6)
					{
						len++;
					}

					len_array[len_array_pos - 1] = 1;

					if(j_mess == true)
					{
						len_array[len_array_pos - 1] = 0;
					}
				}

				//if(letter == 'j')
				//{
				//	if(j_mess == true)
				//	{
				//		len++;
//						len_array[len_array_pos - 1] += 1;
//					}
//					else
//					{
//						len_array[len_array_pos - 1] = 0;
//					}

//					if(shift_array[shift_array_pos - 1] < 6)
//					{
//						len++;
//						//goto skip_zeroing;
//					}
//					else
//					{
//						len++;
//						len_array[len_array_pos - 1] += 1;
//					}

					//skip_zeroing:;
				//}

				len_array[len_array_pos] = len;
				prev_block_end = block_end;

				spc_left = (block_end * 8) - total_width;
				shift = 8 - spc_left;
				total_shift += shift;

				if(shift == 8)
				{
					shift = 0;
				}

				shift_array[shift_array_pos] = shift;

				if(letter == 'i')
				{
					j_mess = true;
				}
				else
				{
					j_mess = false;
				}

				if(letter == 0x20)
				{
					printf("{space");
				}
				else if(letter == ':')
				{
					printf("{colon");
				}
				else if(letter == ';')
				{
					printf("{semicolon");
				}
				else
				{
					printf("{%c", letter);
				}

				printf("%d}{len%d}\n",
				shift_array[shift_array_pos - 1],
				len_array[len_array_pos - 1]);

				if(
				(letter == 'm' ||
				letter == 'w'||
				letter == 'W')
				&& shift_array[shift_array_pos - 1] >= 5)
				{
					printf("{%c%d_ext}{len3}\n", letter, (shift_array[shift_array_pos - 1]));
					len_array[len_array_pos + 1] = 0;
					len_array_pos++;
				}

				line_pos++;
			}
			printf("\n");
			total_width = 0;
			remainder = 0;
			spc_left = 0;
			shift = 0;
			total_shift = 0;
			len = 0;
			prev_block_end = 1;
			block_end = 0;
			letter = 0;
			line_pos = 0;
			len_array_pos = 0;
			shift_array_pos = 0;
		}
	}
	fclose(InputText); return(0);
}

char *lzss(char *LoadBuffer, int SaveSize, int LoadBufferPosition)
{
    FILE *DecodedFile; sprintf(OutputFileName, "%s_UNPACKED", InputFileName); DecodedFile = fopen(OutputFileName, "wb");

    static char *SaveBuffer;
	SaveBuffer = malloc(SaveSize * sizeof(char));
	int FlagBits					= 0;
    unsigned char Flag				= 0;
    unsigned char ThisRing 			= 0;
	unsigned char ThisRing2 		= 0;
	unsigned char ThisSaveBuffer 	= 0;
	unsigned char ThisLoadBuffer 	= 0;
    int RingPosition				= 0;
    int SaveBufferPosition			= 0;
    int Position					= 0;
	int Lenght						= 0;
    int Data						= 0;
    unsigned char Var1				= 0;
    unsigned char Var2				= 0;
    int Var3						= 0;
    RingPosition 					= 0x0FEE;
	unsigned char Ring[0x1011];
	memset(Ring, 0, sizeof(Ring));

	while (SaveSize > 0)
    {
		//Get flag
        if (FlagBits-- > 0)
        {
            Flag <<= 1;                                 //Shift flag (up to 8 literals between flags)
            printf("\nFlag 1: %02X", Flag);
        }

		else
        {
            //Flag 2 (a byte in compressed file) determines the number of literals before DATA
            //Flags keep shifting until Flag 2 appears
            Flag = ~(LoadBuffer[LoadBufferPosition++]);
            printf("\nFlag 2: %02X\n", Flag);
            FlagBits = 7;                               //Reset to 0x80
        }

        if (!((Flag & 0x80) == 0)) //If higher than 0x80
        {
            //Data
            ThisLoadBuffer = ~(LoadBuffer[LoadBufferPosition++]);
            ThisRing = ThisSaveBuffer = ThisLoadBuffer;
            Ring[RingPosition++] = ThisRing;
            SaveBuffer[SaveBufferPosition++] = ThisSaveBuffer;

            printf("\nThisLoadBuffer: %02X", ThisLoadBuffer);
            printf("\nThisRing: %02X", ThisRing);
            printf("\nThisSaveBuffer: %02X", ThisSaveBuffer);
            printf("\nRingPosition: %02X", RingPosition);
            printf("\nRing[RingPosition]: %02X", Ring[RingPosition]);
            printf("\nSaveBuffer[SaveBufferPosition]: %02X", SaveBuffer[SaveBufferPosition]);

            RingPosition &= 0x0FFF;
            SaveSize--;
        }

		else
        {
            //Raw data = 00B2FE
            //Data = 00FEB2
            //Lenght = 000005 (3 + 2 from the data)
            //Position = 000FEB

			Var1 = ~(LoadBuffer[LoadBufferPosition]);
			Var2 = ~(LoadBuffer[LoadBufferPosition + 1]);
			Var3 = (Var2 << 8);
			Data = Var1 + Var3;
			printf("\nLoadBuffer[LoadBufferPosition]: %02X", Var1);
            printf("\nLoadBuffer[LoadBufferPosition++]: %02X", Var2);
            printf("\nLoadBuffer[LoadBufferPosition+++]: %02X", Var3);
            printf("\nData = %06X", Data);
            LoadBufferPosition += 2;

            //Lenght
            Lenght = (Data & 0x0F) + 3;
            SaveSize -= Lenght;


            //Last position
            Position = Data >> 4;

            printf("\nData = %06X\nLenght = %06X\nPosition = %06X\nRing position = %06X\n", Data, Lenght, Position, RingPosition);

            //Copy from last position
            while (Lenght-- > 0)
            {
                ThisRing2 = Ring[Position++];
                printf("\nReferenced bytes: %02X", ThisRing2);

                ThisRing = ThisSaveBuffer = ThisRing2;

                SaveBuffer[SaveBufferPosition++] = ThisSaveBuffer;
                Ring[RingPosition++] = ThisRing;

                Position &= 0x0FFF;
                RingPosition &= 0x0FFF;
           }
        }
    }
	fseek(DecodedFile, 0, SEEK_SET);						//Write to LVNS3PAK
	fwrite(SaveBuffer, sizeof(char), SaveSize, DecodedFile);
    return SaveBuffer;
}

char *lzs(char *loadbuf, int size)
{
    static char *savebuf; savebuf = malloc(size * sizeof(char));

	unsigned char ring[0x1000];
    int i, j;
    int c, m;
    int flag;
    int pos, len;

    memset(ring, 0, sizeof(ring));

	for (i = 0, c = 0, m = 0xfee; i < size;)
	{
        if (--c < 0) {
            flag = *loadbuf++;
            c = 7;
        }
        if (flag & 0x80)
        {
            savebuf[i++] = ring[m++] = *loadbuf++;
            m &= 0xfff;
        }
        else
        {
            int data = loadbuf[0] + (loadbuf[1]<<8); loadbuf+=2;

            len = (data & 0x0f) + 3;
            pos = data >> 4;

            for (j = 0; j < len; j++)
            {
                savebuf[i++] = ring[m++] = ring[pos++];
                m &= 0xfff;
                pos &= 0xfff;
            }
        }
        flag = flag << 1;
    }
	return savebuf;
}

void CompileExeText30()
{
	long PointerPosition[160] = {0x0002C794, //Start game
								0x0002C798, //Reset saved data
								0x0002C79C, //Memories album
								0x00001506, //Memories album
								0x0002C7A0, //Change font
								0x0002C7A4, //Exit
								0x0002C7FC, //Save 1
								0x0002C800, //Save 2
								0x0002C804, //Save 3
								0x0002C808, //Save 4
								0x0002C80C, //Save 5
								0x0002C860, //Continue
								0x0002C864, //New game
								0x0002C868, //Bonus 1
								0x0002C86C, //Bonus 2
								0x0002C91C, //Hide text
								0x0002C920, //Load game
								0x0002C924, //Save game
								0x0002C928, //Log
								0x0002C92C, //Change name
								0x0002C930, //Change font
								0x0002C934, //Save in album
								0x0002C938, //Return to main menu
								0x0002C93C, //Go back one choice
								0x0002C940, //Check love points
								0x00012B14, //Check love points
								0x00016DEC, //Data will be loaded
								0x00016EF1, //Data will be saved
								0x00017181, //Going back one choice
								0x0001714D, //Returning to the main menu
								0x00016F30, //Saved successfully
								0x00016FD6, //Adding to album
								0x00017013, //Added to album
								0x00016FC2, //Album is full
								0x0000117C, //Album will be reset
								0x00001215, //Album was reset
								0X0001713C, //Already in album
								0x0000119E, //Are you sure? 1
								0x0000E672, //Are you sure? 2
								0x00019D2F, //Are you sure? 3
								0x0002CA84, //Yes
								0X0002CA88, //No
								0x0002CACC, //MIDI
								0x0002CAD0, //Arranged
								0x0002CAD4, //Mute
								0x0002CB80, //Please select a slot to load from
								0x0002CB84, //Please select a slot to save to
								0x0002CB88, //Please select a slot to reset
								0x000126CD, //All saved data will be reset
								0x0001277E, //All data was reset
								0x0002CBFC, //Read album
								0x0000161C, //Organize album 1
								0x0002CC00, //Organize album 2
								0X0002CC04, //Reset album
								0x0002CC08, //Show date
								0x0002CC0C, //CG completion
								0x0002CC10, //Close album
								0x0002CC74, //Previous picture
								0x0002CC78, //Next picture
								0x0002CC7C, //Select page
								0x0002CC80, //Show date
								0x0002CC84, //Remove from album
								0x0002CC88, //Close album
								0x0002CCC0, //Order by date
								0x0002CCC4, //Order by girl
								0x0002CCC8, //Order by type
								0x00002305, //Album
								0x0000237F, //Page/ 1
								0x00017084, //Page/ 2
								0x000023F4, //Pages in total 1
								0x000170F3, //Pages in total 2
								0X00001685, //Remove duplicated picture?
								0x00001B1C, //Display date?
								0x00002044, //Removing from album
								0X000020A7, //Removed successfully
								0X00001A1C, //Rearranged successfully
								0X0000245F, //Date
								0X000026C3, //Type
								0X0000264F, //Character
								0X00002750, //Event
								0X000026FC, //Visual
								0X0000272C, //H-Scene
								0X000026A6, //Misc 1
								0X00002A6F, //Misc 2
								0X0000222B, //See
								0X000128FC, //Sound test
								0x00012923, //Playing now
								0x00012975, //Selected song
								0x00012943, //Composer/Arranger 1
								0x00012995, //Composer/Arranger 2
								0x0002CE74, //Naoya Shimokawa
								0x0002CE78, //Shinya Ishikawa
								0x0002CE7C, //Kazuhide Nakagami
								0x0002CE80, //Shimokawa / Nakagami
								0x0002CE84, //Takahashi / Ishikawa
								0x0002CEC0, //Previous song
								0x0002CEC4, //Play
								0x0002CEC8, //Stop
								0x0002CECC, //Next song
								0x0002D128, //Leaf
								0x0002D12C, //Brand New Heart
								0x0002D130, //What's your name?
								0x0002D134, //Fresh prairie
								0x0002D138, //Sunshine day
								0x0002D13C, //Riding on the spring wind
								0X0002D140, //Tokimeki Sympathy
								0x0002D144, //Your profile
								0x0002D148, //Elegant lady
								0x0002D14C, //Her depression
								0x0002D150, //My friend
								0x0002D154, //Clear then V-sign
								0x0002D158, //Dreaming robot
								0x0002D15C, //Silent power
								0x0002D160, //Smiling
								0x0002D164, //So delightful!
								0x0002D168, //DoTaBaTa
								0x0002D16C, //Laaameee
								0x0002D170, //Darkening place
								0x0002D174, //Shining
								0x0002D178, //Summoning
								0x0002D17C, //Technical power
								0x0002D180, //Karaoke
								0x0002D184, //Kizuato Cinema
								0x0002D188, //Sebastian
								0x0002D18C, //Development manager Nagase
								0x0002D190, //The secret arts of a witch
								0x0002D194, //Strain
								0x0002D198, //Our afternoon
								0x0002D19C, //Eternal love
								0x0002D1A0, //French kiss
								0x0002D1A4, //Music box
								0x0002D1A8, //Windy good night
								0x0002D1AC, //A new premonition
								0x0002D2B0, //Akari
								0x0002D2B4, //Serika
								0x0002D2B8, //Tomoko
								0x0002D2BC, //Shiho
								0x0002D2C0, //Aoi
								0x0002D2C4, //Multi
								0x0002D2C8, //Kotone
								0x0002D2CC, //Lemmy
								0x0002D2D0, //See album
								0x0002D2D4, //Reset album
								0x0002D308, //Event visual
								0x0002D30C, //H-Scene visual
								0x0002D364, //Sunday
								0x0002D368, //Monday
								0x0002D36C, //Tuesday
								0x0002D370, //Wednesday
								0x0002D374, //Thursday
								0x0002D378, //Friday
								0x0002D37C, //Saturday
								0x000029C5, //CG completion
								0x0002F168, //Surname
								0x0002F16C, //Name
								0x0002F170, //Reading
								0x0002F174, //Nickname
								0x0002F1B0, //Delete
								0x0002F1B4, //Default
								0x0002F1F4, //OK
							   };

	int SpaceSpace	= 0x0000; //opcodes
	int DoublePoint = 0x4400;
	int Bar			= 0x4c00;
	char Nope		= 0x90;
	char EndBlock	= 0xAF;
	char VSPACE		= 0x1A;
	//LETTERS
	int N_STRING = 0x0F00;
	int O_STRING = 0x1000;
	int D_STRING = 0x0500;
	int A_STRING = 0x0200;
	int T_STRING = 0x1500;
	int WEEKDAYX = 0x01D0;
	int DATEX	 = 0x0180;

	long Pointer	 	= 0;
	int AddOn 			= 0;
    int BlockCounter 	= 0;
    int DatafileSize 	= 0;
    int Catcher			= 0;
    int DataIndex		= 0;
    unsigned char* Data;
    FILE *ExeFile;
	ExeFile = fopen("LVNS3_ENG.EXE", "r+b");
	FILE *DataFile;
	DataFile = fopen("EXE_TEXT_TRANSLATED_30.BIN", "rb");

    //Datafile size
    fseek(DataFile, 0L, SEEK_END);
    DatafileSize = ftell(DataFile);
    fseek(DataFile, 0L, SEEK_SET);

    //Read text data
    Data = malloc(DatafileSize * sizeof(char));
    fread(Data, sizeof(char), DatafileSize, DataFile);

	//Write datafile to exe, at the beginning of the kanji table
	fseek(ExeFile, 0x30000, SEEK_SET);
	fwrite(Data, sizeof(char), DatafileSize, ExeFile);
    fseek(ExeFile, 0L, SEEK_SET);

    //Patch western date format
	fseek(ExeFile, 0x0002D1DC, SEEK_SET);
    //Year (unused)
	fwrite(&SpaceSpace, sizeof(char), 2, ExeFile);
    //Month
	fwrite(&Bar, sizeof(char), 2, ExeFile);
    //Day
	fwrite(&SpaceSpace, sizeof(char), 2, ExeFile);
	fseek(ExeFile, 0x0002D1E4, SEEK_SET);
	//Hour
	fwrite(&DoublePoint, sizeof(char), 2, ExeFile);
	//Minute
	fwrite(&SpaceSpace, sizeof(char), 2, ExeFile);
	//Second (unused)
	fwrite(&SpaceSpace, sizeof(char), 2, ExeFile);

	//Context menu date X position
	//0x16D23 - Line 15281 disassembly
	//Album menu date position
	//Weekday
	//X position: 0x1C53
	//Date
	//Y position: 0x1C26
	//X position: 0x1C2B - Line 1567 disassembly

	//Fix album date X position
	fseek(ExeFile, 0x00001C2B, SEEK_SET);
	fwrite(&DATEX, sizeof(char), 2, ExeFile);
	//Fix album weekday X position
	fseek(ExeFile, 0x00001C53, SEEK_SET);
	fwrite(&WEEKDAYX, sizeof(char), 2, ExeFile);
	//1DE16

	//new line vertical spacing
	fseek(ExeFile, 0x0001DDF9, SEEK_SET); //normal text
	fwrite(&VSPACE, sizeof(char), 1, ExeFile);
	fseek(ExeFile, 0x0001DE1A, SEEK_SET);
	fwrite(&VSPACE, sizeof(char), 1, ExeFile);
	fseek(ExeFile, 0x0001F0A5, SEEK_SET); //after returning from menu
	fwrite(&VSPACE, sizeof(char), 1, ExeFile);

	//remove limit for number of letters
	fseek(ExeFile, 0x0001EC44, SEEK_SET); fwrite(&(unsigned char){0xA8}, sizeof(char), 1, ExeFile); //normal text 1
	fseek(ExeFile, 0x0001EC4A, SEEK_SET); fwrite(&(unsigned char){0xA8}, sizeof(char), 1, ExeFile); //normal text 2
	fseek(ExeFile, 0x0001ECE3, SEEK_SET); fwrite(&(unsigned char){0xA8}, sizeof(char), 1, ExeFile); //normal text 3
	fseek(ExeFile, 0x0001F257, SEEK_SET); fwrite(&(unsigned char){0xA8}, sizeof(char), 1, ExeFile); //log text 1
	fseek(ExeFile, 0x0001F262, SEEK_SET); fwrite(&(unsigned char){0xA8}, sizeof(char), 1, ExeFile); //log text 2
	fseek(ExeFile, 0x0001F303, SEEK_SET); fwrite(&(unsigned char){0xA8}, sizeof(char), 1, ExeFile); //log text 3
	fseek(ExeFile, 0x0001F5EA, SEEK_SET); fwrite(&(unsigned char){0xA8}, sizeof(char), 1, ExeFile); //return text 1
	fseek(ExeFile, 0x0001F5F5, SEEK_SET); fwrite(&(unsigned char){0xA8}, sizeof(char), 1, ExeFile); //return text 2
	fseek(ExeFile, 0x0001F67A, SEEK_SET); fwrite(&(unsigned char){0xA8}, sizeof(char), 1, ExeFile); //return text 2

	//"No data" string (no pointer/hardcoded)
	fseek(ExeFile, 0x0002D1EE, SEEK_SET);
	fwrite(&SpaceSpace, sizeof(char), 2, ExeFile);
	fwrite(&N_STRING, sizeof(char), 2, ExeFile);
	fwrite(&O_STRING, sizeof(char), 2, ExeFile);
	fwrite(&SpaceSpace, sizeof(char), 2, ExeFile);
	fwrite(&D_STRING, sizeof(char), 2, ExeFile);
	fwrite(&A_STRING, sizeof(char), 2, ExeFile);
	fwrite(&T_STRING, sizeof(char), 2, ExeFile);
	fwrite(&A_STRING, sizeof(char), 2, ExeFile);
	fwrite(&EndBlock, sizeof(char), 2, ExeFile);

	//Caption
	fseek(ExeFile, 0x0002D544, SEEK_SET);
	fprintf(ExeFile, "ToHeart                                           ");

	//Version information
	fseek(ExeFile, 0x0002D513, SEEK_SET);
	fprintf(ExeFile, "Version info ");

	//Exit message
	fseek(ExeFile, 0x0002D5C0, SEEK_SET);
	fprintf(ExeFile, "Are you sure you want to exit?");

	//No-CD crack
	fseek(ExeFile, 0x0001FB08, SEEK_SET);
	fwrite(&Nope, sizeof(char), 1, ExeFile);
	fwrite(&Nope, sizeof(char), 1, ExeFile);
	fwrite(&Nope, sizeof(char), 1, ExeFile);
	fwrite(&Nope, sizeof(char), 1, ExeFile);
	fwrite(&Nope, sizeof(char), 1, ExeFile);
    fseek(ExeFile, 0L, SEEK_SET);

	//Yeet unneeded text
	fseek(ExeFile, 0x0002F1B9, SEEK_SET); fwrite(&(unsigned char){0xFF}, sizeof(char), 1, ExeFile);
	fseek(ExeFile, 0x0002F1C5, SEEK_SET); fwrite(&(unsigned char){0xFF}, sizeof(char), 1, ExeFile);
	fseek(ExeFile, 0x0002F1D1, SEEK_SET); fwrite(&(unsigned char){0xFF}, sizeof(char), 1, ExeFile);

    //Select name roman letters
	fseek(ExeFile, 0x0002F248, SEEK_SET);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x02}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x03}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x04}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x05}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x06}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x07}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x08}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x09}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x0A}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x0B}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x0C}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x0D}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x0E}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x0F}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x10}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x11}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x12}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x13}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x14}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x15}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x16}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x17}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x18}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x19}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x1A}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x1B}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x40}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x41}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x42}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x43}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x44}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x45}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x46}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x47}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x48}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x49}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x4A}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x4B}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x4C}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x5B}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x1C}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x1D}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x1E}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x1F}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x20}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x21}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x22}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x23}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x24}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x25}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x26}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x27}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x28}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x29}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x2A}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x2B}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x2C}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x2D}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x2E}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x2F}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x30}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x31}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x32}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x33}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x34}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x35}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x36}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x37}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x38}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x39}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x3A}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x3B}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x3C}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x3D}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x3E}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x3F}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fwrite(&(unsigned char){0x00}, sizeof(char), 1, ExeFile);
	fseek(ExeFile, 0L, SEEK_SET);

	//Write pointers
	while(BlockCounter != 160)
	{
		Pointer = (0x430000 + AddOn);
		printf("\nAdd on: %02X\n", AddOn);
		printf("\nPointer: %02X\n", Pointer);
		fseek(ExeFile, PointerPosition[BlockCounter], SEEK_SET);
		fwrite(&Pointer, sizeof(char), 4, ExeFile);
	    fseek(ExeFile, 0L, SEEK_SET);
		BlockCounter++;
		while ((Catcher = Data[DataIndex++]) != 0xAF) //Get end of block
			AddOn++;
		AddOn++;
	}
	fclose(ExeFile);
	return;
}

void DecompressFile()
{
    FILE *DecodedFile;
    //Scene name + "Decoded"

    sprintf(OutputFileName, "%s_Decoded.DECODED", InputFileName);
    DecodedFile = fopen(OutputFileName, "wb");

    int CSSP					= 0; //Compressed scenario start position
    int CTSP				 	= 0; //Compressed text start position
    int UTSP					= 0; //Uncompressed text start position
    int TextSize				= 0;
    int ScenarioSize			= 0;
	int ExtraZeroes 			= 0;
    char *SourceData;
    char *ScenarioData;
    char *TextData;

    SourceData		= malloc(InputFileSize * sizeof(char));
    ScenarioData	= malloc(0xFFFF * sizeof(char));
    TextData		= malloc(0xFFFF * sizeof(char));

    fseek(InputFile, 0L, SEEK_END);
    InputFileSize = ftell(InputFile);
    fseek(InputFile, 0L, SEEK_SET);
    fread(SourceData, sizeof(char), InputFileSize, InputFile);

    CSSP			= (SourceData[0] | SourceData[1] << 8) * 16;
    CTSP			= (SourceData[2] | SourceData[3] << 8) * 16;
    ScenarioSize	= (SourceData[CSSP] | SourceData[CSSP + 1] << 8) | (SourceData[CSSP + 2] << 16 | SourceData[CSSP + 3] << 24);
    TextSize		= (SourceData[CTSP] | SourceData[CTSP + 1] << 8) | (SourceData[CTSP + 2] << 16 | SourceData[CTSP + 3] << 24);
    UTSP	 		= CTSP - CSSP;

    printf("\nCompressed text start position: %02X", CTSP);
    printf("\nScenario start position: %02X", CSSP);
    printf("\nText start position: %02X", UTSP);
    printf("\nScenario size: %02X", ScenarioSize);
    printf("\nText size: %02X", TextSize);

    //Write header
    fwrite(&CSSP, sizeof(short), 1, DecodedFile);			//Scenario start position
    fwrite(&UTSP, sizeof(short), 1, DecodedFile);			//Text start position
    fwrite(&ScenarioSize, sizeof(short), 1, DecodedFile);	//Scenario size
    fwrite(&TextSize, sizeof(long), 1, DecodedFile);		//Text size

    //Decompress scenario data
	ScenarioData = lzss(SourceData + CSSP, ScenarioSize, 4);

	//Write scenario data to file
    fseek(DecodedFile, 0x10, SEEK_SET);								//Skip cursor right after header
	fwrite(ScenarioData, sizeof(char), ScenarioSize, DecodedFile);

    //Decompress text data
	TextData = lzss(SourceData + UTSP + 0x10, TextSize, 4);

	//Write text data to file
	fseek(DecodedFile, 0x10 + ScenarioSize + (0x10 - (ScenarioSize & 0x0F)), SEEK_SET); //Skip cursor right after scenario file
	fwrite(TextData, sizeof(char), TextSize, DecodedFile);

	//Filler bytes at the end of file
	fwrite(&ExtraZeroes, sizeof(char), (0x10 - (TextSize & 0x0F)), DecodedFile);

    return;
}

void DecompressFile2()
{
    FILE *DecodedFile;
    //Scene name + "Decoded"

    sprintf(OutputFileName, "%s_Decoded.DECODED", InputFileName);
    DecodedFile = fopen(OutputFileName, "wb");

    //int CSSP					= 0; //Compressed scenario start position
    //int CTSP				 	= 0; //Compressed text start position
    //int UTSP					= 0; //Uncompressed text start position
    int DataSize				= 0;
    //int ScenarioSize			= 0;
	//int ExtraZeroes 			= 0;
    char *SourceData;
    //char *ScenarioData;
    char *GFXData;

    SourceData		= malloc(InputFileSize * sizeof(char));
    //ScenarioData	= malloc(0xFFFF * sizeof(char));
    GFXData		= malloc(0xFFFF * sizeof(char));

    fseek(InputFile, 0L, SEEK_END);
    InputFileSize = ftell(InputFile);
    fseek(InputFile, 0L, SEEK_SET);
    fread(SourceData, sizeof(char), InputFileSize, InputFile);

    //CSSP			= (SourceData[0] | SourceData[1] << 8) * 16;
    //CTSP			= (SourceData[2] | SourceData[3] << 8) * 16;
    //ScenarioSize	= (SourceData[CSSP] | SourceData[CSSP + 1] << 8) | (SourceData[CSSP + 2] << 16 | SourceData[CSSP + 3] << 24);
    //TextSize		= (SourceData[CTSP] | SourceData[CTSP + 1] << 8) | (SourceData[CTSP + 2] << 16 | SourceData[CTSP + 3] << 24);
    //UTSP	 		= CTSP - CSSP;

    DataSize = InputFileSize;

    //printf("\nCompressed text start position: %02X", CTSP);
    //printf("\nScenario start position: %02X", CSSP);
    //printf("\nText start position: %02X", UTSP);
    //printf("\nScenario size: %02X", ScenarioSize);
    //printf("\nText size: %02X", TextSize);

    //Write header
    //fwrite(&CSSP, sizeof(short), 1, DecodedFile);			//Scenario start position
    //fwrite(&UTSP, sizeof(short), 1, DecodedFile);			//Text start position
    //fwrite(&ScenarioSize, sizeof(short), 1, DecodedFile);	//Scenario size
    //fwrite(&TextSize, sizeof(long), 1, DecodedFile);		//Text size

    //Decompress scenario data
	GFXData = lzs(SourceData + 0x30, DataSize - 0x30);

	//Write scenario data to file
    fseek(DecodedFile, 0x10, SEEK_SET);								//Skip cursor right after header
	fwrite(GFXData, sizeof(char), DataSize, DecodedFile);

    return;
}

int FileCheck (FILE *File, char *Name) //Check if file exists
{
	if (File == NULL)
	{
		printf("\nERROR: Can't find %s", Name);
		return(1);
	}
	return(0);
}

int ScenarioStructureCheck (int Instance, int FileNumber, int ReverseLines, int StructureFlag) //Structure integrity check
{
	if (StructureFlag != Instance)
	{
		printf("\nERROR: Structure broken at scene %02X, line %d", FileNumber, ReverseLines);
		return(1);
	}
	return(0);
}

int CompileTextFile()
{
	//Using little endian
	int GameCharacterTable[100] = 	{
							0x0000, //Space
							0x0100, //°
							0x0200, //A
							0x0300, //B
							0x0400, //C
							0x0500, //D
							0x0600, //E
							0x0700, //F
							0x0800, //F
							0x0900, //G
							0x0a00, //I
							0x0b00, //J
							0x0c00, //K
							0x0d00, //L
							0x0e00, //M
							0x0f00, //N
							0x1000, //O
							0x1100, //P
							0x1200, //Q
							0x1300, //R
							0x1400, //S
							0x1500, //T
							0x1600, //U
							0x1700, //V
							0x1800, //W
							0x1900, //X
							0x1A00, //Y
							0x1B00, //Z
							0x1C00, //K
							0x1D00, //K
							0x1E00, //K
							0x1F00, //K
							0x2000, //K
							0x2100, //K
							0x2200, //K
							0x2300, //K
							0x2400, //K
							0x2500, //K
							0x2600, //K
							0x2700, //K
							0x2800, //K
							0x2900, //K
							0x2A00, //K
							0x2B00, //K
							0x2C00, //K
							0x2D00, //K
							0x2E00, //K
							0x2F00, //K
							0x3000, //K
							0x3100, //K
							0x3200, //K
							0x3300, //K
							0x3400, //K
							0x3500, //K
							0x3600, //K
							0x3700, //K
							0x3800, //K
							0x3900, //K
							0x3a00, //K
							0x3b00, //K
							0x3c00, //K
							0x3d00, //K
							0x3e00, //8
							0x3f00, //9
							0x4000, //(
							0x4100, //)
							0x4200, //?
							0x4300, //!
							0x4400, //:
							0x4500, //-
							0x4600, //.
							0x4700, //,
							0x4800, //'
							0x4900, //*
							0x4A00, //"
							0X4B00, //%
							0x5B00, //á
							0x4D00, //í
							0x4E00, //ú
							0X4F00, //é
							0x5000, //ó
							0x5100, //ñ
							0x5200, //Á
							0X5300, //Í
							0x5400, //Ú
							0x5500, //É
							0x5600, //Ó
							0X5700, //Ñ
							0x5800, //¡
							0x5900, //¿
							0x5A00, //<3
							0x4C00, //Slash (in-game)
							0x94B0, //@
							0x94B2, //>
							0xAFB3, //Slash (end text block)
							0x32B7, //|
							};

	char CharacterTable[100][5] = 	{
								" ", "#", "A", "B",
								"C", "D", "E", "F",
								"G", "H", "I", "J",
								"K", "L", "M", "N",
								"O", "P", "Q", "R",
								"S", "T", "U", "V",
								"W", "X", "Y", "Z",
								"a", "b", "c", "d",
								"e", "f", "g", "h",
								"i", "j", "k", "l",
								"m", "n", "o", "p",
								"q", "r", "s", "t",
								"u", "v", "w", "x",
								"y", "z", "0", "1",
								"2", "3", "4", "5",
								"6", "7", "8", "9",
								"(", ")", "?", "!",
								":", "-", ".", ",",
								"'", "*", "\"", "%",
								"$", "$", "$", "$",
								"$", "$", "$", "$",
								"$", "$", "$", "$",
								"$", "$", "$", "/"
								"@", ">", "\\", "|"
								};

	int UnicodeTable[15] = 		{
								//á		//í		//ú		//é
								0xA1C3, 0xADC3, 0xBAC3, 0xA9C3,
								//ó		//ñ		//Á		//Í
								0xB3C3, 0xB1C3, 0x81C3, 0x8DC3,
								//Ú		//É		//Ó		//Ñ
								 0x9AC3, 0x89C3, 0x93C3, 0x91C3,
								//¡		//¿
								0xA1C2, 0xBFC2
								};

	int UnicodeTable2[15] = 	{
								//á		//í		//ú		//é
								0xA599E2, 0xADE3, 0xBAE3, 0xA9E3,
								//ó		//ñ		//Á		//Í
								0xB3E3, 0xB1E3, 0x81E3, 0x8DE3,
								//Ú		//É		//Ó		//Ñ
								0x9AE3, 0x89E3, 0x93E3, 0x91E3,
								//¡		//¿
								0xA1E2, 0xBFE2, 0xA599E2
								};

	char ropcodes2[98][100] =	{
								"End20",
								"TitleDisplay",
								"Nazo23",
								"Choice",
								"Nazo26",
								"Nazo27",
								"Jump",
								"JumpBlk",
								"VariableChoice",
								"ChoiceSetup",
								"Push2D",
								"Return2D",
								"Push2F",
								"Return2F",
								"DateSettingNoCalendar",
								"TimeSetting",
								"FlagAdd",
								"FlagSub",
								"LoadBG",
								"ClearCharacter",
								"Nazo40",
								"LoadVisual",
								"LoadCharacter",
								"Nazo44",
								"Effect",
								"SepiaEffect",
								"Nazo50",
								"SameBlkJump",
								"IfEq",
								"IfNe",
								"IfGt",
								"IfLe",
								"IfGte",
								"IfLte",
								"IfBitOn",
								"IfBitOff",
								"SetFlag",
								"FlagAdd62",
								"FlagSetBit",
								"DisplayMessageAndClear",
								"DisplayMessage",
								"Nazo6B",
								"Nazo6C",
								"Nazo70",
								"Nazo71",
								"Nazo72",
								"Brighten",
								"Nazo74",
								"Nazo75",
								"Nazo76",
								"WhiteOut",
								"WhiteIn",
								"Nazo79",
								"Nazo7A",
								"StartBGM",
								"FadeBGM",
								"PauseBGM",
								"WaitForFadeBGM",
								"FadeInBGM",
								"SetNextBGM",
								"GameOver",
								"StartEnding",
								"LoadPCM",
								"StopPCM",
								"StartPCM",
								"WaitPCM",
								"NazoPCMA6",
								"NazoPCMA7",
								"NazoPCMA8",
								"EndTextBlk",
								"NewLine",
								"WaitKey",
								"WaitPage",
								"NazoB4",
								"CharacterDrawSpeed",
								"Wait",
								"NazoB8",
								"SetTextHOffset",
								"SetTextVOffset",
								"Flash",
								"Vibrato",
								"LoadBG2",
								"NazoBE",
								"LoadVisualScene",
								"LoadHVisualScene",
								"ChangeCharacter",
								"LoadCharacterC2",
								"ClearAndLoadCharacter",
								"LoadCharacterAndBg",
								"LoadThreeCharacters",
								"NazoC7",
								"NazoC9",
								"NazoCC",
								"NazoCF",
								"DateSetting",
								"SpecialEffect",
								"NazoF8",
								"EndFF"
								};

	int ropcodes1[98] = 	{
							0x20, 0x21, 0x23, 0x24, 0x26,
							0x27, 0x28, 0x29, 0x2b, 0x2c,
							0x2d, 0x2e, 0x2f, 0x30, 0x32,
							0x33, 0x34, 0x35, 0x3c, 0x3d,
							0x40, 0x42, 0x43, 0x44, 0x45,
							0x47, 0x50, 0x55, 0x56, 0x57,
							0x58, 0x59, 0x5a, 0x5b, 0x5e,
							0x5f, 0x61, 0x62, 0x63, 0x69,
							0x6a, 0x6b, 0x6c, 0x70, 0x71,
							0x72, 0x73, 0x74, 0x75, 0x76,
							0x77, 0x78, 0x79, 0x7A, 0x80,
							0x81, 0x82, 0x85, 0x87, 0x88,
							0x94, 0x95, 0xA0, 0xA1, 0xA2,
							0xa3, 0xa6, 0xa7, 0xa8, 0xaf,
							0xb0, 0xb2, 0xb3, 0xb4, 0xb6,
							0xb7, 0xb8, 0xb9, 0xba, 0xbb,
							0xbc, 0xbd, 0xbe, 0xbf, 0xc0,
							0xc1, 0xc2, 0xc3, 0xc4, 0xc6,
							0xc7, 0xc9, 0xcc, 0xcf, 0xf5,
							0xf7, 0xf8, 0xff
							};

	FILE *InputText; //File pointers
    FILE *TempFile;

	char Catcher = '0'; //Char variables
	const char Space[2] = " ";
	char LineData[0xFFF]; //Fixed arrays
	char NextLine[0xFFF];
	char *Tokens; //Arrays
	char *Text;
	char *OpcodeString;
	char *Characters;

	int TextPosition 		= 0; //Array indexes
	int LDPosition 			= 0;
	int OpcodeIndex 		= 0;
	int OpcodeIndexBackup 	= 0;
	int FontIndex 			= 0;
    int SPChar 				= 0;
    int NMChar 				= 0;
	int Lines 				= 0;
	int ReverseLines 		= 0;
    int LastLinePosition 	= 0;
	int FileNumber 			= 0; //Attributes

	Tokens 			= malloc(0xFFFF * sizeof(char)); //Allocate memory
    Text			= malloc(0xFFFF * sizeof(char));
    OpcodeString 	= malloc(0xFFFF * sizeof(char));
    Characters 		= malloc(0xFFFF * sizeof(char));

	InputText = fopen("EXE_TEXT_TRANSLATED_30.txt", "r"); FileCheck(InputText, "SCRIPT");
	TempFile = fopen("EXE_TEXT_TRANSLATED_30.bin", "wb+");

	while ((Catcher = fgetc(InputText)) != EOF)	//Get lines
		if (Catcher == '\n')
			Lines++;
	Lines++;									//Include last line

	fseek(InputText, 0L, SEEK_SET);				//Move cursor to beginning

	while (Lines--)
	{
		ReverseLines++;							//Update line counter
		fgets(LineData, 0xFFF, InputText);		//Get line

		if(strcmp(LineData, "\n") == 0 || strcmp(LineData,"\r\n") == 0 || strncmp(LineData, ";;", 2) == 0 || strncmp(LineData, "CancelNewLine", 13) == 0) //Ignore empty line and comments
			continue;

		Tokens = strtok(LineData, " "); Tokens[strcspn(Tokens, "\n")] = 0; //Get token and remove newline

		printf("\nTokens: %s", Tokens);

		if (strcmp(Tokens, "`") == 0)			//Text Handler
		{
			LDPosition = 2;		//Starting after "`"

			while (strncmp(LineData + LDPosition, "\n", 1) != 0)
			{
				Characters[0] = LineData[LDPosition];
				printf("\nCHARACTER: %02X", LineData[LDPosition]);

		        //////////////////////////////////////PSEUDO-UNICODE BLOCK//////////////////////////////////////
		        //Catch Unicode char
				if ((Characters[0] & 0xF0) == 0xC0)
		        {
		        	memcpy(&NMChar, LineData + LDPosition, 2);		//Read next character
		        	while (NMChar != SPChar)						//Obtain font index
					{
		        		memcpy(&SPChar, UnicodeTable + FontIndex, 2);
		        		FontIndex++;
					}

					FontIndex--;			//Compensate for 0 slot

					if (FontIndex >= 14)	//Catch wrong character
					{
						printf("\nERROR: Can't find special character (Unicode 2-byte) in scene %02X, line %d", FileNumber, ReverseLines);
						return(1);
					}

					memcpy(Text + TextPosition, &GameCharacterTable[FontIndex+76], 2);	//Copy char
					TextPosition 	+= 2;
		        	LDPosition 		+= 2;
		        	NMChar 			= 0;
		        	SPChar 			= 0;
					goto RESET_INDEXES;
		        }
		        //////////////////////////////////////PSEUDO-UNICODE BLOCK//////////////////////////////////////

		        //////////////////////////////////////PSEUDO-UNICODE BLOCK//////////////////////////////////////
		        //Catch Unicode char
				if ((Characters[0] & 0xF0) == 0xE0)
		        {
		        	memcpy(&NMChar, LineData + LDPosition, 3);		//Read next character
		        	while (NMChar != SPChar)						//Obtain font index
					{
		        		memcpy(&SPChar, UnicodeTable2 + FontIndex, 3);
		        		FontIndex++;
					}
					printf("\nCHARACTER N: %02X", NMChar);
					printf("\nCHARACTER S: %02X", SPChar);

					FontIndex--;			//Compensate for 0 slot

					if (FontIndex >= 15)	//Catch wrong character
					{
						printf("\nERROR: Can't find special character (Unicode 3-byte) in scene %02X, line %d", FileNumber, ReverseLines);
						return(1);
					}

					memcpy(Text + TextPosition, &GameCharacterTable[FontIndex+90], 3);	//Copy char
					TextPosition 	+= 2;
		        	LDPosition 		+= 3;
		        	NMChar 			= 0;
		        	SPChar 			= 0;
					goto RESET_INDEXES;
		        }
		        //////////////////////////////////////PSEUDO-UNICODE BLOCK//////////////////////////////////////

				while (strncmp(Characters, CharacterTable[FontIndex], 1) != 0)
					FontIndex++;

				if (FontIndex >= 94) //Catch wrong character
				{
					printf("\nERROR: Can't find character '%02X' ('%c') in line %d, column %d. FI: %02X", Characters[0], Characters[0], ReverseLines, LDPosition, FontIndex);
					return(1);
				}
				memcpy(Text + TextPosition, &GameCharacterTable[FontIndex], 2);
				TextPosition += 2;
		        LDPosition++;
		        RESET_INDEXES:
		        FontIndex = 0;
			}

			LastLinePosition = ftell(InputText);
			fgets(NextLine, 0xFFF, InputText);
			NextLine[strcspn(NextLine, "\n")] = 0;			//Remove newline
			printf("\nNextLine: %s\n", NextLine);
			if ((strncmp(NextLine, "WaitPage", 8) != 0) && (strncmp(NextLine, "CancelNewLine", 13) != 0))		//Don't write newline if next opcode is waitpage or if user canceled it
				Text[TextPosition++] = 0xB0;
			fseek(InputText, LastLinePosition, SEEK_SET);	//Restore last position
			continue;
		}

		Tokens[strcspn(Tokens, "\n")] = 0;		//Remove newline

		OPCODES:
		if (strcmp(Tokens, OpcodeString) == 0) //Use same index if duplicated
		{
			OpcodeIndex = OpcodeIndexBackup;
			goto ADJUST_OPCODE_INDEX;
		}

		while (strcmp(Tokens, OpcodeString) != 0)
		{
			OpcodeString = ropcodes2[OpcodeIndex];
			OpcodeIndex++;
			if (OpcodeIndex >= 99)
			{
				printf("\nERROR: Can't find opcode '%s' in line %d.", Tokens, ReverseLines);
				return(1);
			}
		}

		OpcodeIndexBackup = OpcodeIndex;

		ADJUST_OPCODE_INDEX:
		//-1 to compensate for slot 0
		OpcodeIndex--;
		memcpy(Text + TextPosition, &ropcodes1[OpcodeIndex], 1);
        TextPosition++;
        OpcodeIndex = 0;

		while (Tokens != NULL) //ARGUMENTS
		{
			Tokens = strtok(NULL, Space); 		//Skip literal argument if NULL
			if (Tokens == NULL)
				goto SKIP_CHECK;
			Tokens[strcspn(Tokens, "\n")] = 0;	//Remove newline

			if (strcmp(Tokens, "AkariPoints") == 0)
				strcpy(Tokens, "14");
			else if (strcmp(Tokens, "WhiteBG") == 0)
				strcpy(Tokens, "fe");
			else if (strcmp(Tokens, ">") == 0)
			{
				Tokens = strtok(NULL, Space); Tokens[strcspn(Tokens, "\n")] = 0;
				goto OPCODES;
			}

			SKIP_CHECK:
			Text[TextPosition] = strtol(Tokens, NULL, 16);
			TextPosition++;
		}

		//Skip last unnecessary token
		TextPosition--;
	}
	fseek(TempFile, 0, SEEK_SET); fwrite(Text, sizeof(char), TextPosition, TempFile);
	fclose(TempFile);
	return(0);
}

int ConvertFile()
{
	//Using little endian
	int GameCharacterTable[100] = 	{
							0x00000683, //space
							0x01000683, //square
							0x02001283, //A
							0x03000E83, //B
							0x04000F83, //C
							0x05001083, //D
							0x06000D83, //E
							0x07000D83, //F
							0x08001183, //G
							0x09000F83, //H
							0x0a000883, //I
							0x0b000A83, //J
							0x0c000F83, //K
							0x0d000D83, //L
							0x0e001283, //M
							0x0f001283, //N
							0x10001283, //O
							0x11001283, //P
							0x12001283, //Q
							0x13001283, //R
							0x14001283, //S
							0x15001283, //T
							0x16001283, //U
							0x17001283, //V
							0x18001283, //W
							0x19001283, //X
							0x1A001283, //Y
							0x1B001283, //Z
							0x10B11C00, //a
							0x11B11D00, //b
							0x10B11E00, //c
							0x10B11F00, //d
							0x10B12000, //e
							0x0AB12100, //f
							0x0DB12200, //g
							0x0CB12300, //h
							0x04B12400, //i
							0x09B12500, //j
							0x0DB12600, //k
							0x14B92700, //l
							0x14B92800, //m
							0x14B92900, //n
							0x14B92A00, //o
							0x14B92B00, //p
							0x14B92C00, //q
							0x14B92D00, //r
							0x14B92E00, //s
							0x14B92F00, //t
							0x14B93000, //u
							0x14B93100, //v
							0x14B93200, //w
							0x14B93300, //x
							0x14B93400, //y
							0x14B93500, //z
							0x14B93600, //0
							0x14B93700, //1
							0x14B93800, //2
							0x14B93900, //3
							0x14B93a00, //4
							0x14B93b00, //5
							0x14B93c00, //6
							0x14B93d00, //7
							0x14B93e00, //8
							0x14B93f00, //9
							0x14B94000, //(
							0x14B94100, //)
							0x14B94200, //?
							0x14B94300, //!
							0x14B94400, //:
							0x14B94500, //-
							0x14B94600, //.
							0x14B94700, //,
							0x14B94800, //'
							0x14B94900, //*
							0x14B94A00, //"
							0X14B94B00, //%
							0x14B94C00, //Slash (in-game)
							//UNICODE 1
							0x4D00, //á
							0x4E00, //í
							0x4F00, //ú
							0X5000, //é
							0x5100, //ó
							0x5200, //ñ
							0x5300, //Á
							0X5400, //Í
							0x5500, //Ú
							0x5600, //É
							0x5700, //Ó
							0X5800, //Ñ
							0x5900, //¡
							0x5A00, //¿
							//UNICODE 2
							0x5B00, //<3
							0x5C00, //Three dots
							0x5D00, //Six dots (input twice in the script)
							//CONTROL CODES
							0x94B0, //@
							0x94B2, //>
							0xAFB3, //Slash (end text block)
							0x32B7, //|
							};

	char CharacterTable[100][5] = 	{
								" ", "#", "A", "B",
								"C", "D", "E", "F",
								"G", "H", "I", "J",
								"K", "L", "M", "N",
								"O", "P", "Q", "R",
								"S", "T", "U", "V",
								"W", "X", "Y", "Z",
								"a", "b", "c", "d",
								"e", "f", "g", "h",
								"i", "j", "k", "l",
								"m", "n", "o", "p",
								"q", "r", "s", "t",
								"u", "v", "w", "x",
								"y", "z", "0", "1",
								"2", "3", "4", "5",
								"6", "7", "8", "9",
								"(", ")", "?", "!",
								":", "-", ".", ",",
								"'", "*", "\"", "%",
								"/",
								//UNICODE
								"$", "$", "$", "$",
								"$", "$", "$", "$",
								"$", "$", "$", "$",
								"$", "$",
								//UNICODE 2
								"$", "$", "$",
								//CONTROL CODES
								">", "@", "\\", "|"
								};

	int UnicodeTable[15] = 		{
								//á		//í		//ú		//é		//ó		//ñ
								0xA1C3, 0xADC3, 0xBAC3, 0xA9C3, 0xB3C3, 0xB1C3,
								//Á		//Í		//Ú		//É		//Ó		//Ñ
								0x81C3, 0x8DC3, 0x9AC3, 0x89C3, 0x93C3, 0x91C3,
								//¡		//¿
								0xA1C2, 0xBFC2
								};

	int UnicodeTable2[15] = 	{
								//♥		//…		//ú		//é
								0xA599E2, 0xA680E2, 0XA580E2, 0xA9E3,
								//ó		//ñ		//Á		//Í
								0xB3E3, 0xB1E3, 0x81E3, 0x8DE3,
								//Ú		//É		//Ó		//Ñ
								0x9AE3, 0x89E3, 0x93E3, 0x91E3,
								//¡		//¿
								0xA1E2, 0xBFE2, 0xA599E2
								};

	char ropcodes2[110][100] =	{
								"End20",
								"TitleDisplay",
								"Nazo23",
								"Choice",
								"Nazo26",
								"Nazo27",
								"Jump",
								"JumpBlk",
								"VariableChoice",
								"ChoiceSetup",
								"Push2D",
								"Return2D",
								"Push2F",
								"Return2F",
								"DateSettingNoCalendar",
								"TimeSetting",
								"FlagAdd",
								"FlagSub",
								"LoadBG",
								"ClearCharacter",
								"Nazo40",
								"LoadVisual",
								"LoadCharacter",
								"Nazo44",
								"Effect",
								"SepiaEffect",
								"Nazo50",
								"SameBlkJump",
								"IfEq",
								"IfNe",
								"IfGt",
								"IfLe",
								"IfGte",
								"IfLte",
								"IfBitOn",
								"IfBitOff",
								"SetFlag",
								"FlagAdd62",
								"FlagSetBit",
								"DisplayMessageAndClear",
								"DisplayMessage",
								"Nazo6B",
								"Nazo6C",
								"Nazo70",
								"Nazo71",
								"Nazo72",
								"Brighten",
								"Nazo74",
								"Nazo75",
								"Nazo76",
								"WhiteOut",
								"WhiteIn",
								"Nazo79",
								"Nazo7A",
								"StartBGM",
								"FadeBGM",
								"PauseBGM",
								"WaitForFadeBGM",
								"FadeInBGM",
								"SetNextBGM",
								"GameOver",
								"StartEnding",
								"LoadPCM",
								"StopPCM",
								"StartPCM",
								"WaitPCM",
								"NazoPCMA6",
								"NazoPCMA7",
								"NazoPCMA8",
								"EndTextBlk",
								"NewLine",
								"WaitKey",
								"WaitPage",
								"NazoB4",
								"CharacterDrawSpeed",
								"Wait",
								"NazoB8",
								"SetTextHOffset",
								"SetTextVOffset",
								"Flash",
								"Vibrato",
								"LoadBG2",
								"NazoBE",
								"LoadVisualScene",
								"LoadHVisualScene",
								"ChangeCharacter",
								"LoadCharacterC2",
								"ClearAndLoadCharacter",
								"LoadCharacterAndBg",
								"LoadThreeCharacters",
								"NazoC7",
								"NazoC9",
								"NazoCC",
								"NazoCF",
								"DateSetting",
								"SpecialEffect",
								"NazoF8",
								"EndFF",
								"SayNameD1",
								"SayNameD2",
								"SayNameD3",
								"SayNameD4",
								"SayNameD5",
								"SayNameD6",
								"SayNameDF",
								"SayNameE0",
								"SayNameE1",
								"SayNameE2",
								"SayNameE3",
								"SayNameE4",
								};

	int ropcodes1[110] = 	{
							0x20, 0x21, 0x23, 0x24, 0x26,
							0x27, 0x28, 0x29, 0x2b, 0x2c,
							0x2d, 0x2e, 0x2f, 0x30, 0x32,
							0x33, 0x34, 0x35, 0x3c, 0x3d,
							0x40, 0x42, 0x43, 0x44, 0x45,
							0x47, 0x50, 0x55, 0x56, 0x57,
							0x58, 0x59, 0x5a, 0x5b, 0x5e,
							0x5f, 0x61, 0x62, 0x63, 0x69,
							0x6a, 0x6b, 0x6c, 0x70, 0x71,
							0x72, 0x73, 0x74, 0x75, 0x76,
							0x77, 0x78, 0x79, 0x7A, 0x80,
							0x81, 0x82, 0x85, 0x87, 0x88,
							0x94, 0x95, 0xA0, 0xA1, 0xA2,
							0xa3, 0xa6, 0xa7, 0xa8, 0xaf,
							0xb0, 0xb2, 0xb3, 0xb4, 0xb6,
							0xb7, 0xb8, 0xb9, 0xba, 0xbb,
							0xbc, 0xbd, 0xbe, 0xbf, 0xc0,
							0xc1, 0xc2, 0xc3, 0xc4, 0xc6,
							0xc7, 0xc9, 0xcc, 0xcf, 0xf5,
							0xf7, 0xf8, 0xff, 0xd1, 0xd2,
							0xd3, 0xd4, 0xd5, 0xd6, 0xdf,
							0xe0, 0xe1, 0xe2, 0xe3, 0xe4
							};

	FILE *InputText; //File pointers
	FILE *TableFile;
    FILE *NEWLVNS3PAK;

	char Catcher = '0'; //Char variables
	const char Space[2] = " ";

	//Fixed arrays
	char Zero[16] = {	1, 0, 0, 0,
						0, 0, 0, 0,
						0, 0, 0, 0,
						0, 0, 0, 0 };
    char Key[11] = {0xd1, 0x58, 0x6a, 0x56, 0x9a, 0x13, 0xa5, 0xf7, 0x7c, 0x3e, 0x74};
	char LineData[0xFFF];
	char NextLine[0xFFF];

	char *Tokens; //Arrays
	char *Text;
	char *LabelOffsets;
	char *OpcodeString;
	char *Characters;
    char *TableData;
    char *ExpandedData;
    char *FullData;
    char *ScenarioData;

	int TextPosition = 0; //Array indexes
    int TextPositionBK = 0;
	int LDPosition = 0;
	int LOPosition = 0;
	int OpcodeIndex = 0;
	int OpcodeIndexBackup = 0;
	int FontIndex = 0;
    int KeyPosition = 0;
    int SDPosition = 0;
    int SPChar = 0;
    int NMChar = 0;

	int FileCount = -1; //Counters
	int LabelCounter = 0;
	int LabelCounterBackup = 0;
	int Lines = 0;
	int ReverseLines = 0;
    int IT = 0;
    int LastLinePosition = 0;
	//Sizeof
    int NegativeBlockSize = 0;
    int FullDataSize = 0;
    int HeaderDataSize = 0;
	int FinalDataSize = 0;
	int ScenarioSize  = 0;
	//Attributes
	int FileNumber = 0;
	int Label = 0;
	long LastPosition = 0;
	long PropertyPosition = 0;
	long HeadOfNextFile = 0;
	//Scene structure
	int StructureFlag = 0;

	Tokens 			= malloc(0xFFFF * sizeof(char)); //Allocate memory
    Text			= malloc(0xFFFF * sizeof(char));
    LabelOffsets 	= malloc(0xFFFF * sizeof(char));
    OpcodeString 	= malloc(0xFFFF * sizeof(char));
    Characters 		= malloc(0xFFFF * sizeof(char));
    FullData		= malloc(0xFFFFFF * sizeof(char)); //Readable data for BOTH text and scenario
    ExpandedData	= malloc(0xFFFFFF * sizeof(char)); //"Compressed" data for BOTH text and scenario
    TableData		= malloc(0xFFFF * sizeof(char)); //Table data
    ScenarioData	= malloc(0xFFFFFF * sizeof(char)); //Scenario data

	NEWLVNS3PAK = fopen("LVNS3SCN.PAK", "rb+"); FileCheck(NEWLVNS3PAK, "PAK");
    TableFile = fopen("TABLE.TABLE", "rb+"); FileCheck(TableFile, "TABLE");
    fread(TableData, sizeof(char), 0x5D18, TableFile);
	InputText = fopen("ScriptReady.txt", "r");	FileCheck(InputText, "SCRIPT");

	TempFile = fopen("TempFile.bin", "wb+");

	while ((Catcher = fgetc(InputText)) != EOF)	//Get lines
		if (Catcher == '\n')
			Lines++;
	Lines++;									//Include last line

	fseek(InputText, 0L, SEEK_SET);				//Move cursor to beginning

	while (Lines--)
	{
		ReverseLines++;							//Update line counter
		fgets(LineData, 0xFFF, InputText);		//Get line

		if(strcmp(LineData, "\n") == 0 || strcmp(LineData,"\r\n") == 0 || strncmp(LineData, ";;", 2) == 0 || strncmp(LineData, "CancelNewLine", 13) == 0) //Ignore empty line and comments
			continue;

		Tokens = strtok(LineData, " "); Tokens[strcspn(Tokens, "\n")] = 0; //Get token and remove newline

		if (strcmp(Tokens, "ScenarioStart") == 0)	//Get file numbers
		{
			ScenarioStructureCheck(0, FileNumber, ReverseLines, StructureFlag); StructureFlag++; //Check structure
			Tokens 		= strtok(NULL, Space); FileNumber 	= strtol(Tokens, NULL, 16); //Get file ID
			Tokens 		= strtok(NULL, Space); FileCount 	= strtol(Tokens, NULL, 10); //Get file counter
			continue;
		}

		if ((strcmp(Tokens, "TextEnd") == 0) || (strcmp(Tokens, "ScenarioEnd") == 0)) //Write contents and reset whenever an entry ends
		{
			while((LabelCounterBackup / 2) != LabelCounter)				//Update label offsets
			{
				memcpy(&Label, LabelOffsets + LabelCounterBackup, 2); 	//Get label
				Label += (LabelCounter * 2) + 2;						//Add offset
				memcpy(LabelOffsets + LabelCounterBackup, &Label, 2);	//Copy back
				LabelCounterBackup += 2;
			}

			memcpy(FullData, &LabelCounter, 2);								//Join label counter, offsets and text
			memcpy(FullData + 2, LabelOffsets, LabelCounter * 2);
			memcpy(FullData + 2 + (LabelCounter * 2), Text, TextPosition);
			FullDataSize = 2 + (LabelCounter * 2) + TextPosition;			//Get full data size
	        NegativeBlockSize = ~(FullDataSize);							//Get negative file size
			FullDataSize += (FullDataSize / 8) + 5;							//Get encoded file size

	        for (SDPosition = 4, IT = 0, TextPositionBK = 0; SDPosition != FullDataSize; SDPosition++, IT++, TextPositionBK++) //Write text
	        {
	            if (IT % 9 == 0)
	            {
	            	 ExpandedData[SDPosition] = 0xFF;
	            	 TextPositionBK--;
	            }
	            else
	                ExpandedData[SDPosition] = FullData[TextPositionBK];
	        }

	        memcpy(ExpandedData, &NegativeBlockSize, 4);					//Write size

	        for (SDPosition = 0; SDPosition != FullDataSize; SDPosition++)	//NOT
	        {
	        	ExpandedData[SDPosition] = ~(ExpandedData[SDPosition]);
	        }

			if (strcmp(Tokens, "ScenarioEnd") == 0)						//Write header and pad scenario file with zeros
			{
				ScenarioStructureCheck(1, FileNumber, ReverseLines, StructureFlag); StructureFlag++;

				memcpy(ExpandedData + 0x10, ExpandedData, FullDataSize);	//Move data to next row
				FullDataSize += 0x10;

				while((FullDataSize % 0x10) != 0)							//Pad until end of row
				{
					memcpy(ExpandedData + FullDataSize, &Zero[2], 1);
					FullDataSize++;
				}

				memcpy(ExpandedData, &Zero, 0x10);						//Fill header
				HeaderDataSize = FullDataSize / 0x10;					//Reduce data size for header
				memcpy(ExpandedData + 2, &HeaderDataSize, 1);			//Write text start offset
				memcpy(ScenarioData, ExpandedData, FullDataSize);		//Save in scenario file
				ScenarioSize = FullDataSize;							//Save size
				FinalDataSize += FullDataSize;
				goto RESET_STUFF;
			}

			if (strcmp(Tokens, "TextEnd") == 0)
			{
				ScenarioStructureCheck(2, FileNumber, ReverseLines, StructureFlag); StructureFlag = 0;

				while((FullDataSize % 0x10) != 0)						//Pad until the end of the row
				{
					memcpy(ExpandedData + FullDataSize, &Zero[2], 1);
					FullDataSize++;
				}

				FinalDataSize += FullDataSize;										//Get final size
				memcpy(ExpandedData + ScenarioSize, ExpandedData, FullDataSize);	//Copy scenario
				memcpy(ExpandedData, ScenarioData, ScenarioSize);

		        for (SDPosition = 0, KeyPosition = 0; SDPosition < FinalDataSize; SDPosition++, KeyPosition++) //Encrypt
		        {
		        	ExpandedData[SDPosition] = (ExpandedData[SDPosition] + Key[KeyPosition % 11]) & 0xFF;
		        }

		        PropertyPosition = LastPosition;					//Get property position from last position
		        if (!PropertyPosition)								//Initial base if zero
		            PropertyPosition = 0x0011912A;
		        HeadOfNextFile = PropertyPosition + FinalDataSize;	//Get head of the next file

		        memcpy(TableData + (24 * FileCount) + 12, &PropertyPosition, 4);	//Write to table
		        memcpy(TableData + (24 * FileCount) + 16, &FinalDataSize, 4);
		        memcpy(TableData + (24 * FileCount) + 20, &HeadOfNextFile, 4);
		        LastPosition = HeadOfNextFile;										//Save last position
				fseek(NEWLVNS3PAK, PropertyPosition, SEEK_SET);						//Write to LVNS3PAK
				fwrite(ExpandedData, sizeof(char), FinalDataSize, NEWLVNS3PAK);
				FinalDataSize = 0;													//Reset final data size
				goto RESET_STUFF;
			}

			RESET_STUFF:
			TextPositionBK 		= 0;
			NegativeBlockSize 	= 0;
			SDPosition 			= 0;
			KeyPosition 		= 0;
			IT 					= 0;
			FullDataSize 		= 0;
			HeaderDataSize 		= 0;
			LabelCounter 		= 0;
			LabelCounterBackup 	= 0;
			LOPosition 			= 0;
			TextPosition 		= 0;
			memset(LabelOffsets, 0, 0xFFFF * sizeof(char));
			memset(Text, 0, 0xFFFF * sizeof(char));
			continue;
		}

		if (strncmp(LineData, "*", 1) == 0)		//LABEL
		{
			LabelCounter++;
	        memcpy(LabelOffsets + LOPosition, &TextPosition, 2);
			LOPosition += 2;
			continue;
		}

		if (strcmp(Tokens, "`") == 0)			//Text Handler
		{
			LDPosition = 2;		//Starting after "`"

			while (strncmp(LineData + LDPosition, "\n", 1) != 0)
			{
				Characters[0] = LineData[LDPosition];

		        //////////////////////////////////////PSEUDO-UNICODE BLOCK//////////////////////////////////////
		        //Catch Unicode char
				if ((Characters[0] & 0xF0) == 0xC0)
		        {
		        	memcpy(&NMChar, LineData + LDPosition, 2);		//Read next character
		        	while (NMChar != SPChar)						//Obtain font index
					{
		        		memcpy(&SPChar, UnicodeTable + FontIndex, 2);
		        		FontIndex++;
					}

					FontIndex--;			//Compensate for 0 slot

					if (FontIndex >= 16)	//Catch wrong character
					{
						printf("\nERROR: Can't find special character in scene %02X, line %d", FileNumber, ReverseLines);
						return(1);
					}

					memcpy(Text + TextPosition, &GameCharacterTable[FontIndex+77], 2);	//Copy char
					TextPosition 	+= 2;
		        	LDPosition 		+= 2;
		        	NMChar 			= 0;
		        	SPChar 			= 0;
					goto RESET_INDEXES;
		        }
		        //////////////////////////////////////PSEUDO-UNICODE BLOCK//////////////////////////////////////

		        //////////////////////////////////////PSEUDO-UNICODE BLOCK//////////////////////////////////////
		        //Catch Unicode char
				if ((Characters[0] & 0xF0) == 0xE0)
		        {
		        	memcpy(&NMChar, LineData + LDPosition, 3);		//Read next character
		        	while (NMChar != SPChar)						//Obtain font index
					{
		        		memcpy(&SPChar, UnicodeTable2 + FontIndex, 3);
		        		FontIndex++;
					}
					printf("\nCHARACTER N: %02X", NMChar);
					printf("\nCHARACTER S: %02X", SPChar);

					FontIndex--;			//Compensate for 0 slot

					if (FontIndex >= 15)	//Catch wrong character
					{
						printf("\nERROR: Can't find special character (Unicode 3-byte) in scene %02X, line %d", FileNumber, ReverseLines);
						return(1);
					}

					memcpy(Text + TextPosition, &GameCharacterTable[FontIndex+91], 3);	//Copy char
					TextPosition 	+= 2;
		        	LDPosition 		+= 3;
		        	NMChar 			= 0;
		        	SPChar 			= 0;
					goto RESET_INDEXES;
		        }
		        //////////////////////////////////////PSEUDO-UNICODE BLOCK//////////////////////////////////////

				while (strncmp(Characters, CharacterTable[FontIndex], 1) != 0)
				{
					FontIndex++;
				}

				if (FontIndex >= 98) //Catch wrong character
				{
					printf("\nERROR: Can't find character '%02X' ('%c') in line %d, column %d. FI: %02X", Characters[0], Characters[0], ReverseLines, LDPosition, FontIndex);
					return(1);
				}
				memcpy(Text + TextPosition, &GameCharacterTable[FontIndex], 4);
				TextPosition += 4;
		        LDPosition++;
		        RESET_INDEXES:
		        FontIndex = 0;
			}

			LastLinePosition = ftell(InputText);
			fgets(NextLine, 0xFFF, InputText);
			NextLine[strcspn(NextLine, "\n")] = 0;			//Remove newline
			printf("\nNextLine: %s\n", NextLine);
			if ((strncmp(NextLine, "WaitPage", 8) != 0) && (strncmp(NextLine, "CancelNewLine", 13) != 0))		//Don't write newline if next opcode is waitpage or if user canceled it
				Text[TextPosition++] = 0xB0;
			fseek(InputText, LastLinePosition, SEEK_SET);	//Restore last position
			continue;
		}

		Tokens[strcspn(Tokens, "\n")] = 0;		//Remove newline

		OPCODES:
		if (strcmp(Tokens, OpcodeString) == 0) //Use same index if duplicated
		{
			OpcodeIndex = OpcodeIndexBackup;
			goto ADJUST_OPCODE_INDEX;
		}

		while (strcmp(Tokens, OpcodeString) != 0)
		{
			OpcodeString = ropcodes2[OpcodeIndex];
			OpcodeIndex++;
			if (OpcodeIndex >= 111)
			{
				printf("\nERROR: Can't find opcode '%s' in line %d.", Tokens, ReverseLines);
				return(1);
			}
		}

		OpcodeIndexBackup = OpcodeIndex;

		ADJUST_OPCODE_INDEX:
		//-1 to compensate for slot 0
		OpcodeIndex--;
		memcpy(Text + TextPosition, &ropcodes1[OpcodeIndex], 1);
        TextPosition++;
		OpcodeIndex = 0;

		while (Tokens != NULL) //ARGUMENTS
		{
			Tokens = strtok(NULL, Space); 		//Skip literal argument if NULL
			if (Tokens == NULL)
				goto SKIP_CHECK;
			Tokens[strcspn(Tokens, "\n")] = 0;	//Remove newline

			if (strcmp(Tokens, "AkariPoints") == 0)
				strcpy(Tokens, "14");
			else if (strcmp(Tokens, "WhiteBG") == 0)
				strcpy(Tokens, "fe");
			else if (strcmp(Tokens, ">") == 0)
			{
				Tokens = strtok(NULL, Space); Tokens[strcspn(Tokens, "\n")] = 0;
				goto OPCODES;
			}

			SKIP_CHECK:
			Text[TextPosition] = strtol(Tokens, NULL, 16);
			TextPosition++;
		}

		//Skip last unnecessary token
		TextPosition--;
	}

	fseek(TableFile, 0, SEEK_SET); fwrite(TableData, sizeof(char), 0x5D18, TableFile);
    for (SDPosition = 0, KeyPosition = 0; SDPosition < 0x5D18; SDPosition++, KeyPosition++)	//Encrypt table
    {
    	TableData[SDPosition] = (TableData[SDPosition] + Key[KeyPosition % 11]) & 0xFF;
    }
	fseek(NEWLVNS3PAK, 0, SEEK_END); fwrite(TableData, sizeof(char), 0x5D18, NEWLVNS3PAK);
	fclose(NEWLVNS3PAK); fclose(TableFile);
	return(0);
}

int main(int argc, char* argv[])
{
    if(argc > 3)
    {
        printf("\nToo many arguments!\n");
        return(1);
    }

	if(argc == 3)
	{
    	InputFile = fopen(argv[2], "rb");

    	if (InputFile == NULL)
    	{
        	printf("\nFile doesn't exist: %s\n", argv[2]);
        	return(1);
    	}
    	strcpy(InputFileName, argv[2]);
    	strcpy(SupraInputFileName, argv[2]);

        if (strcmp(argv[1], "DecodeFile") == 0)
        {
            GetSourceData();
            DecryptFile();
            DecompressFile();
            return (0);
        }

        if (strcmp(argv[1], "DecodeFile3") == 0)
        {
            GetSourceData();
            DecompressFile();
            return (0);
        }

        if (strcmp(argv[1], "DecompFile") == 0)
        {
            GetSourceData();
            DecompressFile2();
            return (0);
        }

        else if (strcmp(argv[1], "EncodeFile") == 0)
        {
            EncodingFile();
            GetSourceDataEncodedFile();
            DoNot();
            MergeScenarioAndText();
            return (0);
        }
        else if (strcmp(argv[1], "EncodeOnly") == 0)
        {
            EncodingFile();
            GetSourceDataEncodedFile();
            DoNot();
            EncryptFile();
            return (0);
        }
        else if (strcmp(argv[1], "DecryptOnly") == 0)
        {
            GetSourceData();
            DecryptFile();
            return (0);
        }
        else if (strcmp(argv[1], "DecryptShizuku") == 0)
        {
            GetSourceData();
            DecryptFileShizuku();
            return (0);
        }
        //Encrypts a single file
        else if (strcmp(argv[1], "EncryptOnly") == 0)
        {
            GetSourceData();
            EncryptFile();
            return (0);
        }
        else if (strcmp(argv[1], "NotOnly") == 0)
        {
            GetSourceData();
            DoNot();
            return (0);
        }
        else if (strcmp(argv[1], "EncodeFileAll") == 0)
        {
            EncodingFileAll();
            return (0);
        }
		else if (strcmp(argv[1], "Autolinebreaking") == 0)
		{
			Autolinebreaking();
			return (0);
		}
		else if (strcmp(argv[1], "AutolinebreakingScript") == 0)
		{
			AutolinebreakingScript();
			return (0);
		}
		else if (strcmp(argv[1], "autolinebreak_shizuku") == 0)
		{
			autolinebreak_shizuku();
			return (0);
		}
		else if (strcmp(argv[1], "CompileExeText30") == 0)
		{
			CompileExeText30();
			return (0);
		}
		else if (strcmp(argv[1], "ConvertFile") == 0)
		{
			ConvertFile();
			return (0);
		}
		else if (strcmp(argv[1], "mono_shizuku") == 0)
		{
			mono_shizuku();
			return (0);
		}
		else if (strcmp(argv[1], "DecodeGFXShizuku") == 0)
		{
			GetSourceData();
			DecompressFile2();
			return (0);
		}
		else if (strcmp(argv[1], "CompileTextFile") == 0)
		{
			CompileTextFile();
			return (0);
		}
		else if (strcmp(argv[1], "vwf_shizuku") == 0)
		{
			text_to_vwf_shizuku();
			return (0);
		}
		else if (strcmp(argv[1], "conv_kiz_tbl") == 0)
		{
			conv_kiz_tbl();
			return (0);
		}
		else if (strcmp(argv[1], "mono_toheart") == 0)
		{
			mono_toheart();
			return (0);
		}
		else if (strcmp(argv[1], "spacing_toheart") == 0)
		{
			spacing_toheart();
			return (0);
		}
		else if (strcmp(argv[1], "uwu") == 0)
		{
			uwu();
			return (0);
		}
        else
        {
            printf("\nUnknown command.\n");
            return (1);
        }
	}

    if(argc < 3)
    {
            printf("\nUsage: [TH_Repack] | [Command] | [Input filename]\n");
            return (1);
    }

    return (0);
}
