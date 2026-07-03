#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#define MAX_ATTR_VALUE_LEN 64
#define MAX_ATTR_NAME_LEN 32
#define MAX_OBJECT_NAME_LEN 64
#define MAX_ATTRIBUTES 20

// Một thuộc tính
typedef struct {
    char name[MAX_ATTR_NAME_LEN];       // tên thuộc tính: màu sắc, kích thước,...
    char value[MAX_ATTR_VALUE_LEN];     // giá trị: đỏ, to, 2kg,...
    float confidence;                    // độ tin cậy
} Attribute;

// Danh sách thuộc tính của một sự vật
typedef struct {
    char object[MAX_OBJECT_NAME_LEN];   // tên sự vật: xe, nhà, áo,...
    Attribute attrs[MAX_ATTRIBUTES];
    int count;
} ObjectAttributes;

// Hàm phân tích
ObjectAttributes* extract_attributes(const char *input);
void free_attributes(ObjectAttributes *obj);

// Hàm sinh mô tả
char* describe_object(ObjectAttributes *obj);
char* generate_attribute_response(ObjectAttributes *obj);

#endif
