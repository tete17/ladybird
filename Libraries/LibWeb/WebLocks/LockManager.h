/*
 * Copyright (c) 2025, Miguel Sacristán Izcue <miguel_tete17@hotmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibJS/Forward.h>

#include <LibWeb/Bindings/LockManagerPrototype.h>
#include <LibWeb/Bindings/PlatformObject.h>

namespace Web::WebLocks {

class LockManager final : public Bindings::PlatformObject {
    WEB_PLATFORM_OBJECT(LockManager, Bindings::PlatformObject);
    GC_DECLARE_ALLOCATOR(LockManager);

public:
    virtual ~LockManager() override = default;

private:
    explicit LockManager(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
};

}
