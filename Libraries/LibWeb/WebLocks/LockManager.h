/*
 * Copyright (c) 2025, Miguel Sacristán Izcue <miguel_tete17@hotmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibJS/Forward.h>

#include <AK/Queue.h>
#include <AK/Utf16String.h>
#include <LibWeb/Bindings/LockManagerPrototype.h>
#include <LibWeb/Bindings/PlatformObject.h>

namespace Web::WebLocks {
namespace Impl {
struct LockManager;
}

// https://www.w3.org/TR/web-locks/#concept-lock-request
struct LockRequest {
    const HTML::Agent& agent;
    String client_id;
    Impl::LockManager& manager;
    Utf16String name;
    Bindings::LockMode mode;
    GC::Root<WebIDL::CallbackType> callback;
    GC::Root<WebIDL::Promise> promise;
    Optional<GC::Root<DOM::AbortSignal>> signal;
};

namespace Impl {

// https://www.w3.org/TR/web-locks/#lock-manager
struct LockManager {
    AK_MAKE_NONCOPYABLE(LockManager);

public:
    LockManager() = default;

    // https://www.w3.org/TR/web-locks/#ref-for-lock-manager%E2%91%A4
    HashMap<Utf16String, Queue<LockRequest>> lock_request_queue_map;
};

}

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

// https://www.w3.org/TR/web-locks/#obtain-a-lock-manager
Optional<Impl::LockManager&> obtain_a_lock_manager(HTML::EnvironmentSettingsObject& environment);

// https://www.w3.org/TR/web-locks/#request-a-lock
LockRequest request_a_lock(
    GC::Ref<WebIDL::Promise> promise,
    const HTML::Agent& agent,
    String const& client_id,
    Impl::LockManager& manager,
    GC::Ref<WebIDL::CallbackType> callback,
    Utf16String const& name,
    Bindings::LockMode mode,
    bool if_available,
    bool steal,
    Optional<GC::Root<DOM::AbortSignal>>& signal);

}
