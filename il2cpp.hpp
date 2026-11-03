#pragma once
#include <dlfcn.h>
#include <cstdint>
#include <string>

namespace il2 {
    inline void* (*domain_get)() = nullptr;
    inline void* (*domain_assembly_open)(void*, const char*) = nullptr;
    inline void* (*assembly_get_image)(void*) = nullptr;
    inline void* (*class_from_name)(void*, const char*, const char*) = nullptr;
    inline void* (*class_get_field_from_name)(void*, const char*) = nullptr;
    inline size_t (*field_get_offset)(void*) = nullptr;
    inline void* (*class_get_method_from_name)(void*, const char*, int) = nullptr;
    inline void* (*runtime_invoke)(void*, void*, void**, void**) = nullptr;
    inline void* (*object_unbox)(void*) = nullptr;
    inline void* (*class_get_type)(void*) = nullptr;
    inline void* (*type_get_object)(void*) = nullptr;
    inline void* image = nullptr;

    inline bool init() {
        void* h = dlopen("UnityFramework", RTLD_LAZY | RTLD_NOLOAD);
        if (!h) h = dlopen(nullptr, RTLD_LAZY);
        if (!h) return false;
        
        domain_get = (decltype(domain_get))dlsym(h, "il2cpp_domain_get");
        domain_assembly_open = (decltype(domain_assembly_open))dlsym(h, "il2cpp_domain_assembly_open");
        assembly_get_image = (decltype(assembly_get_image))dlsym(h, "il2cpp_assembly_get_image");
        class_from_name = (decltype(class_from_name))dlsym(h, "il2cpp_class_from_name");
        class_get_field_from_name = (decltype(class_get_field_from_name))dlsym(h, "il2cpp_class_get_field_from_name");
        field_get_offset = (decltype(field_get_offset))dlsym(h, "il2cpp_field_get_offset");
        class_get_method_from_name = (decltype(class_get_method_from_name))dlsym(h, "il2cpp_class_get_method_from_name");
        runtime_invoke = (decltype(runtime_invoke))dlsym(h, "il2cpp_runtime_invoke");
        object_unbox = (decltype(object_unbox))dlsym(h, "il2cpp_object_unbox");
        class_get_type = (decltype(class_get_type))dlsym(h, "il2cpp_class_get_type");
        type_get_object = (decltype(type_get_object))dlsym(h, "il2cpp_type_get_object");
        
        if (!domain_get || !class_from_name) return false;
        void* dom = domain_get();
        void* asm_ = domain_assembly_open(dom, "Assembly-CSharp");
        if (!asm_) return false;
        image = assembly_get_image(asm_);
        return image != nullptr;
    }
}
