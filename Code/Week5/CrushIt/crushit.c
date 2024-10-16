#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

#define MAXROWS 20
#define WIDTH 5
#define HEIGHT 6
#define MAX_BUFFER ((MAXROWS + 2) * WIDTH + 1) 

// 从文件中读取内容
bool read_file(const char* filename, char* buffer, int max_buffer) {
    if (filename == NULL || buffer == NULL) {
        return false;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return false;
    }

    if (fgets(buffer, max_buffer, file) == NULL) {
        fclose(file);
        return false;
    }
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
    if (len != WIDTH * HEIGHT) {
        return false; // 字符串长度不符合要求
    }

    int index = 0;
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            s->board[j][i] = str[index];
            printf("board %c and in str %c\n", s->board[j][i], str[index]);
            index++;
        }
    }
    return true;
}

// 初始化棋盘
bool initialise(state* s, const char* str) {
    if (s == NULL || str == NULL) {
        return false;
    }

    char buffer[MAX_BUFFER];
    int max_buffer = MAX_BUFFER;
    char processed_str[WIDTH * MAXROWS + 1]; // 存储处理后的字符串

    // 尝试从文件读取
    if (read_file(str, buffer, max_buffer)) {
        // 文件读取成功，处理字符串
        if (!process_string(buffer, processed_str)) {
            return false;
        }
    } else {
        // 假设 str 是直接的棋盘字符串
        int len = strlen(str);
        if ((len % WIDTH != 0) || (len > MAXROWS * WIDTH)) {
            return false;
        }

        if (!process_string(str, processed_str)) {
            return false;
        }
    }

    printf("content of str: %s\n", processed_str); // 打印处理后的字符串
    // 填充棋盘
    bool result = fill_board(s, processed_str);
    return result;
}

bool tostring(const state* s, char* str){
    if (s == NULL || str == NULL) {
        return false;
    }

    int index = 0;
    int start_row = 0;

    // 找到第一个包含非空洞块的行
    while (start_row < HEIGHT) {
        int i;
        for (i = 0; i < WIDTH; i++) {
            if (s->board[start_row][i] != '.') {
                // 在该行找到了非空洞块
                break;
            }
        }
        if (i < WIDTH) {
            // 找到包含非空洞块的行
            break;
        }
        start_row++;
    }

    // 如果整个棋盘都是空洞
    if (start_row == HEIGHT) {
        str[0] = '\0';
        return true;
    }

    // 从第一个非空行开始复制
    for (int j = start_row; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            str[index++] = s->board[j][i];
        }
    }
    str[index] = '\0'; // 添加字符串结束符
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
    state s;
    char str[MAX_BUFFER];

    // 提供测试用的长字符串（30个字符）
    const char* test_str =
        "....."  // Row 0
        "....."  // Row 1
        "....."  // Row 2
        "....."  // Row 3
        "AAAAA"  // Row 4
        "AAAAA"; // Row 5

    // 初始化棋盘
    assert(initialise(&s, test_str) == true);
    printf("After initialisation:\n");
    print_board(&s);

    // 进行匹配
    assert(matches(&s) == true);
    printf("After matches:\n");
    print_board(&s);

    // 下落方块
    assert(dropblocks(&s) == true);
    printf("After dropblocks:\n");
    print_board(&s);

    // 将棋盘转换为字符串
    assert(tostring(&s, str) == true);
    printf("Board as string: %s\n", str);

    // 验证最终结果是否为空字符串（因为所有的 'A' 都被消除了）
    assert(strlen(str) == 0);
    assert(matches(&s)==false);
}
