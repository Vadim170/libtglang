#include "tglang.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h> // Include the sys/stat.h header for S_ISDIR and S_ISREG
#include <dlfcn.h> 
#include <chrono>

#define LIB_PATH "submission/libtglang.dylib"

// Struct to store file information
typedef struct {
	char path[1024]; // Adjust this size as needed
	int language;
} FileInfo;

// Struct to store language count and percentage
typedef struct {
	const char *name;
	int count;
	float percentage;
} LanguageInfo;

// Function to compare two LanguageInfo structs for sorting
int compareLanguageInfo(const void *a, const void *b) {
	const LanguageInfo *langInfoA = (const LanguageInfo *)a;
	const LanguageInfo *langInfoB = (const LanguageInfo *)b;
	return (int)(langInfoB->percentage - langInfoA->percentage);
}


FileInfo fileInfoArray[10000]; // Adjust the size as needed

typedef enum  TglangLanguage (*DetectLanguageFunction)(const char *text);

// Function to recursively process files in a directory and store information in an array
void processDirectory(void * libraryHandle, const char *path, const char *basepath, FileInfo *fileInfoArray, int *fileCount, char **excludedExtensions, int excludedExtensionsCount, char **excludedDirectories, int excludedDirectoriesCount) {
	struct dirent *entry;
	DIR *dir = opendir(path);

	if (dir == NULL) {
		perror("Error opening directory");
		return;
	}

	printf("processDirectory - %s\n", path);

	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_type == DT_DIR) {
			// Check if the directory is excluded
			int isExcluded = 0;
			for (int i = 0; i < excludedDirectoriesCount; i++) {
				if (strcmp(entry->d_name, excludedDirectories[i]) == 0) {
					isExcluded = 1;
					break;
				}
			}
			if (isExcluded) {
				continue;
			}

			// Skip "." and ".." directories
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
				continue;
			}

			char fullpath[1024]; // Adjust this size as needed
			snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

			// Recursively process subdirectories
			processDirectory(libraryHandle, fullpath, basepath, fileInfoArray, fileCount, excludedExtensions, excludedExtensionsCount, excludedDirectories, excludedDirectoriesCount);
		} else if (entry->d_type == DT_REG) {
			// Check if the file extension is excluded
			char *fileExtension = strrchr(entry->d_name, '.');
			if (fileExtension != NULL) {
				int isExcluded = 0;
				for (int i = 0; i < excludedExtensionsCount; i++) {
					// printf("Usage: %s %s %d\n", fileExtension, excludedExtensions[i], strcmp(fileExtension, excludedExtensions[i]));
					if (strcmp(fileExtension, excludedExtensions[i]) == 0) {
						isExcluded = 1;
						break;
					}
				}
				if (isExcluded) {
					continue;
				}
			}

			// Process regular files
			char fullpath[1024]; // Adjust this size as needed
			snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

			FILE *file = fopen(fullpath, "r");
			if (file) {
				// Read the content of the file
				fseek(file, 0, SEEK_END);
				long file_size = ftell(file);
				fseek(file, 0, SEEK_SET);
				printf("%s\n", fullpath);
				char *file_content = (char *)malloc(file_size + 1);
				if (file_content) {
					fread(file_content, 1, file_size, file);
					file_content[file_size] = '\0';

					// Detect programming language and store results
					// int result = tglang_detect_programming_language(file_content);


					std::chrono::steady_clock::time_point start_time, end_time;
					std::chrono::duration<double, std::milli> elapsed_time;
					start_time = std::chrono::steady_clock::now();

					// Get a pointer to the function
					DetectLanguageFunction detectLanguage = (DetectLanguageFunction)dlsym(libraryHandle, "tglang_detect_programming_language");
					if (!detectLanguage) {
						fprintf(stderr, "Failed to retrieve the function: %s\n", dlerror());
						dlclose(libraryHandle);
						return;
					}
					// Call the function
					int result = detectLanguage(file_content);

					end_time = std::chrono::steady_clock::now();
					elapsed_time = end_time - start_time;
					printf("Full exec speed: %.4f ms\n", elapsed_time.count());





					FileInfo fileInfo;
					strncpy(fileInfo.path, fullpath + strlen(basepath), sizeof(fileInfo.path));
					fileInfo.language = result;
					fileInfoArray[*fileCount] = fileInfo;
					(*fileCount)++;

					free(file_content);

					// Log the filename and detected language
					printf("%s - %d, %s\n", fileInfo.path, result,TglangLanguageNames[result]);
				}
				fclose(file);
			}
		}
	}
	closedir(dir);
}

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage: %s <folder_path_or_file> [excluded_extensions] [excluded_directories]\n", argv[0]);
		return 1;
	}

	const char *path = argv[1];

	// Check if the provided path is a directory or a file
	struct stat path_stat;
	if (stat(path, &path_stat) == -1) {
		perror("Error checking path");
		return 1;
	}

	int fileCount = 0;
	if (S_ISDIR(path_stat.st_mode)) {
		const char *basepath = path;

		// Initialize arrays to store excluded extensions and directories
		char *excludedExtensions[10]; // Adjust the size as needed
		int excludedExtensionsCount = 0;

		char *excludedDirectories[10]; // Adjust the size as needed
		int excludedDirectoriesCount = 0;

		// Parse additional command-line arguments for excluded extensions and directories
		for (int i = 2; i < argc; i++) {
			if (strcmp(argv[i], "-ext") == 0 && i + 1 < argc) {
				// Argument specifies excluded extensions
				excludedExtensions[excludedExtensionsCount] = argv[i + 1];
				excludedExtensionsCount++;
				i++;
			} else if (strcmp(argv[i], "-dir") == 0 && i + 1 < argc) {
				// Argument specifies excluded directories
				excludedDirectories[excludedDirectoriesCount] = argv[i + 1];
				excludedDirectoriesCount++;
				i++;
			}
		}


		void *libraryHandle = dlopen(LIB_PATH, RTLD_LAZY); //"submission/libtglang.dylib"
		if (!libraryHandle) {
			fprintf(stderr, "Failed to load the library: %s\n", dlerror());
			return 1;
		}

		processDirectory(libraryHandle, path, basepath, fileInfoArray, &fileCount, excludedExtensions, excludedExtensionsCount, excludedDirectories, excludedDirectoriesCount);
		dlclose(libraryHandle);

		// Calculate the percentage of each language
		int languageCounts[100] = {0}; // Assuming a maximum of 100 different languages
		for (int i = 0; i < fileCount; i++) {
			languageCounts[fileInfoArray[i].language]++;
		}

		// Create an array of LanguageInfo structs to store language names, counts, and percentages
		LanguageInfo languageInfoArray[100];
		int languageInfoCount = 0;

		for (int i = 0; i < 100; i++) {
			if (languageCounts[i] > 0) {
				float percentage = (float)languageCounts[i] / fileCount * 100.0;
				languageInfoArray[languageInfoCount].name = TglangLanguageNames[i];
				languageInfoArray[languageInfoCount].count = languageCounts[i];
				languageInfoArray[languageInfoCount].percentage = percentage;
				languageInfoCount++;
			}
		}

		// Sort the languageInfoArray by percentage
		qsort(languageInfoArray, languageInfoCount, sizeof(LanguageInfo), compareLanguageInfo);

		// List detected languages and their percentages (sorted)
		printf("\nList of detected languages (sorted by percentage):\n");
		for (int i = 0; i < languageInfoCount; i++) {
			printf("Language: %s, Count: %d, Percentage: %.2f%%\n", languageInfoArray[i].name, languageInfoArray[i].count, languageInfoArray[i].percentage);
		}
	} else if (S_ISREG(path_stat.st_mode)) {
		// It's a regular file, process it directly

		// Open and process the file here
		FILE *file = fopen(path, "r");
		if (file) {
			// Read the content of the file
			fseek(file, 0, SEEK_END);
			long file_size = ftell(file);
			fseek(file, 0, SEEK_SET);
			printf("%s\n", path);
			char *file_content = (char *)malloc(file_size + 1);
			if (file_content) {
				fread(file_content, 1, file_size, file);
				file_content[file_size] = '\0';


				// // Detect programming language and store results
				// int result = tglang_detect_programming_language(file_content);


				std::chrono::steady_clock::time_point start_time, end_time;
				std::chrono::duration<double, std::milli> elapsed_time;
				start_time = std::chrono::steady_clock::now();


				void *libraryHandle = dlopen(LIB_PATH, RTLD_LAZY); //"submission/libtglang.dylib"
				if (!libraryHandle) {
					fprintf(stderr, "Failed to load the library: %s\n", dlerror());
					return 1;
				}

				// Get a pointer to the function
				DetectLanguageFunction detectLanguage = (DetectLanguageFunction)dlsym(libraryHandle, "tglang_detect_programming_language");
				if (!detectLanguage) {
					fprintf(stderr, "Failed to retrieve the function: %s\n", dlerror());
					dlclose(libraryHandle);
					return 1;
				}

				// Call the function
				int result = detectLanguage(file_content);
				dlclose(libraryHandle);



				end_time = std::chrono::steady_clock::now();
				elapsed_time = end_time - start_time;
				printf("Full exec speed: %.4f ms\n", elapsed_time.count());




				FileInfo fileInfo;
				strncpy(fileInfo.path, path, sizeof(fileInfo.path));
				fileInfo.language = result;
				fileInfoArray[0] = fileInfo; // Assuming only one file when processing a single file
				fileCount = 1;

				free(file_content);

				// Log the filename and detected language
				printf("%s - %s\n", fileInfo.path, TglangLanguageNames[result]);
			}
			fclose(file);
		}
	} else {
		printf("Invalid path: %s\n", path);
		return 1;
	}

	return 0;
}

