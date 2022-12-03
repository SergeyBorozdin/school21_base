#ifndef SRC_SRC_S1_CAT_H_
#define SRC_SRC_S1_CAT_H_

void numbers_not_empty_lines(char* path); //-b (GNU: --number-nonblank)  нумерует только непустые строки
void numbers_all_lines(char* path); //-n (GNU: --number)  нумерует все выходные строки
void end_of_line_s(char* path); // -e предполагает и -v (GNU only: -E то же самое, но без применения -v) также отображает символы конца строки как $
void remove_empty_lines(char* path); //-s (GNU: --squeeze-blank)  сжимает несколько смежных пустых строк
void change_tab(char* path); // -t implies -v (GNU: -T the same, but without implying -v)  также отображает табы как ^I


#endif  // SRC_S1_CAT_H_