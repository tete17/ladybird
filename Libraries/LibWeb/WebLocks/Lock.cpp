/*
 * Copyright (c) 2025, Miguel Sacristán Izcue <miguel_tete17@hotmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibWeb/WebLocks/Lock.h>

#include <LibWeb/Bindings/Intrinsics.h>

namespace Web::WebLocks {

GC_DEFINE_ALLOCATOR(Lock);

Lock::Lock(JS::Realm& realm)
    : PlatformObject(realm)
{
}

void Lock::initialize(JS::Realm& realm)
{
    WEB_SET_PROTOTYPE_FOR_INTERFACE(Lock);
    Base::initialize(realm);
}

}
