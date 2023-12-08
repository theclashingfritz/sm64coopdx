#include "geo_header.h"
// 0x16000CA4
const GeoLayout warios_cap_geo[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_SHADOW(SHADOW_CIRCLE_9_VERTS, 0xB4, 75),
      GEO_OPEN_NODE(),
         GEO_SCALE(0x00, 16384),
         GEO_OPEN_NODE(),
            GEO_ASM(LAYER_OPAQUE + 3, geo_mario_set_player_colors),
            GEO_ASM(LAYER_ALPHA + 3, geo_mario_set_player_colors),
            GEO_ASM(LAYER_TRANSPARENT + 3, geo_mario_set_player_colors),
            GEO_ASM(10, geo_update_layer_transparency),
            GEO_SWITCH_CASE(2, geo_switch_anim_state),
            GEO_OPEN_NODE(),
               GEO_NODE_START(),
               GEO_OPEN_NODE(),
                  GEO_DISPLAY_LIST(LAYER_OPAQUE, wario_cap_seg3_dl_03022F48),
               GEO_CLOSE_NODE(),
               GEO_NODE_START(),
               GEO_OPEN_NODE(),
                  GEO_DISPLAY_LIST(LAYER_TRANSPARENT, wario_cap_seg3_dl_03022F48),
               GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
         GEO_CLOSE_NODE(),
      GEO_CLOSE_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, wario_cap_material_revert_render_settings),
      GEO_DISPLAY_LIST(LAYER_ALPHA, wario_cap_material_revert_render_settings),
      GEO_DISPLAY_LIST(LAYER_TRANSPARENT, wario_cap_material_revert_render_settings),
   GEO_CLOSE_NODE(),
   GEO_END(),
};

// 0x16000CF0
const GeoLayout warios_metal_cap_geo[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_SHADOW(SHADOW_CIRCLE_9_VERTS, 0xB4, 75),
      GEO_OPEN_NODE(),
         GEO_SCALE(0x00, 16384),
         GEO_OPEN_NODE(),
            GEO_ASM(LAYER_OPAQUE + 3, geo_mario_set_player_colors),
            GEO_ASM(LAYER_ALPHA + 3, geo_mario_set_player_colors),
            GEO_ASM(LAYER_TRANSPARENT + 3, geo_mario_set_player_colors),
            GEO_ASM(10, geo_update_layer_transparency),
            GEO_SWITCH_CASE(2, geo_switch_anim_state),
            GEO_OPEN_NODE(),
               GEO_DISPLAY_LIST(LAYER_OPAQUE, wario_cap_seg3_dl_03022FF8),
               GEO_DISPLAY_LIST(LAYER_TRANSPARENT, wario_cap_seg3_dl_03022FF8),
            GEO_CLOSE_NODE(),
         GEO_CLOSE_NODE(),
      GEO_CLOSE_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, wario_cap_material_revert_render_settings),
      GEO_DISPLAY_LIST(LAYER_ALPHA, wario_cap_material_revert_render_settings),
      GEO_DISPLAY_LIST(LAYER_TRANSPARENT, wario_cap_material_revert_render_settings),
   GEO_CLOSE_NODE(),
   GEO_END(),
};

// 0x16000D3C
const GeoLayout warios_wing_cap_geo[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_SHADOW(SHADOW_CIRCLE_9_VERTS, 0xB4, 75),
      GEO_OPEN_NODE(),
         GEO_SCALE(0x00, 16384),
         GEO_OPEN_NODE(),
            GEO_ASM(LAYER_OPAQUE + 3, geo_mario_set_player_colors),
            GEO_ASM(LAYER_ALPHA + 3, geo_mario_set_player_colors),
            GEO_ASM(LAYER_TRANSPARENT + 3, geo_mario_set_player_colors),
            GEO_ASM(10, geo_update_layer_transparency),
            GEO_SWITCH_CASE(2, geo_switch_anim_state),
            GEO_OPEN_NODE(),
               GEO_NODE_START(),
               GEO_OPEN_NODE(),
                  GEO_DISPLAY_LIST(LAYER_OPAQUE, wario_cap_seg3_dl_03022F48),
                  GEO_DISPLAY_LIST(LAYER_ALPHA, wario_cap_seg3_dl_030230B0),
               GEO_CLOSE_NODE(),
               GEO_NODE_START(),
               GEO_OPEN_NODE(),
                  GEO_DISPLAY_LIST(LAYER_TRANSPARENT, wario_cap_seg3_dl_03023160),
               GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
         GEO_CLOSE_NODE(),
      GEO_CLOSE_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, wario_cap_material_revert_render_settings),
      GEO_DISPLAY_LIST(LAYER_ALPHA, wario_cap_material_revert_render_settings),
      GEO_DISPLAY_LIST(LAYER_TRANSPARENT, wario_cap_material_revert_render_settings),
   GEO_CLOSE_NODE(),
   GEO_END(),
};

// 0x16000DA8
const GeoLayout warios_winged_metal_cap_geo[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_SHADOW(SHADOW_CIRCLE_9_VERTS, 0xB4, 75),
      GEO_OPEN_NODE(),
         GEO_SCALE(0x00, 16384),
         GEO_OPEN_NODE(),
            GEO_ASM(LAYER_OPAQUE + 3, geo_mario_set_player_colors),
            GEO_ASM(LAYER_ALPHA + 3, geo_mario_set_player_colors),
            GEO_ASM(LAYER_TRANSPARENT + 3, geo_mario_set_player_colors),
            GEO_ASM(10, geo_update_layer_transparency),
            GEO_SWITCH_CASE(2, geo_switch_anim_state),
            GEO_OPEN_NODE(),
               GEO_NODE_START(),
               GEO_OPEN_NODE(),
                  GEO_DISPLAY_LIST(LAYER_OPAQUE, wario_cap_seg3_dl_03022FF8),
                  GEO_DISPLAY_LIST(LAYER_ALPHA, wario_cap_seg3_dl_03023108),
               GEO_CLOSE_NODE(),
               GEO_NODE_START(),
               GEO_OPEN_NODE(),
                  GEO_DISPLAY_LIST(LAYER_TRANSPARENT, wario_cap_seg3_dl_03023298),
               GEO_CLOSE_NODE(),
            GEO_CLOSE_NODE(),
         GEO_CLOSE_NODE(),
      GEO_CLOSE_NODE(),
      GEO_DISPLAY_LIST(LAYER_OPAQUE, wario_cap_material_revert_render_settings),
      GEO_DISPLAY_LIST(LAYER_ALPHA, wario_cap_material_revert_render_settings),
      GEO_DISPLAY_LIST(LAYER_TRANSPARENT, wario_cap_material_revert_render_settings),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
