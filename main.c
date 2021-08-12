//Decode and encode To Heart PAK files.
//By Saki - 2020.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

void GetSourceData()
{
    fseek(InputFile, 0L, SEEK_END);
    InputFileSize = ftell(InputFile);
    fseek(InputFile, 0L, SEEK_SET);
    SourceData = malloc(InputFileSize * sizeof(char));
    fread(SourceData, sizeof(char), InputFileSize, InputFile);

    return;
}

void GetSourceDataGeneral(FILE **SourceFile)
{
    fseek(*SourceFile, 0L, SEEK_END);
    SourceFileSize = ftell(*SourceFile);
    fseek(*SourceFile, 0L, SEEK_SET);
    SourceData = malloc(SourceFileSize * sizeof(char));
    fread(SourceData, sizeof(char), SourceFileSize, *SourceFile);
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

void EncryptDataGeneral(unsigned int SourceFileSize)
{
    char EncryptKey[KEYLEN] = {0xd1, 0x58, 0x6a, 0x56, 0x9a, 0x13, 0xa5, 0xf7, 0x7c, 0x3e, 0x74};
    int KeyPosition;
    int SDPosition;
    int EncryptedByte;

    for (SDPosition = 0, KeyPosition = 0; SDPosition < SourceFileSize; SDPosition++)
    {
        EncryptedByte = (SourceData[SDPosition] + EncryptKey[KeyPosition]) & 0xFF;
        KeyPosition = (KeyPosition + 1) % KEYLEN;
        SourceData[SDPosition] = EncryptedByte;
    }
    return;
}

void EncryptFile()
{
    sprintf(OutputFileName, "%s_Encrypted.ENCRYPTED", InputFileName);
    EncryptedFile = fopen(OutputFileName, "wb");

    char Key[KEYLEN] = {0xd1, 0x58, 0x6a, 0x56, 0x9a, 0x13, 0xa5, 0xf7, 0x7c, 0x3e, 0x74};
    int KeyPosition;
    int SDPosition;

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
    	//SourceData[1] = 1;
        printf("KeyPosition: %02X", KeyPosition);
    }

    printf("KeyPosition3: %02X", KeyPosition);

    fwrite(SourceData, sizeof(char), InputFileSize, EncryptedFile);
    //fclose(EncryptedFile);
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

//Using "cut" method
int AutolinebreakingScript()
{
	//File pointers
	FILE *InputText;
	FILE *OutputText;
	//General variables
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

	InputText = fopen(SupraInputFileName, "r");
	//Exception handler
	if (InputText == NULL)
	{
		printf("\n%s", SupraInputFileName);
		printf("\nText file doesn't exist.\n");
		return(1);
	}
	//Create output file
	OutputText = fopen("ScriptAutolinebroken.txt", "w");
	//Exception handler
	if (OutputText == NULL)
	{
		printf("\nCan't create output file.\n");
		return(1);
	}

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
				if (NewLineCounter == 42)
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
					if (LDTempPosition <= 2)
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
		fputs(LineData, OutputText);
	}
	//We're done with this file
	fclose(InputText);
	fclose(OutputText);
	return(0);
}

__int16 GetShort(unsigned char *Data, int DataPos)
{
	return (Data[DataPos] | Data[DataPos + 1] << 8);
}

__int32 GetLong(unsigned char *Data, int DataPos)
{
	return (Data[DataPos] | Data[DataPos + 1] << 8 | Data[DataPos + 2] << 16 | Data[DataPos + 3] << 24);
}

