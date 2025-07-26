#ifndef GFX_DXGI_H
#define GFX_DXGI_H

#include "macros.h"
#include "gfx_rendering_api.h"

#ifdef DECLARE_GFX_DXGI_FUNCTIONS
OPTIMIZE_O3 void gfx_dxgi_create_factory_and_device(bool debug, int d3d_version, bool (*create_device_fn)(IDXGIAdapter1 *adapter, bool test_only));
OPTIMIZE_O3 Microsoft::WRL::ComPtr<IDXGISwapChain1> gfx_dxgi_create_swap_chain(IUnknown *device);
extern "C" HWND gfx_dxgi_get_h_wnd(void);

OPTIMIZE_O3 void ThrowIfFailedExt(HRESULT res, UNUSED const char *file, UNUSED const char *function, UNUSED u64 lineno);
OPTIMIZE_O3 void ThrowIfFailedExt(HRESULT res, HWND h_wnd, const char *message, UNUSED const char *file, UNUSED const char *function, UNUSED u64 lineno);

#define ThrowIfFailed(res) ThrowIfFailedExt(res, __FILE__, __func__, __LINE__)
#define ThrowIfFailedWithMsgBox(res, h_wnd, message) ThrowIfFailedExt(res, h_wnd, message, __FILE__, __func__, __LINE__)
#endif

extern struct GfxWindowManagerAPI gfx_dxgi;

#endif
