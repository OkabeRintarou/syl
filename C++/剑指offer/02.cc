class Solution {
public:
    void replaceSpace(char *str, int length) {
        int count = 0;
        char *ptr = str;
        while (*ptr) {
            if (*ptr == ' ') {
                ++count;
            }
            ptr++;
        }
        int len = length + 2 * count;
        ptr = str + len;
        *ptr = '\0';
        const char *p = str + length - 1;
        while (p != str - 1) {
            if (*p == ' ') {
                --ptr;
                *ptr = '0';
                --ptr;
                *ptr = '2';
                --ptr;
                *ptr = '%';
            } else {
                --ptr;
                *ptr = *p;
            }
            --p;
        }
    }
};