/*
 * This file contains a reimplementation of the KCL collision system used in many Nintendo games including Super Mario 64 DS.
 * This implementaiton of KCL is specific for our purposes and is not compatible with any other version of KCL.
 * References and Credits: 
 *   https://github.com/Gota7/SM64DSe-Ultimate/blob/master/Documentation/kcl_format.txt
 *   https://github.com/Gota7/SM64DSe-Ultimate/blob/master/src/core/formats/KCL.cs
 *   https://discord.com/channels/334848602130219009/1160241116583575704 (You can access this by joining the SM64DS Hacking Discord)
 */

#ifndef KCL_H
#define KCL_H

#ifdef __cplusplus
#include <new>
#include <string>

extern "C" {
#endif // __cplusplus

#include <math.h>

#include <PR/ultratypes.h>

#include "config.h"
#include "types.h"
#include "engine/math_util.h"
#include "game/memory.h"
#include "pc/fs/fs.h"
#include "pc/fs/fs_ext.h"

#ifdef __cplusplus
}
#endif // __cplusplus

struct KCLPlane {
    u32 originId;
    u32 normalId;
    u32 dirId1;
    u32 dirId2;
    u32 dirId3;
    f32 length;
    u32 terrainType;
};

struct KCLTri {
    Vec3f point1;
    Vec3f point2;
    Vec3f point3;
    Vec3f normal;
    u32 terrainType;
}

struct KCLOctreeNode {
    Vec3f size;
    Vec3f position;
    int *planeIdList;
    u32 numPlaneIds;
}

struct KCL {
    // We use this pool for allocating the needed data for both reading KCL files,
    // and storing KCL data.
    struct DynamicPool pool;
    
    Vec3f *points;
    Vec3f *vectors;
    struct KCLPlane *planes;
    struct KCLTri *tris;
    struct KCLOctreeNode *nodes;
    u32 numPoints;
    u32 numVectors;
    u32 numPlanes;
    u32 numTris;
    u32 numNodes;
}


void kcl_init(struct KCL *kcl);
void kcl_free(struct KCL *kcl);
bool kcl_raycast(struct KCL *kcl, Vec3f start, Vec3f dir, OUT Vec3f hit_pos);
bool kcl_raycast_ext(struct KCL *kcl, Vec3f start, Vec3f dir, f32 dir_length, OUT Vec3f hit_pos, OUT f32 *length);
s32 kcl_from_file(fs_file_t *file, OUT struct KCL *kcl);

bool kcl_plane_to_tri(struct KCL *kcl, u32 id);

bool kcl_octree_contains_point(struct KCLOctreeNode *node, Vec3f point);
bool kcl_octree_intersects_ray(struct KCLOctreeNode *node, Vec3f start, Vec3f dir);

#endif // KCL_H