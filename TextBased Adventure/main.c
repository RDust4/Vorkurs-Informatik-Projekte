#include <stdio.h>
#include <string.h>
#include <Windows.h>


char* currentDialog;

#define MAX_DIALOGS 10
char* nextDialogs[MAX_DIALOGS];
int dialogCount;


FILE* openDialog(char* dialogPath) {
    char fullPath[256] = ".\\story\\";
    strcat(fullPath, dialogPath);

    FILE* file = fopen(fullPath, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", fullPath);
        return NULL;
    }
    return file;
}

void closeDialog(FILE* file) {
    fclose(file);
}


void printDialogDescription(FILE* dialog) {
    char line[256];
    while (fgets(line, sizeof(line), dialog) != NULL) {
        if (strstr(line, "---") != NULL) {
            break;
        }
        printf("%s", line);
    }
}

void parseDialogOptions(FILE* dialog) {
    // Read all lines into nextDialogs
    char line[256];
    while (fgets(line, sizeof(line), dialog) != NULL) {
        if (dialogCount >= MAX_DIALOGS) continue;

        // Remove trailing spaces/newlines
        char* end = line + strlen(line) - 1;
        while (end > line && isspace((unsigned char)*end)) {
            *end = '\0';
            end--;
        }

        // Store the line
        nextDialogs[dialogCount] = malloc(strlen(line) + 1);
        strcpy(nextDialogs[dialogCount], line);
        dialogCount++;
    }
}


void stepIntoDialog(int index) {
    if (index < 0 || index >= dialogCount) {
        printf("Error: Invalid dialog index\n");
        return;
    }

    free(currentDialog);
    currentDialog = nextDialogs[index];
    for (int i = 0; i < dialogCount; i++) {
        if (i != index) {
            free(nextDialogs[i]);
        }
        nextDialogs[i] = 0;
    }
    dialogCount = 0;
}


int main() {
    currentDialog = malloc(strlen("start.txt") + 1);
    strcpy(currentDialog, "start.txt");

    while (1) {
        FILE* dialog = openDialog(currentDialog);
        if (dialog == NULL) {
            return 1;
        }

        printDialogDescription(dialog);
        parseDialogOptions(dialog);
        closeDialog(dialog);

        if (dialogCount == 0) {
            break;
        }
        
        printf("\nEnter choice: ");
        int choice;
        scanf("%d", &choice);
        stepIntoDialog(choice - 1);

        printf("\n\n");
    }

    return 0;
}
