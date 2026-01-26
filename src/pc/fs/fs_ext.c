#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "fs_ext.h"

int64_t fs_read_s8(fs_file_t *file, OUT s8 *val) {
    if (!val) { return -1; }
    return fs_read(file, val, sizeof(s8));
}

int64_t fs_read_u8(fs_file_t *file, OUT u8 *val) {
    if (!val) { return -1; }
    return fs_read(file, val, sizeof(u8));
}

int64_t fs_read_s16(fs_file_t *file, OUT s16 *val) {
    if (!val) { return -1; }
    return fs_read(file, val, sizeof(s16));
}

int64_t fs_read_u16(fs_file_t *file, OUT u16 *val) {
    if (!val) { return -1; }
    return fs_read(file, val, sizeof(u16));
}

int64_t fs_read_s32(fs_file_t *file, OUT s32 *val) {
    if (!val) { return -1; }
    return fs_read(file, val, sizeof(s32));
}

int64_t fs_read_u32(fs_file_t *file, OUT u32 *val) {
    if (!val) { return -1; }
    return fs_read(file, val, sizeof(u32));
}

int64_t fs_read_f32(fs_file_t *file, OUT f32 *val) {
    if (!val) { return -1; }
    return fs_read(file, val, sizeof(f32));
}

int64_t fs_read_s64(fs_file_t *file, OUT s64 *val) {
    if (!val) { return -1; }
    return fs_read(file, val, sizeof(s64));
}

int64_t fs_read_u64(fs_file_t *file, OUT u64 *val) {
    if (!val) { return -1; }
    return fs_read(file, val, sizeof(u64));
}

int64_t fs_read_f64(fs_file_t *file, OUT f64 *val) {
    if (!val) { return -1; }
    return fs_read(file, val, sizeof(f64));
}

int64_t fs_read_vec2s(fs_file_t *file, OUT Vec2s val) {
    if (fs_remaining(file) < sizeof(Vec2s)) { return -1; }
    
    fs_read_s16(file, &val[0]);
    return fs_read_s16(file, &val[1]);
}

int64_t fs_read_vec3s(fs_file_t *file, OUT Vec3s val) {
    if (fs_remaining(file) < sizeof(Vec3s)) { return -1; }
    
    fs_read_s16(file, &val[0]);
    fs_read_s16(file, &val[1]);
    return fs_read_s16(file, &val[2]);
}

int64_t fs_read_vec4s(fs_file_t *file, OUT Vec4s val) {
    if (fs_remaining(file) < sizeof(Vec4s)) { return -1; }
    
    fs_read_s16(file, &val[0]);
    fs_read_s16(file, &val[1]);
    fs_read_s16(file, &val[2]);
    return fs_read_s16(file, &val[3]);
}

int64_t fs_read_vec2i(fs_file_t *file, OUT Vec2i val) {
    if (fs_remaining(file) < sizeof(Vec2i)) { return -1; }
    
    fs_read_s32(file, &val[0]);
    return fs_read_s32(file, &val[1]);
}

int64_t fs_read_vec3i(fs_file_t *file, OUT Vec3i val) {
    if (fs_remaining(file) < sizeof(Vec3i)) { return -1; }
    
    fs_read_s32(file, &val[0]);
    fs_read_s32(file, &val[1]);
    return fs_read_s32(file, &val[2]);
}

int64_t fs_read_vec4i(fs_file_t *file, OUT Vec4i val) {
    if (fs_remaining(file) < sizeof(Vec4i)) { return -1; }
    
    fs_read_s32(file, &val[0]);
    fs_read_s32(file, &val[1]);
    fs_read_s32(file, &val[2]);
    return fs_read_s32(file, &val[3]);
}

int64_t fs_read_vec2f(fs_file_t *file, OUT Vec2f val) {
    if (fs_remaining(file) < sizeof(Vec2f)) { return -1; }
    
    fs_read_f32(file, &val[0]);
    return fs_read_f32(file, &val[1]);
}

int64_t fs_read_vec3f(fs_file_t *file, OUT Vec3f val) {
    if (fs_remaining(file) < sizeof(Vec3f)) { return -1; }
  
    fs_read_f32(file, &val[0]);
    fs_read_f32(file, &val[1]);
    return fs_read_f32(file, &val[2]);
}

int64_t fs_read_vec4f(fs_file_t *file, OUT Vec4f val) {
    if (fs_remaining(file) < sizeof(Vec4f)) { return -1; }
    
    fs_read_f32(file, &val[0]);
    fs_read_f32(file, &val[1]);
    fs_read_f32(file, &val[2]);
    return fs_read_f32(file, &val[3]);
}

int64_t fs_read_mat4(fs_file_t *file, OUT Mat4 val) {
    if (fs_remaining(file) < sizeof(Mat4)) { return -1; }
    
    fs_read_f32(file, &val[0][0]);
    fs_read_f32(file, &val[0][1]);
    fs_read_f32(file, &val[0][2]);
    fs_read_f32(file, &val[0][3]);
    fs_read_f32(file, &val[1][0]);
    fs_read_f32(file, &val[1][1]);
    fs_read_f32(file, &val[1][2]);
    fs_read_f32(file, &val[1][3]);
    fs_read_f32(file, &val[2][0]);
    fs_read_f32(file, &val[2][1]);
    fs_read_f32(file, &val[2][2]);
    fs_read_f32(file, &val[2][3]);
    fs_read_f32(file, &val[3][0]);
    fs_read_f32(file, &val[3][1]);
    fs_read_f32(file, &val[3][2]);
    return fs_read_f32(file, &val[3][3]);
}