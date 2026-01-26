#ifndef _SM64_FS_EXT_H_
#define _SM64_FS_EXT_H_

#include "config.h"
#include "types.h"

#include "fs.h"

/* Standard Types */

int64_t fs_read_s8(fs_file_t *file, OUT s8 *val);
int64_t fs_read_u8(fs_file_t *file, OUT u8 *val);
int64_t fs_read_s16(fs_file_t *file, OUT s16 *val);
int64_t fs_read_u16(fs_file_t *file, OUT u16 *val);
int64_t fs_read_s32(fs_file_t *file, OUT s32 *val);
int64_t fs_read_u32(fs_file_t *file, OUT u32 *val);
int64_t fs_read_f32(fs_file_t *file, OUT f32 *val);
int64_t fs_read_s64(fs_file_t *file, OUT s64 *val);
int64_t fs_read_u64(fs_file_t *file, OUT u64 *val);
int64_t fs_read_f64(fs_file_t *file, OUT f64 *val);

/* Structures and Arrays
   If any of these error. The data will liekly be malformed.
   On error they will return -1. 
*/

int64_t fs_read_vec2s(fs_file_t *file, OUT Vec2s val);
int64_t fs_read_vec3s(fs_file_t *file, OUT Vec3s val);
int64_t fs_read_vec4s(fs_file_t *file, OUT Vec4s val);
int64_t fs_read_vec2i(fs_file_t *file, OUT Vec2i val);
int64_t fs_read_vec3i(fs_file_t *file, OUT Vec3i val);
int64_t fs_read_vec4i(fs_file_t *file, OUT Vec4i val);
int64_t fs_read_vec2f(fs_file_t *file, OUT Vec2f val);
int64_t fs_read_vec3f(fs_file_t *file, OUT Vec3f val);
int64_t fs_read_vec4f(fs_file_t *file, OUT Vec4f val);
int64_t fs_read_mat4(fs_file_t *file, OUT Mat4 val);

#endif