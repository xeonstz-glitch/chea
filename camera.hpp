#pragma once
#include "il2cpp.hpp"
#include "config.hpp"
#include <cmath>

namespace game {
    // статический синглтон PlayerMainCamera
    inline void* get_main_cam_singleton() {
        static void* m = nullptr;
        if (!m) {
            void* cls = il2::class_from_name(il2::image, "Axlebolt.Standoff.Player", "PlayerMainCamera");
            if (cls) m = il2::class_get_method_from_name(cls, "get_HGGHFBCAGEEGBDH", 0);
        }
        if (!m) return nullptr;
        void* exc = nullptr;
        void* b = il2::runtime_invoke(m, nullptr, nullptr, &exc);
        if (exc || !b) return nullptr;
        return *(void**)il2::object_unbox(b);
    }

    // Camera из синглтона
    inline void* get_camera() {
        void* pmc = get_main_cam_singleton();
        if (!pmc) return nullptr;
        return *(void**)((uint8_t*)pmc + cfg::o_camera);
    }

    // локальный PlayerController
    inline void* get_local_player() {
        void* pmc = get_main_cam_singleton();
        if (!pmc) return nullptr;
        return *(void**)((uint8_t*)pmc + cfg::o_local_pc);
    }

    // матрицы камеры
    inline bool get_mvp(void* cam, float out[16]) {
        static void* m_view = nullptr;
        static void* m_proj = nullptr;
        if (!m_view) {
            void* cls = il2::class_from_name(il2::image, "UnityEngine", "Camera");
            m_view = il2::class_get_method_from_name(cls, "get_worldToCameraMatrix", 0);
            m_proj = il2::class_get_method_from_name(cls, "get_projectionMatrix", 0);
        }
        if (!m_view || !m_proj) return false;
        auto call = [](void* meth, void* obj, float* outm) -> bool {
            void* exc = nullptr;
            void* b = il2::runtime_invoke(meth, obj, nullptr, &exc);
            if (exc || !b) return false;
            float* f = (float*)il2::object_unbox(b);
            if (!f) return false;
            for (int i = 0; i < 16; ++i) outm[i] = f[i];
            return true;
        };
        float V[16], P[16];
        if (!call(m_view, cam, V)) return false;
        if (!call(m_proj, cam, P)) return false;
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c) {
                float s = 0;
                for (int k = 0; k < 4; ++k) s += P[r*4+k] * V[k*4+c];
                out[r*4+c] = s;
            }
        return true;
    }

    inline bool w2s(const float* m, float x, float y, float z, float W, float H, float* sx, float* sy) {
        float cx = x*m[0] + y*m[4] + z*m[8]  + m[12];
        float cy = x*m[1] + y*m[5] + z*m[9]  + m[13];
        float cw = x*m[3] + y*m[7] + z*m[11] + m[15];
        if (cw < 0.001f) return false;
        *sx = (W * 0.5f) + (cx / cw) * (W * 0.5f);
        *sy = (H * 0.5f) - (cy / cw) * (H * 0.5f);
        return true;
    }
}
