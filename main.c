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
char InputFileName[20];
char OutputFileName[20];
unsigned char *SourceData;
static int InputFileSize;
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

    SceneFile = fopen(InputFileName, "rb");
    if (SceneFile == NULL)
    {
        printf("\nCan't find %s file.\n", InputFileName);
        return(1);
    }

    NewPakFile = fopen("LVNS3SCNB.PAK", "wb");

    //Get scene file size
    fseek(SceneFile, 0L, SEEK_END);
    SceneSize = ftell(SceneFile);

    //Get PAK file size
    fseek(PakFile, 0L, SEEK_END);
    PakFileSize = ftell(PakFile);
    fseek(PakFile, 0L, SEEK_SET);

    //Obtain scene ID
    SceneID = atoi(InputFileName);

    //Entry for this scene
    fseek(TableFile, (0x18 * SceneID) + 0x0C, SEEK_SET);

    //////////////////////
    //Deleting old scene//
    //////////////////////

    //Get old position
    fread(&OldScenePosition, sizeof(unsigned long int), 1, TableFile);
    //Get old size
    fread(&OldSceneSize, sizeof(unsigned long int), 1, TableFile);

    CleanSceneBuffer1 = malloc(OldScenePosition * sizeof(char));

    //Copy first part of the pak file
    while (OldScenePosition-- > 0)
    {
        CleanSceneBuffer1[CSBufferPosition++] = fgetc(PakFile);
    }
    //Write contents
    fwrite(CleanSceneBuffer1, sizeof(char), CSBufferPosition, NewPakFile);

    //Old pak file: Skip right after deleted scene
    fseek(PakFile, OldScenePosition + OldSceneSize, SEEK_SET);
    //New pak file: Skip right after copied data
    fseek(NewPakFile, CSBufferPosition, SEEK_SET);

    CSBufferPosition = 0;
    LeftoverSize = PakFileSize - (OldScenePosition + OldSceneSize);

    CleanSceneBuffer2 = malloc(LeftoverSize * sizeof(char));
    //Copy second part of the pak file
    while (LeftoverSize-- > 0)
    {
        CleanSceneBuffer2[CSBufferPosition++] = fgetc(PakFile);
    }
    fwrite(CleanSceneBuffer1, sizeof(char), CSBufferPosition, NewPakFile);

    //////////////////////////////////////
    //Write new scene right before table//
    //////////////////////////////////////

    //Get new scene position
    fseek(NewPakFile, -0x5D18, SEEK_END);
    ScenePosition = ftell(NewPakFile);
    printf("\nScenePosition: %lX\n", ScenePosition);

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
    sprintf(OutputFileName, "%s_Encrypted.scn", InputFileName);
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
    sprintf(OutputFileName, "%s_Encoded.scn", InputFileName);
    EncodedFile = fopen(OutputFileName, "wb");

    fseek(InputFile, 0L, SEEK_END);
    InputFileSize = ftell(InputFile);
    printf("\nEncoded file size 0: %04X\n", InputFileSize);
    fseek(InputFile, 0, SEEK_SET);

    int SDPosition;
    unsigned char InputByte;
    unsigned char ThisCounter;
    unsigned char FByte = 0xFF;

    for (SDPosition = 0; SDPosition < InputFileSize; SDPosition++)
    {
        //Skip size data
        if (SDPosition <= 5)
        ThisCounter = 0;

        if (ThisCounter++ == 8)
        {
        fwrite(&FByte, sizeof(char), 1, EncodedFile);
        ThisCounter = 0;
        InputFileSize++;
        }

        else
        {
        InputByte = fgetc(InputFile);
        fwrite(&InputByte, sizeof(char), 1, EncodedFile);
        }

    }

    fclose(EncodedFile);

    return;
}

void DecryptFile()
{
    FILE *DecFile;
    sprintf(OutputFileName, "%s_Decrypted.scn", InputFileName);
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
    sprintf(OutputFileName, "%s_Noted.scn", InputFileName);
    NotedFile = fopen(OutputFileName, "wb");
    int SDPosition;
    unsigned char NotedByte;

    //printf("\nNoted file size 2: %04X\n", InputFileSize);

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
    int GenCounter = 0;
    int SDPosition = -1;
    int ScenarioSize;
    int CompleteFileSize;

    //Scenario file
    FILE *ScenarioFile;
    sprintf(OutputFileName, "%s_SCENARIO.scn", InputFileName);
    ScenarioFile = fopen(OutputFileName, "rb");
    if (ScenarioFile == NULL)
    {
        printf("\nCan't locate the scenario sauce.\n");
        return(1);
    }

    //Scenario size
    fseek(ScenarioFile, 0L, SEEK_END);
    ScenarioSize = ftell(ScenarioFile);
    fseek(ScenarioFile, 0L, SEEK_SET);

    //File ready to be encrypted
    sprintf(OutputFileName, "%s.scn", InputFileName);
    CompleteFile = fopen(OutputFileName, "wb");

    while (GenCounter++ < ScenarioSize)
    {
    PlaceHolder = fgetc(ScenarioFile);
    fwrite(&PlaceHolder, sizeof(char), 1, CompleteFile);
    }

    //Skip cursor right after scenario file
    fseek(CompleteFile, ScenarioSize, SEEK_SET);

    while (SDPosition++ < InputFileSize)
    {
    PlaceHolder = SourceData[SDPosition];
    fwrite(&PlaceHolder, sizeof(char), 1, CompleteFile);
    }

    //Size of complete file
    fseek(CompleteFile, 0L, SEEK_END);
    CompleteFileSize = ftell(CompleteFile);
    fseek(CompleteFile, 0L, SEEK_SET);

    fclose(CompleteFile);
    CompleteFile = fopen(OutputFileName, "rb");

    SourceData = (unsigned char *)realloc(SourceData, CompleteFileSize * sizeof(char));
    fread(SourceData, sizeof(char), CompleteFileSize, CompleteFile);

    fclose(ScenarioFile);
    fclose(CompleteFile);

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

void DecompressFile()
{
    FILE *DecodedFile;
    //Scene name + "Decoded"

    sprintf(OutputFileName, "%s_Decoded.scn", InputFileName);
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
            EncryptFile();
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

        else if (strcmp(argv[1], "UpdatePakFile") == 0)
        {
            UpdatePakFile();
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
