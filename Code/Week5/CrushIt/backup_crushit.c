#include "crushit.h"
#include "mydefs.h"

#define MAXROWS 20
#define WIDTH 5
#define HEIGHT 6
#define MAX_BUFFER ((MAXROWS + 2) * WIDTH + 1) 

bool initialise(state* s, const char* str) {
    if (s == NULL || str == NULL) {
        return false;
    }

    char buffer[MAX_BUFFER];  // 存储文件或字符串内容的缓冲区
    int max_buffer = MAX_BUFFER;

    // 尝试从文件读取
    if (read_file(str, buffer, max_buffer)) {
        // 文件读取成功，继续处理 buffer
        printf("Content from file: %s\n", buffer);
    } else {
        // 如果无法读取文件，假定传入的是字符串
        int len = strlen(str);
        if ((len % WIDTH != 0) || (len > MAXROWS * WIDTH)) {
            return false;  // 检查字符串的格式是否正确
        }
        strncpy(buffer, str, max_buffer - 1);  // 将字符串拷贝到 buffer 中
        buffer[max_buffer - 1] = '\0';  // 确保 buffer 以 '\0' 结尾
    }

    // 调用填充棋盘的函数（假设 fill_board 是你实现的函数）
    bool result = fill_board(s, buffer);
    return result;
}


bool read_file(const char* filename, char* buffer, int max_buffer) {
    if (filename == NULL || buffer == NULL) {
        return false;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return false;
    }

    int buffer_index = 0;  // 用来跟踪 buffer 中的位置
    char line[WIDTH + 2];  // 每行的存储空间，最多 WIDTH 个字符加换行符和 '\0'

    // 逐行读取文件并处理
    while (fgets(line, sizeof(line), file) != NULL) {
        // 去除末尾的换行符（'\n' 或 '\r'）
        int line_len = strlen(line);
        if (line[line_len - 1] == '\n' || line[line_len - 1] == '\r') {
            line[line_len - 1] = '\0';  // 用 '\0' 替换掉换行符
            line_len--;
        }

        // 检查行长度是否符合要求
        if (line_len != WIDTH) {
            fclose(file);
            return false; // 如果行长度不等于 WIDTH，返回 false
        }

        // 将每行的前 WIDTH 个字符存入 buffer 中
        for (int i = 0; i < WIDTH && line[i] != '\0'; i++) {
            if (buffer_index < max_buffer - 1) {  // 确保 buffer 没有溢出
                buffer[buffer_index++] = line[i];
            }
        }
    }

    buffer[buffer_index] = '\0';  // 为 buffer 添加字符串结束符

    fclose(file);
    return true;
}

// 处理字符串
bool process_string(const char* input, char* output) {
    int index_in = 0;
    int index_out = 0;
    while (input[index_in] != '\0') {
        char c = input[index_in];
        if (isalpha(c) || c == '.') {
            output[index_out++] = c;
        } else if (c == '\n' || c == '\r') {
            // 跳过换行符
        } else {
            // 非法字符，返回 false
            return false;
        }
        index_in++;
    }
    output[index_out] = '\0'; // 添加字符串结束符
    printf("input %s and output %s\n", input, output);
    return true;
}

// 填充棋盘
bool fill_board(state* s, const char* str) {
    if (s == NULL || str == NULL) {
        return false;
    }

    int len = strlen(str);
    if (len % 5 != 0) {
        return false; // 字符串长度不符合要求
    }
    int index = 0;
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            if (!isalpha(str[index]) && str[index] != '.') {
            return false;

            }
            s->board[j][i] = str[index];
            printf("board %c and in str %c\n", s->board[j][i], str[index]);
            index++;
        }
    }
    return true;
}

bool tostring(const state* s, char* str) {
    if (s == NULL || str == NULL) {
        return false;
    }

    int index = 0;
    int start_row = 0;

    // Find the first row that contains at least one non-empty block
    while (start_row < HEIGHT) {
        int found_non_empty = 0;
        for (int i = 0; i < WIDTH; i++) {
            if (s->board[start_row][i] != '.') {
                found_non_empty = 1;
                break;
            }
        }
        if (found_non_empty) {
            break;
        }
        start_row++;
    }

    // If the entire board is empty, return an empty string
    if (start_row == HEIGHT) {
        str[0] = '\0';
        return true;
    }

    // Copy the board from the first non-empty row
    for (int j = start_row; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            // Add '.' for empty cells, otherwise copy the character
            if (s->board[j][i] == '.') {
                str[index++] = '.';
            } else {
                str[index++] = s->board[j][i];
            }
        }
    }

    str[index] = '\0'; // Null-terminate the string
    return true;
}


