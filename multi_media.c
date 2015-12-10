#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int simple_search(int *pos, char *text, char *key) {
  int text_len = strlen(text);
  int key_len = strlen(key);

  int loop_cnt = text_len - (key_len - 1);
  int search_cnt = 0;
  int i, j;
  int find = 0;
  int find_cnt = 0;

  for (i = 0; i < loop_cnt; i++) {
    find = 1;

    for (j = 0; j < key_len; j++) {
      search_cnt++;

      if (text[i + j] != key[j]) {
        find = 0;
        break;
      }
    }

    if (find == 1) {
      pos[find_cnt] = i;
      find_cnt++;
    }
  }

  printf("検索回数 %d\n", search_cnt);
  return find_cnt;
}

char *hash_key;
int hash_cnt = 0;

void setup_skip(char *key) {
  int key_len = strlen(key);
  int loop_cnt = key_len - 1;
  int i;

  hash_key = (char *)malloc(sizeof(char) * key_len);

  for (i = 1; i < loop_cnt; i++) {
    if (strchr(hash_key, key[key_len - 1 - i]) == NULL) {
      hash_key[hash_cnt] = key[key_len - 1 - i];
      hash_cnt++;
    }
  }
}

int skip(int key_len, char c) {
  char *pos = strchr(hash_key, c);

  if (pos == NULL) {
    return key_len;
  } else {
    return pos - hash_key + 1;
  }
}

int bm_search(int *pos, char *text, char *key) {
  int text_len = strlen(text);
  int key_len = strlen(key);

  int loop_cnt = text_len - (key_len - 1);
  int search_cnt = 0;
  int search_index = key_len - 1;
  int i;
  int find = 0;
  int find_cnt = 0;

  setup_skip(key);

  while (search_index < text_len) {
    find = 1;

    for (i = 0; i < key_len; i++) {
      search_cnt++;

      if (text[search_index - i] != key[key_len - 1 - i]) {
        find = 0;
        break;
      }
    }

    if (find == 1) {
      pos[find_cnt] = search_index - (key_len - 1);
      find_cnt++;
    }

    search_index += skip(key_len, text[search_index]);
  }

  printf("検索回数 %d\n", search_cnt);
  return find_cnt;
}

void main() {
  char *text = "abcabcababcaba";
  char *key = "abcaba";

  int pos[200];

  clock_t start, end;

  printf("\n-------------------------------------\n");

  printf("key = %s\n", key);
  printf("text = %s\n", text);

  printf("\n-------------------------------------\n");

  printf("Simple Search\n");
  start = clock();
  int find_cnt = simple_search(pos, text, key);
  end = clock();

  char *unit;
  if (CLOCKS_PER_SEC == 1000000000) {
    unit = "ナノ秒";
  } else if (CLOCKS_PER_SEC == 1000000) {
    unit = "マイクロ秒";
  } else if (CLOCKS_PER_SEC == 1000) {
    unit = "ミリ秒";
  } else if (CLOCKS_PER_SEC == 1) {
    unit = "秒";
  }

  printf("処理時間 %d %s\n", end - start, unit);

  printf("検出位置\n");
  int i;
  for (i = 0; i < find_cnt; i++) {
    printf("%d, ", pos[i]);
  }

  printf("\n-------------------------------------\n");

  printf("BM Search\n");
  start = clock();
  find_cnt = bm_search(pos, text, key);
  end = clock();

  if (CLOCKS_PER_SEC == 1000000000) {
    unit = "ナノ秒";
  } else if (CLOCKS_PER_SEC == 1000000) {
    unit = "マイクロ秒";
  } else if (CLOCKS_PER_SEC == 1000) {
    unit = "ミリ秒";
  } else if (CLOCKS_PER_SEC == 1) {
    unit = "秒";
  }

  printf("処理時間 %d %s\n", end - start, unit);

  printf("検出位置\n");
  for (i = 0; i < find_cnt; i++) {
    printf("%d, ", pos[i]);
  }
  printf("\n");
}
