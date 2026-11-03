#pragma once
#include <cstddef>

namespace cfg {
    // PlayerController — оффсеты из дампа
    constexpr size_t o_health     = 0x5C;   // float CHDAEHHBGDDDEAD
    constexpr size_t o_team       = 0x59;   // Team  AGACDDDCBFFGEHF
    constexpr size_t o_transform  = 0xD8;   // Transform CBCBGACGGECGEAA
    constexpr size_t o_photonview = 0x120;  // PhotonView HHDBBBDEBCGEEAE
    constexpr size_t o_actor_id   = 0x128;  // int GCCFDFCBAFABEBF
    constexpr size_t o_owner      = 0x130;  // PhotonPlayer HHCCEADDHBAFECD

    // PlayerMainCamera — оффсеты
    constexpr size_t o_camera     = 0x18;   // Camera HBDEBCBFEEGABFB
    constexpr size_t o_local_pc   = 0x40;   // PlayerController EBEGGBGAEAHAGFC

    // рендер
    constexpr float  max_dist     = 500.0f;
}
