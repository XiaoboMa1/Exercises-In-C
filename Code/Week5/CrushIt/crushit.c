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

1. 分析代码，提供不同种类的数据例子，解释每一行代码的作用

为了帮助您理解代码的运行过程，我们将对每一行代码进行详细解释，并使用具体的例子来演示三种可能的情况。

代码段：

c
复制代码
for (int j = HEIGHT - 1; j >= 0; j--) {
    if (s->board[j][i] == '.') {
        // 从当前空洞位置向上寻找非空字符
        int up = j - 1;
        while (up >= 0 && s->board[up][i] == '.') {
            up--;
        }
        if (up >= 0) {
            // 找到非空字符，进行下移
            possible_src = &(s->board[up][i]);
            dst = &(s->board[j][i]);
            *dst = *possible_src;
            *possible_src = '.'; // 原位置置为空洞
        } else {
            // 上方没有非空字符，当前位置置为空洞
            s->board[j][i] = '.';
        }
    }
}
改进的变量命名：

possible_src 改为 source_block，表示可能的源块（待下移的块）。
dst 改为 destination_block，表示目标位置（空洞）。
示例棋盘：

我们以您提供的棋盘为例（为了便于说明，我们将其转换为二维数组表示）：

lua
复制代码
Index | 0    1    2    3    4
------+-------------------------
  0   | '.'  '.'  '.'  'D'  'B'
  1   | 'C'  'D'  'A'  'A'  'C'
  2   | 'D'  'A'  'A'  'B'  'D'
  3   | 'A'  'A'  'B'  'C'  '.'
  4   | 'A'  'B'  'C'  'D'  '.'
  5   | 'B'  'C'  '.'  '.'  '.'
数组索引从顶部（0）到底部（HEIGHT - 1）。

我们将逐列分析，每一列可能遇到的情况如下：

列 0：

初始状态：
sql
复制代码
Row | Content
----+---------
 5  | 'B'
 4  | 'A'
 3  | 'A'
 2  | 'D'
 1  | 'C'
 0  | '.'
从底部开始遍历（j 从 5 到 0）：

j = 5：

s->board[5][0] 是 'B'，不是空洞，跳过。
j = 4：

s->board[4][0] 是 'A'，不是空洞，跳过。
j = 3：

s->board[3][0] 是 'A'，不是空洞，跳过。
j = 2：

s->board[2][0] 是 'D'，不是空洞，跳过。
j = 1：

s->board[1][0] 是 'C'，不是空洞，跳过。
j = 0：

s->board[0][0] 是 '.'，是空洞。

寻找上方的非空字符：

up = j - 1 = -1，up 小于 0，说明上方没有非空字符。
处理：

s->board[0][0] 置为 '.'（已经是空洞）。
列 1：

初始状态：
sql
复制代码
Row | Content
----+---------
 5  | 'C'
 4  | 'B'
 3  | 'A'
 2  | 'A'
 1  | 'D'
 0  | '.'
从底部开始遍历（j 从 5 到 0）：

j = 5 到 j = 1：

s->board[j][1] 都不是空洞，跳过。
j = 0：

s->board[0][1] 是 '.'，是空洞。

寻找上方的非空字符：

up = -1，上方没有非空字符。
处理：

s->board[0][1] 置为 '.'（已经是空洞）。
列 2：

初始状态：
sql
复制代码
Row | Content
----+---------
 5  | '.'
 4  | 'C'
 3  | 'B'
 2  | 'A'
 1  | 'A'
 0  | '.'  
从底部开始遍历（j 从 5 到 0）：

j = 5：

s->board[5][2] 是 '.'，是空洞。

寻找上方的非空字符：

up = 4，s->board[4][2] 是 'C'，非空字符。
处理：

将 'C' 下移到 s->board[5][2]。
s->board[4][2] 置为 '.'。
j = 4：

s->board[4][2] 现在是 '.'，是空洞。

寻找上方的非空字符：

up = 3，s->board[3][2] 是 'B'。
处理：

将 'B' 下移到 s->board[4][2]。
s->board[3][2] 置为 '.'。
j = 3：

s->board[3][2] 现在是 '.'，是空洞。

寻找上方的非空字符：

up = 2，s->board[2][2] 是 'A'。
处理：

将 'A' 下移到 s->board[3][2]。
s->board[2][2] 置为 '.'。
j = 2：

s->board[2][2] 现在是 '.'，是空洞。

寻找上方的非空字符：

up = 1，s->board[1][2] 是 'A'。
处理：

将 'A' 下移到 s->board[2][2]。
s->board[1][2] 置为 '.'。
j = 1：

s->board[1][2] 现在是 '.'，是空洞。

寻找上方的非空字符：