bool check_horizontal_matches(state* s, state* next, bool* match) {
    int cnt;
    for (int j = 0; j < HEIGHT; j++) {
        cnt = 1; // 重置计数器
        for (int i = 1; i < WIDTH; i++) {
            if (s->board[j][i] == s->board[j][i - 1] && s->board[j][i] != '.') {
                cnt++;
            } else {
                if (cnt >= 3) {
                    // 替换匹配的字符为 '.'
                    for (int k = i - cnt; k < i; k++) {
                        next->board[j][k] = '.';
                    }
                    *match = true;
                }
                cnt = 1;  // 重置计数器
            }
        }
        // 检查行尾的匹配
        if (cnt >= 3) {
            for (int k = WIDTH - cnt; k < WIDTH; k++) {
                next->board[j][k] = '.';
            }
            *match = true;
        }
    }
    return *match;
}

bool check_vertical_matches(state* s, state* next, bool* match) {
    int cnt;
    for (int i = 0; i < WIDTH; i++) {
        cnt = 1; // 重置计数器
        for (int j = 1; j < HEIGHT; j++) {
            if (s->board[j][i] == s->board[j - 1][i] && s->board[j][i] != '.') {
                cnt++;
            } else {
                if (cnt >= 3) {
                    for (int k = j - cnt; k < j; k++) {
                        next->board[k][i] = '.';
                    }
                    *match = true;
                }
                cnt = 1;  // 重置计数器
            }
        }
        // 检查列尾的匹配
        if (cnt >= 3) {
            for (int k = HEIGHT - cnt; k < HEIGHT; k++) {
                next->board[k][i] = '.';
            }
            *match = true;
        }
    }
    return *match;
}

bool matches(state* s) {
    if (s == NULL) {
        return false;
    }
    state next;

    // 复制当前棋盘到 next.board
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            next.board[j][i] = s->board[j][i];
        }
    }

    bool match = false;

    // 检查横向匹配
    check_horizontal_matches(s, &next, &match);

    // 检查纵向匹配
    check_vertical_matches(s, &next, &match);

    // 将 next 的棋盘复制回 s
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            s->board[j][i] = next.board[j][i];
        }
    }

    return match;
}


bool dropblocks(state* s) {
    if (s == NULL){
        return false;
    }
    char* source_block = NULL;
    char* destination_block = NULL; 

    for (int i = 0; i < WIDTH; i++) {
        int empty_row = HEIGHT - 1; // Start from the bottom of the column
        for (int j = HEIGHT - 1; j >= 0; j--) {
            if (s->board[j][i] != '.') {
                // Move block down if necessary
                if (j != empty_row) {
                    source_block = &(s->board[j][i]);
                    destination_block = &(s->board[empty_row][i]);
                    *destination_block = *source_block;
                    *source_block = '.';
                }
                empty_row--;
            }
        }
        // Fill remaining positions with '.'
        for (int j = empty_row; j >= 0; j--) {
            s->board[j][i] = '.';
        }
    }
    return true;
}


void print_board(const state* s) {
    printf("Current board state:\n");
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            printf("%c", s->board[j][i]);
        }
        printf("\n");
    }
    printf("\n");
}

void test(void) {
}











// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>
// #include <stdbool.h>
// #include <assert.h>

// #define MAXROWS 20
// #define WIDTH 5
// #define HEIGHT 6
// #define MAX_BUFFER ((MAXROWS + 2) * WIDTH + 1) 

// typedef struct {
//     char board[HEIGHT][WIDTH]; // 定义棋盘
// } state;

// // 从文件中读取内容
// bool read_file(const char* filename, char* buffer, int max_buffer) {
//     if (filename == NULL || buffer == NULL) {
//         return false;
//     }

//     FILE* file = fopen(filename, "r");
//     if (file == NULL) {
//         return false;
//     }

//     int buffer_index = 0;  // 用来跟踪 buffer 中的位置
//     char line[MAX_BUFFER]; // 存储每一行的临时缓冲区

//     while (fgets(line, sizeof(line), file) != NULL) {
//         // 确保不会超出 buffer 的大小，并且每行只复制前 WIDTH 个字符
//         for (int i = 0; i < WIDTH && line[i] != '\0' && line[i] != '\n'; i++) {
//             if (buffer_index < max_buffer - 1) {  // 留下一个位置用于 null 终止符
//                 buffer[buffer_index++] = line[i]; // 复制字符到 buffer 中
//             }
//         }
//         // 每行复制完后，插入 '\n'（如果需要分行显示，非必须）
//         if (buffer_index < max_buffer - 1) {
//             buffer[buffer_index++] = '\n'; // 每行结束后加一个换行符，非必须
//         }
//     }

