#include <stdio.h> // ввод вывод информации
#include <unistd.h>
#include <fcntl.h> // работа с файлами
#include <stdbool.h>
#include <string.h>

typedef struct {
    bool number_empty;
    bool number_all;
    bool show_endl;
    bool squeeze;
    bool show_tabs;
    bool error;
} CatInfo;


bool cat_parse_arg(CatInfo *info, char *argv, char *name) {
    ++argv;
    if (*argv == '-') {
        ++argv;
        if (strcmp(argv, "number-nonblank") == 0) {
            info->number_empty = true;
        } else if (strcmp(argv, "number") == 0) {
            info->number_all = true;
        } else if (strcmp(argv, "squeeze-blank") == 0) {
            info->squeeze = true;
        } else {
            dprintf(STDERR_FILENO, "%s: invalid option '-- %s'", name, argv);
            info->error = false;
            return false;
        }
        return true;
    }

    for (char *it = argv; *it; it++) {
        switch (*it) {
            case 'b':
            info->number_empty = true;
            break; case 'e':
            info->show_endl = true;
            break; case 'n':
            info->number_all = true;
            break; case 's':
            info->squeeze = true;
            break; case 't':
            info->show_tabs = true;
            break; default:
            dprintf(STDERR_FILENO, "%s: %s '%s'", name, "invalid option --", argv);
            info->error = false;
            return false;
        }
    }
    return true;
}

bool cat_no_args(int fd, char *name) {
    char buf[4096]; // количесвто байт на одной странице
    int bytes_read;
    
    if(fd == -1) {
        perror(name);
        return false;
    }
    bytes_read = read (fd, buf, 4096); // читаем количесвто байт
    while (bytes_read > 0) {
        printf("%.*s", bytes_read, buf);
        bytes_read = read (fd, buf, 4096);
    }
    return bytes_read != -1;
}

void cat_args_helper(CatInfo info, char *ptr, char *end, bool *empty) {
    
    if (end == ptr) {
        if(!info.squeeze) {
            printf("\n");
            return;
        }

        if(!*empty) {
            printf("\n");
        }
        *empty = true;
        return;
    } else {
        *empty = false;
    } if (!end) {
        printf("%s", ptr);
    } else {
        printf("%.*s", (int)(end - ptr + 1), ptr);
    }
}

bool cat_args_perform (CatInfo *info, char *file, char *name) {
    char buf[4096]; // количесвто байт на одной странице
    int fd;
    int bytes_read;
    char *end;
    char *ptr;
    bool empty = false;

    fd = open(file, O_RDONLY);
    if (fd == -1) {
        perror(name);
        return false;
    }
    bytes_read = read (fd, buf, 4095); // читаем количесвто на один меньше, для функции strchr
    while (bytes_read > 0) {
        buf[bytes_read] = 0; //азноляем самостоятельно
        ptr = buf; //"\n"
        while(1) { // поправить условие цикла
            end = strchr(ptr, '\n'); // ищет след символ и возвращает указатель на этот символ
            cat_args_helper(*info, ptr, end, &empty);
            if (end == 0) {
                break;
            }
            ptr = end + 1;
        }
        bytes_read = read (fd, buf, 4095);
    }
    return bytes_read != -1;

}

bool cat_args(int argc, char *argv[]) {
    CatInfo info = {false, false, false, false, false, true};
    for (int i = 1; i != argc; i++) {
        if (*argv[i] == '-') {
            if (!cat_parse_arg(&info, argv[i], argv[0])) {
                return false;
            }
        }
    }
    if (!(info.number_empty || info.number_all || info.show_endl || info.squeeze || info.show_tabs)) {
        for (int i = 1; i != argc; i++) {
            if (*argv[i] != '-') {
                if(!cat_no_args(open(argv[i], O_RDONLY), argv[0])){
                    info.error = false;
                }
            }
        }
    } else {
        for (int i = 1; i != argc; i++) {
            if (*argv[i] != '-') {
                if(!cat_args_perform(&info, argv[i], argv[0])) {
                    info.error = false;
                }
            }
        }
    }
    return info.error;
}

int main(int argc, char *argv[]) { // кол во аргумеентов и множество аргументов
    //(void)argv;
    if(argc == 1) {
        if(!cat_no_args(STDIN_FILENO, argv[0])) { // отправка в консоль ./s21_cat  явл одним аргументом
            return 1;
        }
    } else {
        if (cat_args(argc, argv)){
            return 1; // EXIT_FILEURE
        }
    }
    return 0;
}
