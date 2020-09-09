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

int UpdatePakFile()
{
    FILE *SceneFile;
    FILE *PakFile;
    FILE *NewPakFile;
    FILE *TableFile;

    int SceneID;
    unsigned long int PakFileSize;
    unsigned int TableFileSize = 0x5D18;
    unsigned long int LeftoverSize;
    unsigned long int SceneSize;
    unsigned long int ScenePosition;
    unsigned long int OldSceneSize;
    unsigned long int OldScenePosition;
    unsigned char* CleanSceneBuffer1;
    unsigned char* CleanSceneBuffer2;
    int CSBufferPosition = 0;

    PakFile = fopen("LVNS3SCN.PAK", "rb");
    if (PakFile == NULL)
    {
        printf("\nCan't find PAK file.\n");
        return(1);
    }

    TableFile = fopen("TABLE.SCN", "rb+");
    if (TableFile == NULL)
    {
        printf("\nCan't find TABLE file.\n");
        return(1);
    }

    strcpy(EncInputFileName, InputFileName);
    EncInputFileName[strlen(EncInputFileName) - 5] = 0;
    sprintf(EncInputFileName, "%s_Encrypted.scn", EncInputFileName);
    SceneFile = fopen(EncInputFileName, "rb");

    if (SceneFile == NULL)
    {
        printf("\nCan't find %s file.\n", EncInputFileName);
        return(1);
    }

    NewPakFile = fopen("LVNS3SCNB.PAK", "rb+");

    //Get scene file size
    fseek(SceneFile, 0L, SEEK_END);
    SceneSize = ftell(SceneFile);
    printf("\nSceneSize: %lX\n", SceneSize);

    //Get PAK file size
    fseek(PakFile, 0L, SEEK_END);
    PakFileSize = ftell(PakFile);
    fseek(PakFile, 0L, SEEK_SET);
    printf("\nPakFileSize: %lX\n", PakFileSize);

    //Obtain scene ID
    //SceneID = atoi(InputFileName);
    SceneID = 0x012A;
    printf("\nSCENE ID: %02X\n", SceneID);

    //Entry for this scene
    fseek(TableFile, (0x18 * SceneID) + 0x0C, SEEK_SET);

    //////////////////////
    //Deleting old scene//
    //////////////////////

    //Get old position
    //fread(&OldScenePosition, sizeof(unsigned long int), 1, TableFile);
    //Get old size
    //fread(&OldSceneSize, sizeof(unsigned long int), 1, TableFile);
    //CleanSceneBuffer1 = malloc(OldScenePosition * sizeof(char));
    //Copy first part of the pak file
    //while (OldScenePosition-- > 0)
    //{
    //    CleanSceneBuffer1[CSBufferPosition++] = fgetc(PakFile);
    //}
    //Write contents
    //fwrite(CleanSceneBuffer1, sizeof(char), CSBufferPosition, NewPakFile);

    //Old pak file: Skip right after deleted scene
    //fseek(PakFile, OldScenePosition + OldSceneSize, SEEK_SET);
    //New pak file: Skip right after copied data
    //fseek(NewPakFile, CSBufferPosition, SEEK_SET);

    //CSBufferPosition = 0;
    //LeftoverSize = PakFileSize - (OldScenePosition + OldSceneSize);

    //CleanSceneBuffer2 = malloc(LeftoverSize * sizeof(char));
    //Copy second part of the pak file
    //while (LeftoverSize-- > 0)
    //{
    //    CleanSceneBuffer2[CSBufferPosition++] = fgetc(PakFile);
    //}
    //fwrite(CleanSceneBuffer1, sizeof(char), CSBufferPosition, NewPakFile);

    //////////////////////////////////////
    //Write new scene right before table//
    //////////////////////////////////////

    //Get new scene position
    fseek(NewPakFile, -0x5D18, SEEK_END);
    ScenePosition = ftell(NewPakFile);
    printf("\nNew scene position: %lX\n", ScenePosition);

    //Write new scene
    GetSourceDataGeneral(&SceneFile);
    fwrite(SourceData, sizeof(char), SceneSize, NewPakFile);
    //The table always go at the end
    fseek(NewPakFile, 0L, SEEK_END);

    //////////////////
    //Updating table//
    //////////////////

    //Entry for this scene
    fseek(TableFile, (0x18 * SceneID) + 0x0C, SEEK_SET);
    //Write position
    fwrite(&ScenePosition, sizeof(unsigned long int), 1, TableFile);
    //Write size
    fwrite(&SceneSize, sizeof(unsigned long int), 1, TableFile);
    //Next
    fseek(TableFile, -0x18, SEEK_CUR);
    //Position of the next file
    //IMPORTANT: This must be backwards. The current scene position
    //is copied into the previous entry.
    fwrite(&ScenePosition, sizeof(unsigned long int), 1, TableFile);

    //Get table data
    SourceData = (unsigned char *)realloc(SourceData, TableFileSize * sizeof(char));
    GetSourceDataGeneral(&TableFile);
    EncryptDataGeneral(TableFileSize);

    //Write to PAK file
    fwrite(SourceData, sizeof(char), TableFileSize, NewPakFile);
    return (0);
}