unsigned char *lzss(unsigned char *LoadBuffer, int SaveSize, int LoadBufferPosition)
{
	static unsigned char *SaveBuffer = 0;
	SaveBuffer = malloc(SaveSize * sizeof(char));
	unsigned char Ring[0x1011];
	memset(Ring, 0, sizeof(Ring));

	int FlagBits;
    unsigned char Flag = 0;
    unsigned char ThisRing, ThisRing2, ThisSaveBuffer, ThisLoadBuffer;
    int RingPosition, SaveBufferPosition;
    int Position, Lenght;
    unsigned short Data;

    SaveBufferPosition = 0; FlagBits = 0; RingPosition = 0x0FEE;

	while (SaveSize > 0)
    {
		//Get flag
        if (FlagBits-- > 0)
        {
            Flag <<= 1;                                 //Shift flag (up to 8 literals between flags)
            printf("\nFlag 1: %02X\n", Flag);
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
            RingPosition &= 0x0FFF;
            SaveSize--;
        }

		else
        {
            //Raw data = 00B2FE
            //Data = 00FEB2
            //Lenght = 000005 (3 + 2 from the data)
            //Position = 000FEB

            //Last position and lenght (2 bytes)
            Data = ~((LoadBuffer[LoadBufferPosition]) + (LoadBuffer[LoadBufferPosition + 1] << 8));
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
                printf("\nActual data: %02X\n", ThisRing2);

                ThisRing = ThisSaveBuffer = ThisRing2;

                SaveBuffer[SaveBufferPosition++] = ThisSaveBuffer;
                Ring[RingPosition++] = ThisRing;

                Position &= 0x0FFF;
                RingPosition &= 0x0FFF;
           }
        }
    }
    return SaveBuffer;
}

void GetFileSize()
{
    fseek(InputFile, 0L, SEEK_END);
    InputFileSize = ftell(CompleteFile);
    fseek(CompleteFile, 0L, SEEK_SET);
}

void AppendFileSize()
{
    InputFileSize = ~(InputFileSize);
    *InputFileSizeArr = InputFileSize;
    char Zero[2] = {0, 0};
    TempFile = fopen("Temp.scn", "wb");

    fwrite(InputFileSizeArr, sizeof(long), 1, InputFile);
    fwrite(Zero, sizeof(char), 2, InputFile);
}

