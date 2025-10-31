/*
 * Copyright (c) 2025, Miguel Sacristán Izcue <miguel_tete17@hotmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibJS/Forward.h>

#include <AK/Utf16String.h>
#include <LibWeb/Bindings/LockManagerPrototype.h>
#include <LibWeb/Bindings/PlatformObject.h>

namespace Web::WebLocks {

struct LockOptions {
    Optional<Bindings::LockMode> mode = Bindings::LockMode::Exclusive;
    Optional<bool> if_available = false;
    Optional<bool> steal = false;
    Optional<GC::Root<DOM::AbortSignal>> signal;
};

class LockManager final : public Bindings::PlatformObject {
    WEB_PLATFORM_OBJECT(LockManager, Bindings::PlatformObject);
    GC_DECLARE_ALLOCATOR(LockManager);

public:
    virtual ~LockManager() override = default;

    // https://www.w3.org/TR/web-locks/#dom-lockmanager-request
    GC::Ref<WebIDL::Promise> request(Utf16String const& name, GC::Ref<WebIDL::CallbackType>& callback);
    GC::Ref<WebIDL::Promise> request(Utf16String const& name, LockOptions options, GC::Ref<WebIDL::CallbackType>& callback);

private:
    explicit LockManager(JS::Realm&);
    virtual void initialize(JS::Realm&) override;
};

}
