// 宏定义
#define MAX_NUM_CHARS 4096 // 20行 × 200字符
#define MAX_NUM_POSTS 128
#define MAX_POST_NAME_LEN 24
#define EN 0
#define CN 1
// 包含库
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "cJSON.h"
// 函数声明
void write_lang_panel(FILE  *const, int);
void open_panel(FILE *const, int, int);
void close_panel(FILE *const, int, int);
void file_to_str(const char *const, char *const);

// 函数定义
int main() {
    struct dirent *dir_entry;
    DIR *posts_dir_ptr = opendir("../posts");
    if (posts_dir_ptr == NULL) {
        printf("Error opening posts directory.\n");
    }
    for (int i = 0; i < 3; i++) {
        dir_entry = readdir(posts_dir_ptr);
    }
    char post_names[MAX_NUM_POSTS][MAX_POST_NAME_LEN];
    int post_idx = 0;
    while (dir_entry != NULL) {
        strcpy(post_names[MAX_NUM_POSTS-post_idx-1], dir_entry->d_name);
        dir_entry = readdir(posts_dir_ptr);
        post_idx++;
    }
    closedir(posts_dir_ptr);
    char num_to_month[13][10];
    strcpy(num_to_month[0], "");
    strcpy(num_to_month[1], "January");
    strcpy(num_to_month[2], "February");
    strcpy(num_to_month[3], "March");
    strcpy(num_to_month[4], "April");
    strcpy(num_to_month[5], "May");
    strcpy(num_to_month[6], "June");
    strcpy(num_to_month[7], "July");
    strcpy(num_to_month[8], "August");
    strcpy(num_to_month[9], "September");
    strcpy(num_to_month[10], "October");
    strcpy(num_to_month[11], "November");
    strcpy(num_to_month[12], "December");
    char home_file_str[MAX_NUM_CHARS];
    file_to_str("home.json", home_file_str);
    cJSON *home_root = cJSON_Parse(home_file_str);
    cJSON *home_title[2] = { cJSON_GetObjectItem(home_root, "title_en"), cJSON_GetObjectItem(home_root, "title_cn") };
    cJSON *home_about[2] = { cJSON_GetObjectItem(home_root, "about_en"), cJSON_GetObjectItem(home_root, "about_cn") };
    cJSON *home_interests[2] = { cJSON_GetObjectItem(home_root, "interests_en"), cJSON_GetObjectItem(home_root, "interests_cn") };
    cJSON *home_mail[2] = { cJSON_GetObjectItem(home_root, "mail_en"), cJSON_GetObjectItem(home_root, "mail_cn") };
    cJSON *home_projects[2] = { cJSON_GetObjectItem(home_root, "projects_en"), cJSON_GetObjectItem(home_root, "projects_cn") };
    for (int lang = 0; lang < 2; lang++) {
        char out_file_str[8];
        if (lang == EN) {
            strcpy(out_file_str, "en.html");
        } else {
            strcpy(out_file_str, "cn.html");
        }        
        FILE *out_fp;
        out_fp = fopen(out_file_str, "w");
        if (out_fp == NULL) {
            printf("Error opening output file %s.\n", out_file_str);
        }
        fprintf(out_fp, "<!DOCTYPE html>\n");
        fprintf(out_fp, "<html>\n");
        fprintf(out_fp, "    <head>\n");
        fprintf(out_fp, "        <meta charset=\"utf-8\">\n");
        fprintf(out_fp, "        <title>%s</title>\n", home_title[lang]->valuestring);
        fprintf(out_fp, "        <link rel=\"stylesheet\" href=\"style.css\">\n");
        fprintf(out_fp, "    </head>\n");
        fprintf(out_fp, "    <body>\n");
        fprintf(out_fp, "    <h1>Tom Jiao - tom21487.github.io</h1>\n");
        write_lang_panel(out_fp, lang);
        fprintf(out_fp, "    <p>\n");
        fprintf(out_fp, "        %s\n", home_about[lang]->valuestring);
        fprintf(out_fp, "    </p>\n");
        fprintf(out_fp, "    <p>\n");
        fprintf(out_fp, "        %s\n", home_interests[lang]->valuestring);
        fprintf(out_fp, "        <br>\n");
        fprintf(out_fp, "        %stomjiao@umich.edu\n", home_mail[lang]->valuestring);
        fprintf(out_fp, "    </p>\n");
        fprintf(out_fp, "    <p>\n");
        fprintf(out_fp, "        %s\n", home_projects[lang]->valuestring);
        fprintf(out_fp, "    </p>\n");
        for (int i = MAX_NUM_POSTS-post_idx; i < MAX_NUM_POSTS; i++) {
            char post_path[19+MAX_POST_NAME_LEN] = "../posts/";
            strcat(post_path, post_names[i]);
            strcat(post_path, "/post.json");
            char post_file_str[MAX_NUM_CHARS];
            file_to_str(post_path, post_file_str);
            cJSON *post_root = cJSON_Parse(post_file_str); //解析成json形式
            cJSON *post_title[2] = { cJSON_GetObjectItem(post_root, "title_en"), cJSON_GetObjectItem(post_root, "title_cn") }; //获取键值内容
            cJSON *post_date = cJSON_GetObjectItem(post_root, "date");
            cJSON *post_date_year = cJSON_GetObjectItem(post_date, "year");
            cJSON *post_date_month = cJSON_GetObjectItem(post_date, "month");
            cJSON *post_desc[2] = { cJSON_GetObjectItem(post_root, "desc_en"), cJSON_GetObjectItem(post_root, "desc_cn") }; //获取键值内容
            // 英文文件
            char year_string[5];
            sprintf(year_string, "%d", post_date_year->valueint);
            fprintf(out_fp, "    <div class=\"project\">\n");
            fprintf(out_fp, "        <h2>\n");
            fprintf(out_fp, "            <a class=\"btn\" href=\"\" target=\"_blank\" rel=\"noopener noreferrer\">\n");
            fprintf(out_fp, "                %s\n", post_title[lang]->valuestring);
            fprintf(out_fp, "            </a>\n");
            fprintf(out_fp, "            <span class=\"date\">\n");
            fprintf(out_fp, "                 %s %s\n", num_to_month[post_date_month->valueint], year_string);
            fprintf(out_fp, "            </span>\n");
            fprintf(out_fp, "        </h2>\n");
            fprintf(out_fp, "        <p>\n");
            fprintf(out_fp, "            %s\n", post_desc[lang]->valuestring);
            fprintf(out_fp, "        </p>\n");
            fprintf(out_fp, "        <img src=\"image.png\" alt=\"project%d\">\n", i-(MAX_NUM_POSTS-post_idx));
            fprintf(out_fp, "    </div>\n");
            cJSON_Delete(post_root);
        }
        fprintf(out_fp, "    </body>\n");
        fprintf(out_fp, "</html>\n");
        if (fclose(out_fp) == EOF) {
            printf("Error closing output file.\n");
        }
    }
    cJSON_Delete(home_root);
    return 0;
}

