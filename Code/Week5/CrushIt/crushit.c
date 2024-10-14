#include "crushit.h"
#include "mydefs.h"
#include "crushit.h"
#include "mydefs.h"
#include <stdio.h>

// struct st {
//    char board[MAXROWS][WIDTH];
//    char* varlen_tiles;
//    struct st* next;
//    };
// typedef struct st state;

//s->board s->varlen_tiles 
//next->board[]

/* const char* str  地址str存的东西不会改变，as接受*/
bool initialise(state* s, const char* str){   
    if ((s == NULL) || (str == NULL)) {
        return false;
    }
    int len = strlen(str);
    if (len != HEIGHT * WIDTH) {
        return false;   
    } else {
        printf("add %p\n", str);
        char* pstr = str + len - 5; // 指向字符串最后一行的开头
        for (int j = 0; j < HEIGHT; j++) {
            for (int i =0; i< WIDTH; i++){
                s->board[j][i] = *pstr;
                pstr++;
                printf("board %c and in str %c\n", s->board[j][i], *pstr);                      
                printf("adr of current pstr %p\n", pstr);
            }
            //pstr增到29，要回到（24）
            pstr -= WIDTH;
        } //加一行
        s->varlen_tiles = NULL;
        s->next = NULL;
        return true;
    }
}

bool tostring(const state* s, char* str){
    if ((s == NULL) || (str == NULL)) {
        return false;
    }
    int index = 0;
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            str[index] = s->board[j][i];
            index++;
        }
    }
    printf("c\n",board[j][i]);
    return true;
}

/* 您的问题是关于以下代码片段的执行顺序和逻辑：

const char* pstr = str; // 指向字符串开头
for (int j = 0; j < HEIGHT; j++) {
    for (int i = 0; i < WIDTH; i++) {
        s->board[j][i] = *pstr;
        pstr++;
    }
}
您问道：
这就能确保 s->board[j][i] = *pstr; 和 pstr++; 变化同步了吗？为什么不需要再加一行，确保它等于改变后的值。


这个问题涉及指针的递增和赋值操作的顺序。

1. 原代码的执行过程

s->board[j][i] = *pstr; // 第一步：将当前指针指向的字符赋值给 board
pstr++;                 // 第二步：将指针 pstr 向后移动一位，指向下一个字符
这个过程是：

首先，*pstr 取得指针 pstr 当前指向的字符，并将其赋值给 s->board[j][i]。
然后，pstr++ 将指针 pstr 向后移动一位，指向下一个字符。
这样，在下一次循环中，*pstr 将取得下一个字符，赋值给 s->board[j][i] 中的下一个位置。

2. 您建议的代码

s->board[j][i] = *pstr; 
pstr++;
s->board[j][i] = *pstr;

执行过程：

第一步：s->board[j][i] = *pstr; 将当前字符赋值给 s->board[j][i]。
第二步：pstr++; 指针前进，指向下一个字符。
第三步：s->board[j][i] = *pstr; 再次对 s->board[j][i] 赋值，这次赋值的是下一个字符。
问题在于：

您在同一个位置 s->board[j][i] 上进行了两次赋值，第一次赋值了当前字符，第二次又覆盖为下一个字符。
最终，s->board[j][i] 中存放的是 pstr 指针移动后指向的字符，即下一个字符。
3. 为什么原代码是正确的

原代码的意图是将字符串 str 中的字符按顺序填充到 s->board 中的每个位置。

操作顺序：

赋值：将当前字符 *pstr 赋值给 s->board[j][i]。
指针前移：pstr++，指向下一个字符。
循环机制：内层循环遍历 i（宽度），外层循环遍历 j（高度）。每次循环都会对 s->board[j][i] 进行一次赋值。

保证同步：由于每次循环都执行一次 s->board[j][i] = *pstr; 和一次 pstr++;，因此在下一次循环时，pstr 已经指向下一个字符，s->board[j][i] 也更新到下一个位置。 */




bool matches(state* s) {
    if (s == NULL) {
        return false;
    }

    bool match = false;

    // 创建一个新的状态，用于保存修改后的棋盘
    //当前的Copy为下一个板子next->board[]的步骤是不是必须的？ 如果不复制，不创造一个新的next->board 后面的drop box还能不能调用原来的板子。
    //这个copy操作是不是必须要与malloc函数一起进行？内存用Malloc还是calloc?

    state* next = (state*)malloc(sizeof(state));
    if (next == NULL) {
        return false;
    }

    // 复制当前状态到 next
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            next->board[j][i] = s->board[j][i];
        }
    }
    next->varlen_tiles = "......"; // 初始化 varlen_tiles
    next->next = NULL;
    char* temp_start= NULL;
    int cnt = 0;



// 您希望在 matches 函数的横向匹配中：

// 使用一个指针 temp_start 来存储开始匹配的位置。
// 扫描一行，确认是否需要匹配。
// 如果确认匹配，返回原始地址，根据 cnt 的数量，一个一个替换为 '.'
bool match = false;

    // 创建一个新的状态，用于保存修改后的棋盘
    state* next = (state*)malloc(sizeof(state));
    if (next == NULL) {
        return false;
    }

    // 复制当前状态到 next
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            next->board[j][i] = s->board[j][i];
        }
    }
    next->next = NULL;
    char* start = NULL;
    char* last = NULL;
    int cnt = 1;  // 初始计数为1

    for (int j = 0; j < HEIGHT; j++) {
        cnt = 1; // 重置计数器
        start = NULL;
        last = NULL;
        for (int i = WIDTH -1 ; i >= 1; i--) {
            if (s->board[j][i] == s->board[j][i-1] && (s->board[j][i] != '.')) {
                if (start == NULL) {
                    start = &(next->board[j][i-1]); // 记录开始位置
                }
                cnt++;
                if (cnt >= 3) {
                    // 替换匹配的字符为 '.'
                    for (int change = 0; change < cnt; change++) {
                        *(start + change) = '.';
                    }
                    match = true;
                    cnt = 1; // 重置计数器
                    start = NULL;
                }
            } else {
                cnt = 1;
                start = NULL;
            }
        }
    }
    // 将 next 的棋盘复制回 s
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            s->board[j][i] = next->board[j][i];
        }
    }
    free(next);
    return match;
}