//     buffer[buffer_index] = '\0';  // 添加字符串的结束符

//     fclose(file);
//     return true;
// }
// // 处理字符串
// bool process_string(const char* input, char* output) {
//     int index_in = 0;
//     int index_out = 0;

//     while (input[index_in] != '\0') {
//         char c = input[index_in];
//         if (isalpha(c) || c == '.') {
//             output[index_out++] = c;
//         } else if (c == '\n' || c == '\r') {
//             // 跳过换行符
//         } else {
//             // 非法字符，返回 false
//             return false;
//         }
//         index_in++;
//     }
//     output[index_out] = '\0'; // 添加字符串结束符
//     printf("input %s and output %s\n", input, output);
//     return true;
// }

// // 填充棋盘
// bool fill_board(state* s, const char* str) {
//     if (s == NULL || str == NULL) {
//         return false;
//     }

//     int len = strlen(str);
//     if (len != WIDTH * HEIGHT) {
//         return false; // 字符串长度不符合要求
//     }

//     int index = 0;
//     for (int j = 0; j < HEIGHT; j++) {
//         for (int i = 0; i < WIDTH; i++) {
//             s->board[j][i] = str[index];
//             printf("board %c and in str %c\n", s->board[j][i], str[index]);
//             index++;
//         }
//     }
//     return true;
// }

// bool initialise(state* s, const char* str) { 
//     if (s == NULL || str == NULL) {
//         printf("Initialise function received NULL argument.\n");  // 添加此行
//         return false;
//     }

//     char buffer[MAX_BUFFER];
//     int max_buffer = MAX_BUFFER;
//     char processed_str[WIDTH * MAXROWS + 1]; // 存储处理后的字符串
    
//     bool process_success = false;
//     bool fill_success = false;

//     // 尝试从文件读取
//     if (read_file(str, buffer, max_buffer)) {
//         printf("File '%s' read successfully.\n", str);  // 添加此行
//         // 文件读取成功，处理字符串
//         process_success = process_string(buffer, processed_str);
//         if (!process_success) {
//             printf("Processing string from file failed.\n");  // 添加此行
//             return false;
//         } else {
//             printf("String after processing from file: %s\n", processed_str);  // 添加此行
//         }
//     } else {
//         printf("Failed to read file '%s'. Assuming input is board string.\n", str);  // 添加此行
//         // 假设 str 是直接的棋盘字符串
//         int len = strlen(str);
//         if ((len % WIDTH != 0) || (len > MAXROWS * WIDTH)) {
//             printf("Invalid string length for board: %d\n", len);  // 添加此行
//             return false;
//         }

//         process_success = process_string(str, processed_str);
//         if (!process_success) {
//             printf("Processing input string failed.\n");  // 添加此行
//             return false;
//         } else {
//             printf("String after processing input: %s\n", processed_str);  // 添加此行
//         }
//     }

//     // 填充棋盘
//     fill_success = fill_board(s, processed_str);
//     if (!fill_success) {
//         printf("Filling board failed.\n");  // 添加此行
//         return false;
//     } else {
//         printf("Board filled successfully.\n");  // 添加此行
//     }

//     // 显示最终的棋盘状态
//     printf("Final board state after initialisation:\n");
//     for (int j = 0; j < HEIGHT; j++) {
//         for (int i = 0; i < WIDTH; i++) {
//             printf("%c", s->board[j][i]);
//         }
//         printf("\n");
//     }

//     return true;
// }

// bool tostring(const state* s, char* str){
//     if (s == NULL || str == NULL) {
//         return false;
//     }

//     int index = 0;
//     int start_row = 0;

//     // 找到第一个包含非空洞块的行
//     while (start_row < HEIGHT) {
//         int i;
//         for (i = 0; i < WIDTH; i++) {
//             if (s->board[start_row][i] != '.') {
//                 // 在该行找到了非空洞块
//                 break;
//             }
//         }
//         if (i < WIDTH) {
//             // 找到包含非空洞块的行
//             break;
//         }
//         start_row++;
//     }

//     // 如果整个棋盘都是空洞
//     if (start_row == HEIGHT) {
//         str[0] = '\0';
//         return true;
//     }

//     // 从第一个非空行开始复制
//     for (int j = start_row; j < HEIGHT; j++) {
//         for (int i = 0; i < WIDTH; i++) {
//             str[index++] = s->board[j][i];
//         }
//     }
//     str[index] = '\0'; // 添加字符串结束符
//     return true;
// }