void write_lang_panel(FILE *const out_fp, int lang) {
    fprintf(out_fp, "    <p>\n");
    fprintf(out_fp, "        ");
    open_panel(out_fp, lang, EN);
    fprintf(out_fp, "En");
    close_panel(out_fp, lang, EN);
    fprintf(out_fp, " | ");
    open_panel(out_fp, lang, CN);
    fprintf(out_fp, "Cn");
    close_panel(out_fp, lang, CN);
    fprintf(out_fp, "\n");
    fprintf(out_fp, "    </p>\n");
}

void open_panel(FILE *const out_fp, int lang, int choice) {
    if (lang == choice) {
        fprintf(out_fp, "<strong>");
    } else {
        fprintf(out_fp, "<a href=\"en.html\" rel=\"noopener noreferrer\">");
    }
}

void close_panel(FILE *const out_fp, int lang, int choice) {
    if (lang == choice) {
        fprintf(out_fp, "</strong>");
    } else {
        fprintf(out_fp, "</a>");
    }
}

void file_to_str(const char *const file_path, char *const file_str) {
    FILE *in_fp = fopen(file_path, "r");
    if (in_fp == NULL) {
        printf("Error opening input file %s\n", file_path);
    }
    int char_idx = 0;
    while (char_idx < MAX_NUM_CHARS) {
        int file_char = fgetc(in_fp);
        if (file_char == EOF) {
            // printf("End of file reached at char %d.\n", i);
            break;
        }
        file_str[char_idx++] = (char)file_char;
    }
    file_str[char_idx] = '\0';
    if (fclose(in_fp) == EOF) {
        printf("Error closing input file.\n");
    }
}
