/*
 * Copyright (c) 2025, Miguel Sacristán Izcue <miguel_tete17@hotmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibJS/Forward.h>

#include <LibWeb/Bindings/LockPrototype.h>
#include <LibWeb/Bindings/PlatformObject.h>

namespace Web::WebLocks {


class Lock final : public Bindings::PlatformObject {
    WEB_PLATFORM_OBJECT(Lock, Bindings::PlatformObject);
    GC_DECLARE_ALLOCATOR(Lock);

public:
    virtual ~Lock() override = default;

private:
    explicit Lock(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
};

}