up = 0，s->board[0][2] 是 '.'。

up = -1，上方没有非空字符。

处理：

s->board[1][2] 置为 '.'。
j = 0：

s->board[0][2] 是 '.'，是空洞。

寻找上方的非空字符：

up = -1，上方没有非空字符。
处理：

s->board[0][2] 置为 '.'。
总结三种可能性：

底部连续多个空洞：

如列 2，在底部遇到连续的空洞。
处理方式：不断向上寻找非空字符，将其下移填充空洞。
上方有空洞，下方有数据：

如列 1，底部是数据，上方有空洞。
处理方式：底部数据不受影响，上方空洞无法被填充。
当前位置是空洞，上方紧邻非空字符：

如列 2 中的 j = 4 和 up = 3，空洞上方直接有非空字符。
处理方式：直接将上方的非空字符下移。
2. 使用更具描述性的变量名，逐行解释代码

c
复制代码
for (int j = HEIGHT - 1; j >= 0; j--) {
    if (s->board[j][i] == '.') {
        // 当前的位置 (j, i) 是空洞，需要填充

        // 初始化上方搜索指针，指向当前空洞的上方一行
        int above_row = j - 1;

        // 向上遍历，寻找最近的非空字符
        while (above_row >= 0 && s->board[above_row][i] == '.') {
            above_row--;
        }

        if (above_row >= 0) {
            // 找到非空字符，进行下移填充
            char* source_block = &(s->board[above_row][i]);     // 源块地址
            char* destination_block = &(s->board[j][i]);        // 目标位置地址

            *destination_block = *source_block;   // 将源块的值赋给目标位置
            *source_block = '.';                  // 源块位置置为空洞

        } else {
            // 上方没有非空字符，当前位置保持为空洞
            s->board[j][i] = '.';
        }
    }
}
逐行解释：

外层循环：

c
复制代码
for (int i = 0; i < WIDTH; i++) {
    // 遍历每一列
}
内层循环：

c
复制代码
for (int j = HEIGHT - 1; j >= 0; j--) {
    // 从底部向上遍历每一行
}
检查当前位置是否为空洞：

c
复制代码
if (s->board[j][i] == '.') {
    // 如果当前位置是空洞，开始处理
}
初始化上方搜索指针：

c
复制代码
int above_row = j - 1;
above_row 表示当前空洞上方的行号。
向上寻找非空字符：

c
复制代码
while (above_row >= 0 && s->board[above_row][i] == '.') {
    above_row--;
}
当 above_row 大于等于 0 且对应位置是空洞时，继续向上移动。
判断是否找到非空字符：

c
复制代码
if (above_row >= 0) {
    // 找到非空字符，进行下移填充
} else {
    // 上方没有非空字符，当前位置保持为空洞
}
执行下移操作：

c
复制代码
char* source_block = &(s->board[above_row][i]);     // 源块地址
char* destination_block = &(s->board[j][i]);        // 目标位置地址

*destination_block = *source_block;   // 将源块的值赋给目标位置
*source_block = '.';                  // 源块位置置为空洞
将找到的非空字符下移到当前空洞位置。
将源位置置为空洞。
3. 归纳总结规律

遍历每一列，从底部向上处理：

这样可以确保下方的空洞优先被填充，模拟重力效果。
遇到空洞时，向上寻找最近的非空字符：

使用 above_row 指针向上遍历，直到找到非空字符或到达顶部。
找到非空字符后，进行下移填充：

将非空字符下移到空洞位置，源位置置为空洞。
如果上方没有非空字符，当前位置保持为空洞：

说明该列的上方已经没有可下移的块。
4. 处理不同的可能性

情况 1：底部连续空洞，上方有非空字符

空洞位置会依次被上方的非空字符填充。
情况 2：底部为非空字符，上方有空洞

底部非空字符不受影响，上方的空洞无法被填充。
情况 3：空洞上方紧邻非空字符

非空字符直接下移填充空洞，源位置置为空洞。
5. 完整的 dropblocks 函数

c
复制代码
bool dropblocks(state* s) {
    if (s == NULL){
        return false;
    }

    for (int i = 0; i < WIDTH; i++) {
        for (int j = HEIGHT - 1; j >= 0; j--) {
            if (s->board[j][i] == '.') {
                int above_row = j - 1;
                while (above_row >= 0 && s->board[above_row][i] == '.') {
                    above_row--;
                }
                if (above_row >= 0) {
                    char* source_block = &(s->board[above_row][i]);
                    char* destination_block = &(s->board[j][i]);
                    *destination_block = *source_block;
                    *source_block = '.';
                } else {
                    s->board[j][i] = '.';
                }
            }
        }
    }
    return true;
}
