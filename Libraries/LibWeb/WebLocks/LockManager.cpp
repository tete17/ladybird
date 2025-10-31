/*
 * Copyright (c) 2025, Miguel Sacristán Izcue <miguel_tete17@hotmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibWeb/WebLocks/LockManager.h>

#include <LibWeb/Bindings/Intrinsics.h>

namespace Web::WebLocks {

GC_DEFINE_ALLOCATOR(LockManager);

LockManager::LockManager(JS::Realm& realm)
    : PlatformObject(realm)
{
}

void LockManager::initialize(JS::Realm& realm)
{
    WEB_SET_PROTOTYPE_FOR_INTERFACE(LockManager);
    Base::initialize(realm);
}

}