// bool check_horizontal_matches(state* s, state* next, bool* match) {
//     int cnt;
//     for (int j = 0; j < HEIGHT; j++) {
//         cnt = 1; // 重置计数器
//         for (int i = 1; i < WIDTH; i++) {
//             if (s->board[j][i] == s->board[j][i - 1] && s->board[j][i] != '.') {
//                 cnt++;
//             } else {
//                 if (cnt >= 3) {
//                     // 替换匹配的字符为 '.'
//                     for (int k = i - cnt; k < i; k++) {
//                         next->board[j][k] = '.';
//                     }
//                     *match = true;
//                 }
//                 cnt = 1;  // 重置计数器
//             }
//         }
//         // 检查行尾的匹配
//         if (cnt >= 3) {
//             for (int k = WIDTH - cnt; k < WIDTH; k++) {
//                 next->board[j][k] = '.';
//             }
//             *match = true;
//         }
//     }
//     return *match;
// }

// bool check_vertical_matches(state* s, state* next, bool* match) {
//     int cnt;
//     for (int i = 0; i < WIDTH; i++) {
//         cnt = 1; // 重置计数器
//         for (int j = 1; j < HEIGHT; j++) {
//             if (s->board[j][i] == s->board[j - 1][i] && s->board[j][i] != '.') {
//                 cnt++;
//             } else {
//                 if (cnt >= 3) {
//                     for (int k = j - cnt; k < j; k++) {
//                         next->board[k][i] = '.';
//                     }
//                     *match = true;
//                 }
//                 cnt = 1;  // 重置计数器
//             }
//         }
//         // 检查列尾的匹配
//         if (cnt >= 3) {
//             for (int k = HEIGHT - cnt; k < HEIGHT; k++) {
//                 next->board[k][i] = '.';
//             }
//             *match = true;
//         }
//     }
//     return *match;
// }

// bool matches(state* s) {
//     if (s == NULL) {
//         return false;
//     }
//     state next;

//     // 复制当前棋盘到 next.board
//     for (int j = 0; j < HEIGHT; j++) {
//         for (int i = 0; i < WIDTH; i++) {
//             next.board[j][i] = s->board[j][i];
//         }
//     }

//     bool match = false;

//     // 检查横向匹配
//     check_horizontal_matches(s, &next, &match);

//     // 检查纵向匹配
//     check_vertical_matches(s, &next, &match);

//     // 将 next 的棋盘复制回 s
//     for (int j = 0; j < HEIGHT; j++) {
//         for (int i = 0; i < WIDTH; i++) {
//             s->board[j][i] = next.board[j][i];
//         }
//     }

//     return match;
// }


// bool dropblocks(state* s) {
//     if (s == NULL){
//         return false;
//     }
//     char* source_block = NULL;
//     char* destination_block = NULL; 

//     for (int i = 0; i < WIDTH; i++) {
//         int empty_row = HEIGHT - 1; // Start from the bottom of the column
//         for (int j = HEIGHT - 1; j >= 0; j--) {
//             if (s->board[j][i] != '.') {
//                 // Move block down if necessary
//                 if (j != empty_row) {
//                     source_block = &(s->board[j][i]);
//                     destination_block = &(s->board[empty_row][i]);
//                     *destination_block = *source_block;
//                     *source_block = '.';
//                 }
//                 empty_row--;
//             }
//         }
//         // Fill remaining positions with '.'
//         for (int j = empty_row; j >= 0; j--) {
//             s->board[j][i] = '.';
//         }
//     }
//     return true;
// }


// void print_board(const state* s) {
//     printf("Current board state:\n");
//     for (int j = 0; j < HEIGHT; j++) {
//         for (int i = 0; i < WIDTH; i++) {
//             printf("%c", s->board[j][i]);
//         }
//         printf("\n");
//     }
//     printf("\n");
// }

// void test(void) {
//     // state s;
//     // char str[MAX_BUFFER];
// }


// in maydefs.h
// #pragma once

// /* Put anything you like here, but
//    certainly your 'state' structure 
//    to hold your board e.g.*/
// #define MAXROWS 20
// #define WIDTH 5
// #define HEIGHT 6
// #define MAX_BUFFER ((MAXROWS + 2) * WIDTH + 1) 

// struct st {
//    char board[MAXROWS][WIDTH];
// };
// typedef struct st state;

// // bool read_file(const char* filename, char* buffer, int max_buffer);
// // bool process_string(const char* input, char* output) ;
// // bool fill_board(state* s, const char* str) ;
// // bool check_horizontal_matches(state* s, state* next, bool* match) ;
// // bool check_vertical_matches(state* s, state* next, bool* match) ;
// // void print_board(const state* s) ;