void EncryptFile()
{
    sprintf(OutputFileName, "%s_Encrypted.ENCRYPTED", InputFileName);
    EncryptedFile = fopen(OutputFileName, "wb");

    char EncryptKey[KEYLEN] = {0xd1, 0x58, 0x6a, 0x56, 0x9a, 0x13, 0xa5, 0xf7, 0x7c, 0x3e, 0x74};
    int KeyPosition;
    int SDPosition;
    int EncryptedByte;

    for (SDPosition = 0, KeyPosition = 0; SDPosition < InputFileSize; SDPosition++)
    {
        EncryptedByte = (SourceData[SDPosition] + EncryptKey[KeyPosition]) & 0xFF;
        KeyPosition = (KeyPosition + 1) % KEYLEN;
        SourceData[SDPosition] = EncryptedByte;
        fwrite(&EncryptedByte, sizeof(char), 1, EncryptedFile);
        fwrite(&EncryptedByte, sizeof(char), 1, CompleteFile);
    }

    fclose(EncryptedFile);
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

    int SDPosition;
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

    int TableInitOffset = 0xE40;
    int SceneNumber = 145;

    char AkariFileList[145][20] = {"0100", "0101", "0102", "0103", "0104", "0105", "0106", "0107", "0108", "0109",
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
                                    "01CB", "01CC", "01CD", "01CE", "01CF"};

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
    FILE *TestFile;

    //File reading variables
    int SceneCounter = 0;
    char SceneFileName[20];
    char ScenarioFileName[20];
    char FileNumber[20];
    char TestFileName[20];

    //Arrays
    unsigned char *SourceData;
    unsigned char *ScenarioData;
    unsigned char *CompleteData;
    unsigned char *PropertyData;
    PropertyData = malloc(24 * sizeof(char));
    unsigned char *PropertyBlock;
    PropertyBlock = malloc(0x0D98 * sizeof(char));
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
                SourceData[SDPosition] = fgetc(InputFile);
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

        //Close input file
        fclose(InputFile);

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
        {
            CompleteData[SDPosition] = (CompleteData[SDPosition] + Key[KeyPosition % 11]) & 0xFF;
        }

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
        if (strcmp(FileNumber, "01CF") == 0)
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
            //fclose(TableFile);
            //Write property block to table
            memcpy(TableData + TableInitOffset, PropertyBlock, 0x0D98);
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
    FILE *DecFile;
    sprintf(OutputFileName, "%s_Decrypted.DECRYPTED", InputFileName);
    DecFile = fopen(OutputFileName, "wb");

    char DecryptKey[KEYLEN] = {0xd1, 0x58, 0x6a, 0x56, 0x9a, 0x13, 0xa5, 0xf7, 0x7c, 0x3e, 0x74};
    int KeyPosition;
    int SDPosition;
    int DecryptedByte;

    for (SDPosition = 0, KeyPosition = 0; SDPosition < InputFileSize; SDPosition++)
    {
        DecryptedByte = (SourceData[SDPosition] - DecryptKey[KeyPosition]) & 0xFF;
        KeyPosition = (KeyPosition + 1) % KEYLEN;
        SourceData[SDPosition] = DecryptedByte;
        fwrite(&DecryptedByte, sizeof(char), 1, DecFile);
    }

    return;
}

void DoNot()
{
    FILE *NotedFile;
    sprintf(OutputFileName, "%s.NOTED", InputFileName);
    NotedFile = fopen(OutputFileName, "wb");

    int SDPosition;
    unsigned char NotedByte;

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

    //Filler bytes at the end of file
    //int ExtraZeroes = 0;
    //int ExtraZeroesAmount = 0x10 - (CompleteFileSize & 0x0F);
    //fwrite(&ExtraZeroes, sizeof(char), ExtraZeroesAmount, CompleteFile);
    //CompleteFileSize = CompleteFileSize + ExtraZeroesAmount;

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
	int SceneNumber = 145;
	char AkariFileList[145][20] =  {"0100", "0101", "0102", "0103", "0104", "0105", "0106", "0107", "0108", "0109",
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
									"01CB", "01CC", "01CD", "01CE", "01CF"};
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
	int LetterCounter = 0;

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
			printf("\nText file doesn't exist.\n");
			return(1);
		}
		//Create output file
		sprintf(OutputSceneFileName, "%s_TEXT_TRANSLATED_AUTOLINEBROKEN.TXT", FileNumber);
		OutputText = fopen(OutputSceneFileName, "w");
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
        	printf("\nFile doesn't exist.\n");
        	return(1);
    	}
    	strcpy(InputFileName, argv[2]);
    	InputFileName[strlen(InputFileName) - 9] = 0;

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
        else if (strcmp(argv[1], "UpdatePakFile") == 0)
        {
            UpdatePakFile();
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