void CompileExeText30()
{
	//Variables
							   //Start	   //Reset	   //Album	   //Font	   //Exit
	long PointerPosition[55] = {0x0002C794, 0x0002C798, 0x0002C79C, 0x0002C7A0, 0x0002C7A4,

							   //Save 1	   //Save 2	   //Save 3	   //Save 4	   //Save 5
							   0x0002C7FC, 0x0002C800, 0x0002C804, 0x0002C808, 0x0002C80C,

							   //New game  //Continue  //Bonus 1   //Bonus 2
							   0x0002C860, 0x0002C864, 0x0002C868, 0x0002C86C,

							   //Hide text //Load game //Save game //Log	   //Change name
							   0x0002C91C, 0x0002C920, 0x0002C924, 0x0002C928, 0x0002C92C,

							   //Change    //Save in   //Return to //Go back one
							   //font 	   //album	   //main menu //choice	     //Love points
							   0x0002C930, 0x0002C934, 0x0002C938, 0x0002C93C,   0x0002C940,

							   	   	   	   	   	   	   //Will return //Will return  //Will save
							   //Will load //Will save //one choice  //to main menu	//to album
							   0x00016DEC, 0x00016EF1, 0x00017181, 	 0x0001714D,    0x00016FD6,

							   //Album is //Album will //Album was //Same
							   //full	  //be reset   //reset	   //photo     //Are you sure? 1
							   0x00016FC2, 0x0000117C, 0x00001215, 0X0001713C, 0x0000119E,

							   //Are you  //Are you
							   //sure? 2  //sure? 3    //Yes	   //No		   //MIDI
							   0x0000E672, 0x00019D2F, 0x0002CA84, 0X0002CA88, 0x0002CACC,

							   	   	   	   	   	   	   //Select	   //Select	   //Select
							   //Arranged  //Disabled  //saveslot  //loadslot  //resetslot
							   0x0002CAD0, 0x0002CAD4, 0x0002CB80, 0x0002CB84, 0x0002CB88,

							   //Will reset  //All data
							   //all data    //was reset  //Akari	 //Serika	 //Tomoko
							   0x000126CD,   0x0001277E,  0x0002D2B0, 0x0002D2B4, 0x0002D2B8,

							   //Shiho	   //Aoi	   //Multi     //Kotone	   //Lemmy
							   0x0002D2BC, 0x0002D2C0, 0x0002D2C4, 0x0002D2C8, 0x0002D2CC,

							   //Love points
							   0x00012B14

							   };

	//OPCODES
	int SpaceSpace = 0x0000;
	int DoublePoint = 0x4400;
	int Bar = 0x5A00;
	char Nope = 0x90;
	char EndBlock = 0xAF;
	//LETTERS
	int N_STRING = 0x000F;
	int O_STRING = 0x0010;
	int D_STRING = 0x0005;
	int A_STRING = 0x0002;
	int T_STRING = 0x0015;

	long Pointer = 0;
	int AddOn = 0;
    int BlockCounter = 0;
    int TotalBlocks = 0;
    int OffsetsDataSize = 0;
    int DatafileSize = 0;
    unsigned char* Data;
    FILE *ExeFile;
	ExeFile = fopen("LVNS3.exe", "r+b");
	FILE *DataFile;
	DataFile = fopen("EXE_TEXT_TRANSLATED_30.BIN", "rb");

	//Get number of blocks
    fread(&TotalBlocks, sizeof(char), 2, DataFile);

    //Get offsets data size
    OffsetsDataSize = 2 + (TotalBlocks * 2);

    //Datafile size
    fseek(DataFile, 0L, SEEK_END);
    DatafileSize = ftell(DataFile);
    fseek(DataFile, 0L, SEEK_SET);
    //Without offsets
    DatafileSize -= OffsetsDataSize;

    //Read text data
    Data = malloc(DatafileSize * sizeof(char));
	fseek(DataFile, OffsetsDataSize, SEEK_SET);
    fread(Data, sizeof(char), DatafileSize, DataFile);
	fseek(DataFile, 0L, SEEK_SET);

	//Write datafile to exe, at the beginning of the kanji table
	fseek(ExeFile, 0x2F24A, SEEK_SET);
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

	//"No data" string (no pointer/hardcoded)
	fseek(ExeFile, 0x0002D1EF, SEEK_SET);
	fwrite(&N_STRING, sizeof(char), 2, ExeFile);
	fwrite(&O_STRING, sizeof(char), 2, ExeFile);
	fwrite(&SpaceSpace, sizeof(char), 2, ExeFile);
	fwrite(&D_STRING, sizeof(char), 2, ExeFile);
	fwrite(&A_STRING, sizeof(char), 2, ExeFile);
	fwrite(&T_STRING, sizeof(char), 2, ExeFile);
	fwrite(&A_STRING, sizeof(char), 2, ExeFile);
	fwrite(&SpaceSpace, sizeof(char), 2, ExeFile);
	//fwrite(&SpaceSpace, sizeof(char), 2, ExeFile);
	fwrite(&EndBlock, sizeof(char), 1, ExeFile);
	fwrite(&EndBlock, sizeof(char), 1, ExeFile);

	//Caption
	fseek(ExeFile, 0x0002D544, SEEK_SET);
	fprintf(ExeFile, "To Heart - Translated by Tomoko's LC              ");

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

	//Write pointers
	while(BlockCounter != TotalBlocks)
	{
		fseek(DataFile, 2 + (BlockCounter * 2), SEEK_SET);
		fread(&AddOn, sizeof(char), 2, DataFile);
		Pointer = (0x42F24A + AddOn) - OffsetsDataSize;
		fseek(ExeFile, PointerPosition[BlockCounter], SEEK_SET);
		fwrite(&Pointer, sizeof(char), 4, ExeFile);
	    fseek(ExeFile, 0L, SEEK_SET);
		BlockCounter++;
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

    int ScenarioStartPosition;
    int TextStartPosition;
    int ScenarioSize;
    int TextSize;
    unsigned char *ScenarioData;
    unsigned char *TextData;

    ScenarioStartPosition = GetShort(SourceData, 0) * 16;
    TextStartPosition = GetShort(SourceData, 2) * 16;

    ScenarioSize = GetLong(SourceData, ScenarioStartPosition);
    TextSize = GetLong(SourceData, TextStartPosition);

    int Temp;
    int TempLenght;
    unsigned char *TempData;

    TempLenght = TextStartPosition - ScenarioStartPosition;

    //Write header
    fwrite(&ScenarioStartPosition, sizeof(short), 1, DecodedFile); //Scenario start position
    fwrite(&TempLenght, sizeof(short), 1, DecodedFile);            //Text start position
    fwrite(&ScenarioSize, sizeof(short), 1, DecodedFile);          //Scenario size
    fwrite(&TextSize, sizeof(long), 1, DecodedFile);               //Text size
    fseek(DecodedFile, 0x10, SEEK_SET);                             //Skip cursor right after header

    TempData = malloc(TempLenght * sizeof(char));
    for (Temp = 0; Temp < TempLenght; Temp++)
    {
        TempData[Temp] = SourceData[ScenarioStartPosition + Temp];
    }

    ScenarioData = malloc(ScenarioSize * sizeof(char));
    ScenarioData = lzss(TempData, ScenarioSize, 4);
    fwrite(ScenarioData, sizeof(char), ScenarioSize, DecodedFile);

    //Free memory
    free(TempData);
    free(ScenarioData);

    //Skip cursor right after the scenario file
    fseek(DecodedFile, ScenarioSize + 0x10 + (0x10 - (ScenarioSize & 0x0F)), SEEK_SET);

    fseek(DecodedFile, ScenarioSize, SEEK_SET);

    TempLenght = InputFileSize - TextStartPosition;
    TempData = malloc(TempLenght * sizeof(char));
    for (Temp = 0; Temp < TempLenght; Temp++)
    {
        TempData[Temp] = SourceData[TextStartPosition + Temp];
    }

    TextData = malloc(TextSize * sizeof(char));
    TextData = lzss(TempData, TextSize, 4);
    fwrite(TextData, sizeof(char), TextSize, DecodedFile);

    //Filler bytes at the end of file
    int ExtraZeroes = 0;
    fwrite(&ExtraZeroes, sizeof(char), (0x10 - (TextSize & 0x0F)), DecodedFile);

    //Free memory
    free(TempData);
    free(TextData);
    free(SourceData);
    return;
}

int ConvertFile()
{
	//Using little endian
	int fonttable1[94] = 	{
							//A		//B		//C		//D
							0x0200, 0x0300, 0x0400, 0x0500,
							//E		//F		//G		//H
							0x0600, 0x0700, 0x0800, 0x0900,
							//I		//J		//K		//L
							0x0a00, 0x0b00, 0x0c00, 0x0d00,
							//M		//N		//O		//P
							0x0e00, 0x0f00, 0x1000, 0x1100,
							//Q		//R		//S		//T
							0x1200, 0x1300, 0x1400, 0x1500,
							//U		//V		//W		//X
							0x1600, 0x1700, 0x1800, 0x1900,
							//Y		//Z		//a		//b
							0x1A00, 0x1B00, 0x1C00, 0x1D00,
							//c		//d		//e		//f
							0x1E00, 0x1F00, 0x2000, 0x2100,
							//g		//h		//i		//j
							0x2200, 0x2300, 0x2400, 0x2500,
							//k		//l		//m		//n
							0x2600, 0x2700, 0x2800, 0x2900,
							//o		//p		//q		//r
							0x2A00, 0x2B00, 0x2C00, 0x2D00,
							//s		//t		//u		//v
							0x2E00, 0x2F00, 0x3000, 0x3100,
							//w		//x		//y		//z
							0x3200, 0x3300, 0x3400, 0x3500,
							//0		//1		//2		//3
							0x3600, 0x3700, 0x3800, 0x3900,
							//4		//5		//6		//7
							0x3a00, 0x3b00, 0x3c00, 0x3d00,
							//8		//9		//(		//)
							0x3e00, 0x3f00, 0x4000, 0x4100,
							//?		//!		//:		//-
							0x4200, 0x4300, 0x4400, 0x4500,
							//.		//,		//'		//*
							0x4600, 0x4700, 0x4800, 0x4900,
							//@		//>		//|		//" "
							0x94B2, 0x94B0, 0x32B7, 0x0000,
							//"		//%		//á		//í
							0x4A00, 0X4B00, 0x4C00, 0x4D00,
							//ú		//é		//ó		//ñ
							0x4E00, 0X4F00, 0x5000, 0x5100,
							//Á		//Í		//Ú		//É
							0x5200, 0X5300, 0x5400, 0x5500,
							//Ó		//Ñ		//¡		//¿
							0x5600, 0X5700, 0x5800, 0x5900,
							//Slash
							0xAFB3
							};


	char fonttable2[94][5] = 	{
								"A", "B", "C", "D",
								"E", "F", "G", "H",
								"I", "J", "K", "L",
								"M", "N", "O", "P",
								"Q", "R", "S", "T",
								"U", "V", "W", "X",
								"Y", "Z", "a", "b",
								"c", "d", "e", "f",
								"g", "h", "i", "j",
								"k", "l", "m", "n",
								"o", "p", "q", "r",
								"s", "t", "u", "v",
								"w", "x", "y", "z",
								"0", "1", "2", "3",
								"4", "5", "6", "7",
								"8", "9", "(", ")",
								"?", "!", ":", "-",
								".", ",", "'", "*",
								"@", ">", "|", " ",
								"\"", "%", "$", "$",
								"$", "$", "$", "$",
								"$", "$", "$", "$",
								"$", "$", "$", "$",
								"\\"
								};

	int fonttablesp[15] = 		{
								//á		//í		//ú		//é		//ó		//ñ
								0xA1C3, 0xADC3, 0xBAC3, 0xA9C3, 0xB3C3, 0xB1C3,
								//Á		//Í		//Ú		//É		//Ó		//Ñ
								0x81C3, 0x8DC3, 0x9AC3, 0x89C3, 0x93C3, 0x91C3,
								//¡		//¿
								0xA1C2, 0xBFC2
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
								"ClearScreen", //EX Nazo44
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
								"FadeWhiteIn", //EX Nazo71
								"Nazo72",
								"Brighten",
								"Nazo74",
								"Nazo75",
								"Nazo76",
								"WhiteOut",
								"WhiteIn",
								"ClearText", //EX Nazo79
								"DisplayText", //EX Nazo7A
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

	//File pointers
	FILE *InputText;
	FILE *TableFile;
    FILE *NEWLVNS3PAK;
	//Char variables
	char Catcher = '0';
	const char Space[2] = " ";
	//Fixed arrays
	char Zero[16] = {	1, 0, 0, 0,
						0, 0, 0, 0,
						0, 0, 0, 0,
						0, 0, 0, 0 };
    char Key[11] = {0xd1, 0x58, 0x6a, 0x56, 0x9a, 0x13, 0xa5, 0xf7, 0x7c, 0x3e, 0x74};
	char LineData[0xFFF];
	char NextLine[0xFFF];
    //Arrays
	char *Tokens;
	char *Text;
	char *LabelOffsets;
	char *OpcodeString;
	char *CharString;
	char *Characters;
    char *TableData;
    char *ExpandedData;
    char *FullData;
    char *ScenarioData;
	//Array indexes
	int TextPosition = 0;
    int TextPositionBK = 0;
	int LDPosition = 0;
	int LOPosition = 0;
	int OpcodeIndex = 0;
	int OpcodeIndexBackup = 0;
	int FontIndex = 0;
	int FontIndexBackup = 0;
    int KeyPosition = 0;
    int SDPosition = 0;
    int SPChar = 0;
    int NMChar = 0;
	//Counters
	int FileCount = -1;
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
	int ScenarioStructureFlag = 0;

    //////////////////////////////////////ALLOCATE MEMORY//////////////////////////////////////
	Tokens = malloc(0xFFFF * sizeof(char));
    Text = malloc(0xFFFF * sizeof(char));
    LabelOffsets = malloc(0xFFFF * sizeof(char));
    OpcodeString = malloc(0xFFFF * sizeof(char));
    CharString = malloc(0xFFFF * sizeof(char));
    Characters = malloc(0xFFFF * sizeof(char));

    //Readable data for BOTH text and scenario
    FullData = malloc(0xFFFFFF * sizeof(char));
    //"Compressed" data for BOTH text and scenario
    ExpandedData = malloc(0xFFFFFF * sizeof(char));
    //Table data
    TableData = malloc(0xFFFF * sizeof(char));
    //Scenario data
    ScenarioData = malloc(0xFFFFFF * sizeof(char));
    //////////////////////////////////////ALLOCATE MEMORY//////////////////////////////////////

    //////////////////////////////////////OPEN FILES//////////////////////////////////////
	//Open LVNS3.PAK
	NEWLVNS3PAK = fopen("LVNS3SCN.PAK", "rb+");
	if (NEWLVNS3PAK == NULL)
	{
		printf("\nCan't find LVNS3.PAK\n");
		return(1);
	}
	//Open table file
    TableFile = fopen("TABLE.TABLE", "rb+");
	if (TableFile == NULL)
	{
		printf("\nCan't find table\n");
		return(1);
	}
    fread(TableData, sizeof(char), 0x5D18, TableFile);
    //Open script
	InputText = fopen("ScriptAutolinebroken.txt", "r");
	//Exception handler
	if (InputText == NULL)
	{
		printf("\nScript doesn't exist\n");
		return(1);
	}
    //////////////////////////////////////OPEN FILES//////////////////////////////////////

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
	while (Lines--)
	{
		//Update reverse lines
		ReverseLines++;

		//Get line
		fgets(LineData, 0xFFF, InputText);

		//Ignore empty line
		if(strcmp(LineData, "\n") == 0 || strcmp(LineData,"\r\n") == 0)
			continue;

		//Divide the line in tokens
		Tokens = strtok(LineData, Space);

		//Remove newline
		Tokens[strcspn(Tokens, "\n")] = 0;

		//Get file numbers
		if (strcmp(Tokens, "ScenarioStart") == 0)
		{
			//Structure check integrity
			if (ScenarioStructureFlag != 0)
			{
				printf("\nERROR: Structure broken at scene %02X, line %d", FileNumber, ReverseLines);
				return(1);
			}
			ScenarioStructureFlag++;

			Tokens = strtok(NULL, Space);
			FileNumber = strtol(Tokens, NULL, 16);
			Tokens = strtok(NULL, Space);
			FileCount = strtol(Tokens, NULL, 10);
			continue;
		}

		//////////////////////////////////////WRITE CONTENTS//////////////////////////////////////
		//Write contents and reset whenever an entry ends
		if ((strcmp(Tokens, "TextEnd") == 0) || (strcmp(Tokens, "ScenarioEnd") == 0))
		{
			//Update label offsets
			while((LabelCounterBackup / 2) != LabelCounter)
			{
				//Get label
				memcpy(&Label, LabelOffsets + LabelCounterBackup, 2);
				//Add offset
				Label += (LabelCounter * 2) + 2;
				//Copy back
				memcpy(LabelOffsets + LabelCounterBackup, &Label, 2);
				LabelCounterBackup += 2;
			}
			//Join label counter, offsets and text
			memcpy(FullData, &LabelCounter, 2);
			memcpy(FullData + 2, LabelOffsets, LabelCounter * 2);
			memcpy(FullData + 2 + (LabelCounter * 2), Text, TextPosition);
			//Get full data size
			FullDataSize = 2 + (LabelCounter * 2) + TextPosition;
			//Get negative file size
	        NegativeBlockSize = ~(FullDataSize);
			//Get encoded file size
			FullDataSize += (FullDataSize / 8) + 5;
			//Write text
	        for (SDPosition = 4, IT = 0, TextPositionBK = 0; SDPosition != FullDataSize; SDPosition++, IT++, TextPositionBK++)
	        {
	            if (IT % 9 == 0)
	            {
	            	 ExpandedData[SDPosition] = 0xFF;
	            	 TextPositionBK--;
	            }
	            else
	                ExpandedData[SDPosition] = FullData[TextPositionBK];
	        }

	        //Write size
	        memcpy(ExpandedData, &NegativeBlockSize, 4);

	        //NOT
	        for (SDPosition = 0; SDPosition != FullDataSize; SDPosition++)
	        {
	            ExpandedData[SDPosition] = ~(ExpandedData[SDPosition]);
	        }

			//Write header and pad scenario file with zeros
			if (strcmp(Tokens, "ScenarioEnd") == 0)
			{
				//Structure check integrity
				if (ScenarioStructureFlag != 1)
				{
					printf("\nERROR: Structure broken at scene %02X, line %d", FileNumber, ReverseLines);
					return(1);
				}
				ScenarioStructureFlag++;

				//Move data to next row
				memcpy(ExpandedData + 0x10, ExpandedData, FullDataSize);
				FullDataSize += 0x10;
				//Pad until end of row
				while((FullDataSize % 0x10) != 0)
				{
					memcpy(ExpandedData + FullDataSize, &Zero[2], 1);
					FullDataSize++;
				}
				//Fill header
				memcpy(ExpandedData, &Zero, 0x10);
				//Reduce data size for header
				HeaderDataSize = FullDataSize / 0x10;
				//Write text start offset
				memcpy(ExpandedData + 2, &HeaderDataSize, 1);
				//Save in scenario file
				memcpy(ScenarioData, ExpandedData, FullDataSize);
				//Save size
				ScenarioSize = FullDataSize;
				FinalDataSize += FullDataSize;
				goto RESET_STUFF;
			}

			if (strcmp(Tokens, "TextEnd") == 0)
			{
				//Structure check integrity
				if (ScenarioStructureFlag != 2)
				{
					printf("\nERROR: Structure broken at scene %02X, line %d", FileNumber, ReverseLines);
					return(1);
				}
				ScenarioStructureFlag = 0;

				//Pad until the end of the row
				while((FullDataSize % 0x10) != 0)
				{
					memcpy(ExpandedData + FullDataSize, &Zero[2], 1);
					FullDataSize++;
				}
				//Get final size
				FinalDataSize += FullDataSize;
				//Copy scenario
				memcpy(ExpandedData + ScenarioSize, ExpandedData, FullDataSize);
				memcpy(ExpandedData, ScenarioData, ScenarioSize);
		        //Encrypt
		        for (SDPosition = 0, KeyPosition = 0; SDPosition < FinalDataSize; SDPosition++, KeyPosition++)
		        {
		        	ExpandedData[SDPosition] = (ExpandedData[SDPosition] + Key[KeyPosition % 11]) & 0xFF;
		        }

		        //////////////////////////////////////TABLE BLOCK//////////////////////////////////////
		        //Get property position from last position
		        PropertyPosition = LastPosition;
		        //Initial base if zero
		        if (PropertyPosition == 0x00000000)
		            PropertyPosition = 0x0011912A;
		        //Get head of the next file
		        HeadOfNextFile = PropertyPosition + FinalDataSize;
		        //Write to table
		        memcpy(TableData + (24 * FileCount) + 12, &PropertyPosition, 4);
		        memcpy(TableData + (24 * FileCount) + 16, &FinalDataSize, 4);
		        memcpy(TableData + (24 * FileCount) + 20, &HeadOfNextFile, 4);
		        //Save last position
		        LastPosition = HeadOfNextFile;
		        //////////////////////////////////////TABLE BLOCK//////////////////////////////////////

		        //////////////////////////////////////UPDATE PAK//////////////////////////////////////
				//Write to LVNS3PAK
		        //IMPORTANT IMPORTANT IMPORTANT
		        //YOU NEED AT LEAST 3 MB OF SPACE IN THE PAK
		        //THANK YOU
				fseek(NEWLVNS3PAK, PropertyPosition, SEEK_SET);
				fwrite(ExpandedData, sizeof(char), FinalDataSize, NEWLVNS3PAK);
		        //////////////////////////////////////UPDATE PAK//////////////////////////////////////
				//Reset final data size
				FinalDataSize = 0;
				goto RESET_STUFF;
			}
	        //////////////////////////////////////RESET STUFF//////////////////////////////////////
			RESET_STUFF:
			TextPositionBK = 0;
			NegativeBlockSize = 0;
			SDPosition = 0;
			KeyPosition = 0;
			IT = 0;
			FullDataSize = 0;
			HeaderDataSize = 0;
			LabelCounter = 0;
			LabelCounterBackup = 0;
			LOPosition = 0;
			TextPosition = 0;
			memset(LabelOffsets, 0, 0xFFFF * sizeof(char));
			memset(Text, 0, 0xFFFF * sizeof(char));
	        //////////////////////////////////////RESET STUFF//////////////////////////////////////
			continue;
		}

		//LABEL
		if (strncmp(LineData, "*", 1) == 0)
		{
			LabelCounter++;
	        memcpy(LabelOffsets + LOPosition, &TextPosition, 2);
			LOPosition += 2;
			continue;
		}

		//Text Handler
		if (strcmp(Tokens, "`") == 0)
		{
			//Starting after "`"
			LDPosition = 2;

			while (strncmp(LineData + LDPosition, "\n", 1) != 0)
			{
				Characters[0] = LineData[LDPosition];

		        //////////////////////////////////////PSEUDO-UNICODE BLOCK//////////////////////////////////////
		        //Catch Unicode char
				if ((Characters[0] & 0xC0) == 0xC0)
		        {
		        	//Read next character
		        	memcpy(&NMChar, LineData + LDPosition, 2);
		        	//Obtain font index
		        	while (NMChar != SPChar)
					{
		        		memcpy(&SPChar, fonttablesp + FontIndex, 2);
		        		FontIndex++;
					}
		        	//Compensate for 0 slot
					FontIndex--;
					//Catch wrong character
					if (FontIndex >= 16)
					{
						printf("\nERROR: Can't find special character in scene %02X, line %d", FileNumber, ReverseLines);
						return(1);
					}
					//Copy char
					memcpy(Text + TextPosition, &fonttable1[FontIndex+78], 2);
					TextPosition += 2;
		        	LDPosition += 2;
		        	NMChar = 0;
		        	SPChar = 0;
					goto RESET_INDEXES;
		        }
		        //////////////////////////////////////PSEUDO-UNICODE BLOCK//////////////////////////////////////

				//Use the same index with repeated characters
				if (strncmp(Characters, CharString, 1) == 0)
				{
					FontIndex = FontIndexBackup;
					goto ADJUST_FONT_INDEX;
				}

				while (strncmp(Characters, CharString, 1) != 0)
				{
					CharString = fonttable2[FontIndex];
					FontIndex++;
				}
				FontIndexBackup = FontIndex;

				ADJUST_FONT_INDEX:
				//-1 to compensate for slot "0"
				FontIndex--;

				//Catch wrong character
				if (FontIndex >= 81)
				{
					if (FontIndex == 92)
						goto COPYSHIT;

					printf("\nERROR: Can't find character '%02X' ('%c') in line %d, column %d. FI: %02X", Characters[0], Characters[0], ReverseLines, LDPosition, FontIndex);
					return(1);
				}
				COPYSHIT:
				memcpy(Text + TextPosition, &fonttable1[FontIndex], 2);
				TextPosition += 2;
		        LDPosition++;
		        RESET_INDEXES:
		        FontIndex = 0;
			}
			LastLinePosition = ftell(InputText);
			fgets(NextLine, 0xFFF, InputText);
			//Remove newline
			NextLine[strcspn(NextLine, "\n")] = 0;
			//Don't write newline if next opcode is waitpage
			if (strcmp(NextLine, "WaitPage") != 0)
				Text[TextPosition++] = 0xB0;
			//Restore last position
			fseek(InputText, LastLinePosition, SEEK_SET);
			continue;
		}

		//Ignore comments
		if (strncmp(Tokens, ";;", 2) == 0)
			continue;

		//OPCODES
		//Remove newline
		Tokens[strcspn(Tokens, "\n")] = 0;

		//Use same index if duplicated
		if (strcmp(Tokens, OpcodeString) == 0)
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

		//ARGUMENTS
		while (Tokens != NULL)
		{
			Tokens = strtok(NULL, Space);
			//Skip literal argument if NULL
			if (Tokens == NULL)
				goto SKIP_CHECK;
			//Remove newline
			Tokens[strcspn(Tokens, "\n")] = 0;
			//printf("\nArg: %s", Tokens);
			if (strcmp(Tokens, "AkariPoints") == 0)
			{
				strcpy(Tokens, "14");
			}
			else if (strcmp(Tokens, "WhiteBG") == 0)
			{
				strcpy(Tokens, "fe");
			}

			SKIP_CHECK:

			Text[TextPosition] = strtol(Tokens, NULL, 16);
			TextPosition++;
		}
		//Skip last unnecessary token
		TextPosition--;
	}
	fseek(TableFile, 0, SEEK_SET);
	fwrite(TableData, sizeof(char), 0x5D18, TableFile);
	//Encrypt table
    for (SDPosition = 0, KeyPosition = 0; SDPosition < 0x5D18; SDPosition++, KeyPosition++)
    {
    	TableData[SDPosition] = (TableData[SDPosition] + Key[KeyPosition % 11]) & 0xFF;
    }
	fseek(NEWLVNS3PAK, -0x5D18, SEEK_END);
	fwrite(TableData, sizeof(char), 0x5D18, NEWLVNS3PAK);
	fclose(NEWLVNS3PAK);
	fclose(TableFile);
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
    	//InputFileName[strlen(InputFileName) - 9] = 0;

        if (strcmp(argv[1], "DecodeFile") == 0)
        {
            GetSourceData();
            DecryptFile();
            DecompressFile();
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
        else if (strcmp(argv[1], "AppendFile") == 0)
        {
            GetFileSize();
            AppendFileSize();
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
