#pragma once
#include "il2cpp.hpp"
#include <vector>
#include <cmath>

namespace game {
    inline std::vector<void*> find_players() {
        static void* cls_obj = nullptr;
        static void* m_find  = nullptr;
        static void* cls_pc  = nullptr;
        static void* sys_type = nullptr;

        if (!cls_obj) {
            cls_obj = il2::class_from_name(il2::image, "UnityEngine", "Object");
            if (cls_obj) m_find = il2::class_get_method_from_name(cls_obj, "FindObjectsOfType", 1);
            cls_pc = il2::class_from_name(il2::image, "Axlebolt.Standoff.Player", "PlayerController");
            if (cls_pc && il2::class_get_type && il2::type_get_object) {
                sys_type = il2::type_get_object(il2::class_get_type(cls_pc));
            }
        }
        if (!m_find || !sys_type) return {};

        void* args[1] = { sys_type };
        void* exc = nullptr;
        void* arr = il2::runtime_invoke(m_find, nullptr, args, &exc);
        if (exc || !arr) return {};

        int32_t len = *(int32_t*)((uint8_t*)arr + 0x18);
        void** items = (void**)((uint8_t*)arr + 0x20);
        if (len <= 0 || len > 128) return {};

        std::vector<void*> out;
        for (int i = 0; i < len; ++i) if (items[i]) out.push_back(items[i]);
        return out;
    }

    inline bool get_position(void* tr, float* out) {
        static void* m = nullptr;
        if (!m) {
            void* cls = il2::class_from_name(il2::image, "UnityEngine", "Transform");
            m = il2::class_get_method_from_name(cls, "get_position", 0);
        }
        if (!m) return false;
        void* exc = nullptr;
        void* b = il2::runtime_invoke(m, tr, nullptr, &exc);
        if (exc || !b) return false;
        float* f = (float*)il2::object_unbox(b);
        if (!f) return false;
        out[0] = f[0]; out[1] = f[1]; out[2] = f[2];
        return true;
    }
}
