#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct Record {
    char *name;
    char *phoneNumber;
    int id;
    struct Record *next;
} person;

typedef struct {
    int size;
    person *people;
} Book;

FILE *file;
char *fileName;
int freeId = -1;
Book phoneBook;
person current;
person *tail, *head, *cur;

char *low(char *a) {
    int i = 0;
    char *ans = (char *) malloc(strlen(a) * sizeof(char));
    while (a[i] != '\0') {
        ans[i] = tolower(a[i++]);
    }
    ans[i] = '\0';
    return ans;
}

char *read(FILE *inStream) {
    if (inStream != stdin) {
        fgetc(inStream);
    }
    char *str = (char *) malloc(1000 * sizeof(char));
    int i = 0, j = 0;
    char c;
    while (true) {
        c = fgetc(inStream);
        if (c == '\n' || c == ' ' && i > 0 || c == EOF) {
            break;
        }
        if (!(i % 1000)) {
            j++;
            str = realloc(str, (j * 1000) * sizeof(char));
        }
        str[i++] = c;
    }
    str[i] = '\0';
    if (inStream != stdin) {
        ungetc(c, inStream);
    }
    return str;
}

person *getId(int id) {
    int i;
    person *mem;
    mem = head;
    cur = head;
    for (i = 0; i < phoneBook.size; i++) {
        if (cur->id == id) {
            return mem;
        }
        mem = cur;
        cur = cur->next;
    }
    return NULL;
}

bool stabilizate(char *a) {
    if (strcmp(a, "") == 0) {
        return false;
    }
    int k, p = 0;
    for (k = 0; k < strlen(a); k++) {
        if (('0' <= a[k]) && (a[k] <= '9')) {
            if (p != k) {
                a[p] = a[k];
            }
            p++;
        }
    }
    a[p] = '\0';
    return true;
}

bool Correct(char *a) {
    if (strcmp(a, "") == 0) {
        return false;
    }
    int i = 0;
    while (a[i] != '\0') {
        if (!isalpha(a[i++])) {
            return false;
        }
    }
    return true;
}

void create(int id, char *name, char *number) {
    person *buf = (person *) malloc(sizeof(person));
    buf->id = id;
    buf->name = name;
    buf->phoneNumber = number;
    buf->next = NULL;
    if (buf->name[0] == '\0') {
        return;
    }
    if (head == NULL) {
        head = buf;
        tail = head;
    } else {
        tail->next = buf;
        tail = buf;
    }
    phoneBook.size++;
    return;
}

void deletting(person *cur) {
    if (cur == head) {
        head = cur->next;
        free(cur);
    } else {
        cur = cur->next;
        cur->next = cur->next;
        free(cur);
    }
    return;
}

void writing() {
    fclose(file);
    file = fopen(fileName, "w+");
    int i;
    for (cur = head; cur != NULL; cur = cur->next) {
        fprintf(file, "%d %s %s\n", cur->id, cur->name, cur->phoneNumber);
    }
    return;
}

void change(char *command) {
    if (!strcmp(command, "name")) {
        current.name = read(stdin);
        if (Correct(current.name)) {
            strcpy(cur->name, current.name);
        }
    } else if (!strcmp(command, "number")) {
        current.phoneNumber = read(stdin);
        if (stabilizate(current.phoneNumber)) {
            strcpy(cur->phoneNumber, current.phoneNumber);
        }
    }
}

void finding(char *command) {
    int i;
    if (strlen(command) > 0) {
        bool itName, itNumber, isFound;
        isFound = false;
        itName = Correct(command);
        if (!itName) {
            stabilizate(command);
        } else {
            command = low(command);
        }
        cur = head;
        for (i = 0; i < phoneBook.size; i++) {
            itNumber = 0;
            if (itName) {
                if (strstr(low(cur->name), command)) {
                    itNumber = 1;
                }
            } else {
                if (strcmp(cur->phoneNumber, command) == 0) {
                    itNumber = 1;
                }
            }
            if (!isFound && itNumber) {
                isFound = 1;
            }
            if (itNumber) {
                printf("%d ", cur->id);
                printf("%s %s\n", cur->name, cur->phoneNumber);
            }
            cur = cur->next;
        }
    }
}

int main(int argc, char *argv[]) {
    int i;
    person *modified;
    char *command;
    fileName = argv[1];
    file = fopen(fileName, "at+");
    if (file == NULL) {
        return 0;
    }
    phoneBook.size = 0;
    head = phoneBook.people;
    rewind(file);
    while (fscanf(file, "%d", &current.id) != EOF) {
        current.name = read(file);
        current.phoneNumber = read(file);
        create(current.id, current.name, current.phoneNumber);
    }

    freeId = (current.id > freeId) ? current.id : freeId;
    freeId++;
    while (true) {
        command = read(stdin);
        if (!strcmp(command, "create")) {
            current.name = read(stdin);
            current.phoneNumber = read(stdin);
            if (Correct(current.name) && stabilizate(current.phoneNumber)) {
                create(freeId, current.name, current.phoneNumber);
                freeId++;
                writing();
            }
        } else if (!strcmp(command, "delete")) {
            scanf("%d", &current.id);
            modified = getId(current.id);
            if (modified != NULL) {
                deletting(modified);
                phoneBook.size--;
                writing();
            }
        } else if (!strcmp(command, "change")) {
            scanf("%d ", &current.id);
            modified = getId(current.id);
            if (modified != NULL) {
                cur = modified->next;
                command = read(stdin);
                change(command);
                writing();
            }
        } else if (!strcmp(command, "find")) {
            command = read(stdin);
            finding(command);
        } else if (!strcmp(command, "exit")) {
            free(command);
            fclose(file);
            free(phoneBook.people);
            break;
        }
        fflush(file);
        fflush(stdout);
    }
    return 0;
}
